# Design Decisions

This document records the main electrical and firmware choices visible in the retained project artifacts.

## 1. LM35 temperature sensing

The physical prototype used an **LM35** analog temperature sensor. The firmware assumes the LM35 relationship of approximately 10 mV/°C and converts the measured ADC voltage using:

```cpp
float tempVoltage = tempAvg * (5.0 / 1023.0);
float temperatureC = tempVoltage * 100.0;
```

The Tinkercad schematic uses a TMP36 symbol because an LM35 component was not available in that environment. The TMP36 is a schematic stand-in only and should not be used to interpret the firmware conversion.

## 2. User-adjustable temperature threshold

A **10 kΩ potentiometer** provides an analog input that is mapped to a 25–40 °C setpoint. This makes the cooling threshold adjustable without recompiling the firmware.

## 3. MOSFET interface for the fans

The Arduino generates the fan command as a PWM signal on pin D3. An **n-channel MOSFET** is used as the switching element between the Arduino control signal and the fan load.

The retained schematic also includes a 100 Ω resistor in the MOSFET gate path and a 10 kΩ resistor at the gate control node.

## 4. Analog input averaging

Both the temperature and potentiometer signals are averaged over **20 ADC readings**. The intent is to reduce short-term fluctuations before the values are used by the controller.

## 5. Threshold-based variable-speed control

The system does not use PID control. The logic is:

- at or below the selected threshold: target PWM = 0;
- above the threshold: target PWM increases with measured temperature;
- active target PWM is constrained to 80–255.

This gives the system variable fan command above the threshold while avoiding very low active PWM commands.

## 6. Smoothed PWM command

Rather than switching immediately to each newly calculated target, the firmware updates the output using:

```text
smoothedPWM = smoothedPWM + 0.1 × (targetPWM - smoothedPWM)
```

This makes commanded fan-speed changes more gradual.

## 7. Two forms of operating feedback

The project uses both:

- a **16×2 I²C LCD** for local feedback; and
- **9600-baud serial output** for diagnostic monitoring during development and testing.

The LCD presents measured temperature, selected threshold, and commanded fan percentage.
