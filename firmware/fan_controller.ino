#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ============================================================
// Arduino-Controlled 3D Printer Enclosure
//
// Hardware:
// - Arduino Uno R3
// - LM35 temperature sensor
// - 10 kOhm potentiometer
// - N-channel MOSFET controlling two DC fans
// - 16x2 I2C LCD
//
// Control behavior:
// - Potentiometer selects a desired temperature from 25-40 C.
// - Fans remain off when measured temperature is at/below setpoint.
// - Above the setpoint, fan PWM increases proportionally.
// - Fans reach full commanded PWM approximately 10 C above setpoint.
// - PWM output is smoothed to reduce abrupt speed changes.
// ============================================================


// -------------------- LCD --------------------

// Common I2C address for PCF8574-based 16x2 LCD modules.
// If the physical display does not respond, some modules use 0x3F.
LiquidCrystal_I2C lcd(0x27, 16, 2);


// -------------------- Pin assignments --------------------

const int TEMP_PIN = A0;   // LM35 analog output
const int POT_PIN = A1;    // Potentiometer wiper
const int FAN_PWM_PIN = 3; // PWM output to MOSFET gate circuit


// -------------------- Analog settings --------------------

const float ADC_REFERENCE_VOLTAGE = 5.0;
const int ADC_MAX_VALUE = 1023;

const int NUM_SAMPLES = 20;
const int SAMPLE_DELAY_MS = 2;


// -------------------- Temperature settings --------------------

// Adjustable setpoint range controlled by potentiometer.
const float MIN_SETPOINT_C = 25.0;
const float MAX_SETPOINT_C = 40.0;

// LM35 output is approximately 10 mV / degree C
// with approximately 0 V corresponding to 0 C.
const float LM35_VOLTS_PER_C = 0.010;


// -------------------- Fan settings --------------------

// Minimum PWM command used once cooling is requested.
//
// A value greater than zero is useful because many DC fans will
// not reliably turn at extremely small PWM duty cycles.
const int MIN_ACTIVE_PWM = 80;
const int MAX_PWM = 255;

// Fan reaches maximum commanded PWM when temperature is this
// many degrees above the selected setpoint.
const float FULL_SPEED_DELTA_C = 10.0;

// Smoothing factor:
// 0.0 = output never changes
// 1.0 = output immediately reaches target
//
// 0.10 gives gradual changes while remaining responsive.
const float PWM_SMOOTHING_FACTOR = 0.10;

float smoothedPWM = 0.0;


// -------------------- Timing --------------------

const unsigned long UPDATE_INTERVAL_MS = 500;
unsigned long previousUpdateTime = 0;


// ============================================================
// Helper functions
// ============================================================

// Read an analog pin several times and return the average.
//
// Averaging helps reduce small fluctuations in the ADC reading.
float readAverageADC(int pin) {
  long sum = 0;

  for (int i = 0; i < NUM_SAMPLES; i++) {
    sum += analogRead(pin);
    delay(SAMPLE_DELAY_MS);
  }

  return sum / (float)NUM_SAMPLES;
}


// Convert a value from one floating-point range to another.
//
// Arduino's standard map() function operates using integer
// arithmetic, so this function is used when temperatures or
// averaged ADC readings contain decimal values.
float mapFloat(
  float value,
  float inputMin,
  float inputMax,
  float outputMin,
  float outputMax
) {
  return (value - inputMin)
         * (outputMax - outputMin)
         / (inputMax - inputMin)
         + outputMin;
}


// Read the LM35 and return temperature in degrees Celsius.
float readTemperatureC() {
  float adcReading = readAverageADC(TEMP_PIN);

  float voltage =
    adcReading * (ADC_REFERENCE_VOLTAGE / ADC_MAX_VALUE);

  // LM35: approximately 10 mV per degree Celsius.
  return voltage / LM35_VOLTS_PER_C;
}


// Read the potentiometer and convert it into the user's
// desired enclosure temperature.
float readSetpointC() {
  float potReading = readAverageADC(POT_PIN);

  float setpoint = mapFloat(
    potReading,
    0.0,
    ADC_MAX_VALUE,
    MIN_SETPOINT_C,
    MAX_SETPOINT_C
  );

  return constrain(
    setpoint,
    MIN_SETPOINT_C,
    MAX_SETPOINT_C
  );
}


// Calculate the required fan PWM command.
//
// At or below the setpoint:
//     fan target = 0
//
// Immediately above the setpoint:
//     fan target begins at MIN_ACTIVE_PWM
//
// At setpoint + FULL_SPEED_DELTA_C:
//     fan target = 255
//
// Above that:
//     command remains at 255.
int calculateTargetPWM(
  float temperatureC,
  float setpointC
) {
  if (temperatureC <= setpointC) {
    return 0;
  }

  float pwm = mapFloat(
    temperatureC,
    setpointC,
    setpointC + FULL_SPEED_DELTA_C,
    MIN_ACTIVE_PWM,
    MAX_PWM
  );

  pwm = constrain(
    pwm,
    (float)MIN_ACTIVE_PWM,
    (float)MAX_PWM
  );

  return (int)round(pwm);
}


