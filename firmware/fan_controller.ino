#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup (change to 0x3F if your screen doesn’t respond)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pins
const int tempPin = A0;   // LM35 sensor
const int potPin = A1;    // Potentiometer for desired temp
const int fanPWM = 3;     // MOSFET gate (PWM output)

// Settings
const int numSamples = 20;  // ADC averaging
int smoothedPWM = 0;

void setup() {
  pinMode(fanPWM, OUTPUT);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  Serial.println("Dynamic Fan Control System Initialized");
}

void loop() {
  // === Average LM35 Temperature ===
  long tempSum = 0;
  for (int i = 0; i < numSamples; i++) {
    tempSum += analogRead(tempPin);
    delay(2);
  }
  float tempAvg = tempSum / (float)numSamples;
  float tempVoltage = tempAvg * (5.0 / 1023.0);
  float temperatureC = tempVoltage * 100.0;

  // === Average Potentiometer Reading ===
  long potSum = 0;
  for (int i = 0; i < numSamples; i++) {
    potSum += analogRead(potPin);
    delay(2);
  }
  float potAvg = potSum / (float)numSamples;
  float potVoltage = potAvg * (5.0 / 1023.0);

  // === Desired Temperature (from potentiometer) ===
  float fanOnThreshold = map(potAvg, 0, 1023, 25, 40);

  // === Fan Control Logic ===
  int targetPWM = 0;
  if (temperatureC > fanOnThreshold) {
    targetPWM = map(temperatureC, fanOnThreshold, fanOnThreshold + 10, 80, 255);
    targetPWM = constrain(targetPWM, 80, 255);
  } else {
    targetPWM = 0;
  }

  // === Smooth Fan Speed ===
  smoothedPWM = smoothedPWM + (targetPWM - smoothedPWM) * 0.1;
  analogWrite(fanPWM, smoothedPWM);
  int fanPercent = map(smoothedPWM, 0, 255, 0, 100);
  bool fanOn = fanPercent > 0;

  // === LCD Display ===
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temperatureC, 1);
  lcd.print("C ");
  lcd.setCursor(0, 1);
  lcd.print("Des:");
  lcd.print(fanOnThreshold, 1);
  lcd.print("C ");
  lcd.print(fanPercent);
  lcd.print("%");
  lcd.setCursor(15, 1);
  lcd.print(fanOn ? ">" : " ");  // shows ">" when fan is ON

  // === Serial Monitor Output ===
  Serial.print("Temp: ");
  Serial.print(temperatureC, 1);
  Serial.print(" C | Target: ");
  Serial.print(fanOnThreshold, 1);
  Serial.print(" C | Fan: ");
  Serial.print(fanPercent);
  Serial.print("% ");
  Serial.println(fanOn ? "| ON" : "| OFF");

  delay(500);
}