// Update the smoothed PWM value and return the integer command
// that will actually be sent to analogWrite().
int updateFanPWM(int targetPWM) {

  smoothedPWM +=
    (targetPWM - smoothedPWM) * PWM_SMOOTHING_FACTOR;

  // Prevent the floating-point smoother from sitting forever
  // just above zero after the fan has been commanded off.
  if (targetPWM == 0 && smoothedPWM < 1.0) {
    smoothedPWM = 0.0;
  }

  // Likewise, settle exactly on the upper target.
  if (
    targetPWM == MAX_PWM &&
    smoothedPWM > MAX_PWM - 1.0
  ) {
    smoothedPWM = MAX_PWM;
  }

  int pwmOutput = (int)round(smoothedPWM);

  return constrain(
    pwmOutput,
    0,
    MAX_PWM
  );
}


// Update the 16x2 LCD.
//
// Clearing each row before writing prevents leftover characters
// when a value changes from a longer number to a shorter one.
void updateLCD(
  float temperatureC,
  float setpointC,
  int fanPercent,
  bool fanOn
) {

  // Clear first row.
  lcd.setCursor(0, 0);
  lcd.print("                ");

  // Display measured temperature.
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temperatureC, 1);
  lcd.print((char)223); // LCD degree symbol
  lcd.print("C");


  // Clear second row.
  lcd.setCursor(0, 1);
  lcd.print("                ");

  // Display desired temperature and commanded fan percentage.
  lcd.setCursor(0, 1);
  lcd.print("Des:");
  lcd.print(setpointC, 1);

  lcd.setCursor(9, 1);
  lcd.print(fanPercent);
  lcd.print("%");

  // Status indicator in final LCD column.
  lcd.setCursor(15, 1);

  if (fanOn) {
    lcd.print(">");
  } else {
    lcd.print(" ");
  }
}


// Send diagnostic information to the Serial Monitor.
void printSerialDiagnostics(
  float temperatureC,
  float setpointC,
  int targetPWM,
  int actualPWM
) {
  int fanPercent =
    (int)round((actualPWM / 255.0) * 100.0);

  Serial.print("Temperature: ");
  Serial.print(temperatureC, 1);
  Serial.print(" C");

  Serial.print(" | Setpoint: ");
  Serial.print(setpointC, 1);
  Serial.print(" C");

  Serial.print(" | Target PWM: ");
  Serial.print(targetPWM);

  Serial.print(" | Output PWM: ");
  Serial.print(actualPWM);

  Serial.print(" | Fan command: ");
  Serial.print(fanPercent);
  Serial.print("%");

  if (actualPWM > 0) {
    Serial.println(" | ON");
  } else {
    Serial.println(" | OFF");
  }
}


// ============================================================
// Arduino setup
// ============================================================

void setup() {

  pinMode(FAN_PWM_PIN, OUTPUT);

  // Make sure the fans are initially commanded off.
  analogWrite(FAN_PWM_PIN, 0);

  // Start LCD.
  lcd.init();
  lcd.backlight();

  // Startup message.
  lcd.setCursor(0, 0);
  lcd.print("Enclosure Fan");

  lcd.setCursor(0, 1);
  lcd.print("Controller");

  // Start Serial Monitor.
  Serial.begin(9600);
  Serial.println();
  Serial.println(
    "Arduino 3D Printer Enclosure Controller"
  );
  Serial.println(
    "System initialized."
  );

  delay(1500);

  lcd.clear();
}


// ============================================================
// Main program loop
// ============================================================

void loop() {

  unsigned long currentTime = millis();

  // Update approximately every 500 ms.
  if (
    currentTime - previousUpdateTime
    < UPDATE_INTERVAL_MS
  ) {
    return;
  }

  previousUpdateTime = currentTime;


  // 1. Measure enclosure temperature using the LM35.
  float temperatureC = readTemperatureC();


  // 2. Read the user's desired temperature from the
  //    potentiometer.
  float setpointC = readSetpointC();


  // 3. Determine the desired fan PWM command.
  int targetPWM =
    calculateTargetPWM(
      temperatureC,
      setpointC
    );


  // 4. Smooth the fan command.
  int fanPWM =
    updateFanPWM(targetPWM);


  // 5. Send PWM to the MOSFET.
  //
  // The Arduino is NOT powering the fans directly.
  // This PWM signal drives the MOSFET gate circuit,
  // which switches the fan current.
  analogWrite(
    FAN_PWM_PIN,
    fanPWM
  );


  // 6. Convert commanded PWM to percentage for display.
  //
  // This is commanded PWM percentage, NOT measured fan RPM.
  int fanPercent =
    (int)round(
      (fanPWM / 255.0) * 100.0
    );

  bool fanOn = fanPWM > 0;


  // 7. Update LCD.
  updateLCD(
    temperatureC,
    setpointC,
    fanPercent,
    fanOn
  );


  // 8. Output diagnostic information.
  printSerialDiagnostics(
    temperatureC,
    setpointC,
    targetPWM,
    fanPWM
  );
}
