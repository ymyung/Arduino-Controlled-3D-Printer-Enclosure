# Control Logic

## Overview

The enclosure uses a threshold-based variable-speed cooling strategy.

The controller has two primary inputs:

1. Enclosure temperature measured using an LM35 sensor
2. Desired temperature selected using a potentiometer

The primary output is a PWM command sent from the Arduino to the MOSFET controlling the two fans.

The controller does not measure actual fan RPM.

## Control Sequence

Each control cycle follows this sequence:

```text
Read LM35
    |
    v
Average ADC samples
    |
    v
Convert ADC value to voltage
    |
    v
Convert voltage to temperature
    |
    +----------------------+
                           |
Read potentiometer         |
    |                      |
    v                      |
Average ADC samples        |
    |                      |
    v                      |
Map reading to 25-40 C ----+
                           |
                           v
               Compare temperature
                   with setpoint
                           |
             +-------------+-------------+
             |                           |
      Temp <= Setpoint            Temp > Setpoint
             |                           |
             v                           v
       Target PWM = 0          Calculate PWM 80-255
             |                           |
             +-------------+-------------+
                           |
                           v
                     Smooth output
                           |
                           v
                    MOSFET PWM signal
                           |
                           v
                       Two fans
```

## Input Averaging

Both analog inputs are sampled 20 times.

The samples are averaged before being used by the controller.

This reduces small ADC fluctuations without requiring additional filtering hardware.

The firmware uses:

```cpp
const int NUM_SAMPLES = 20;
```

with approximately 2 ms between individual samples.

## LM35 Temperature Conversion

The Arduino Uno ADC produces values from:

```text
0 to 1023
```

using an approximately 5 V reference.

Voltage is calculated as:

```text
voltage = ADC × (5.0 / 1023)
```

The physical prototype used an LM35 sensor.

The LM35 produces approximately:

```text
10 mV / °C
```

Therefore:

```text
temperature (°C) = voltage / 0.010
```

The Tinkercad schematic uses a TMP36 symbol only because the original simulation environment did not provide the LM35 component.

The firmware uses the LM35 conversion relationship.

## Setpoint Calculation

The potentiometer is connected to a second analog input.

Its averaged ADC reading is mapped to:

```text
25 °C to 40 °C
```

The controller uses floating-point mapping rather than Arduino's standard integer `map()` function so fractional averaged ADC values and temperature values can be preserved.

## Cooling Threshold

When:

```text
measured temperature <= desired temperature
```

the target fan PWM is:

```text
0
```

When:

```text
measured temperature > desired temperature
```

active cooling begins.

## Variable Fan Command

Once active cooling is required, the target PWM command begins at:

```text
80 / 255
```

and increases as measured temperature rises.

The controller reaches maximum commanded PWM when the measured temperature is approximately:

```text
10 °C above the selected setpoint
```

Therefore the operating relationship is approximately:

```text
T <= Tset
PWM = 0
```

```text
Tset < T < Tset + 10 °C
PWM = proportional value between 80 and 255
```

```text
T >= Tset + 10 °C
PWM = 255
```

The value 80 was used as the minimum active PWM command because very small PWM duty cycles may not reliably start a DC fan.

No fan-start threshold was experimentally characterized and retained, so this value should not be interpreted as a measured minimum operating requirement.

## PWM Smoothing

The controller applies exponential smoothing to PWM changes:

```text
smoothedPWM =
smoothedPWM
+ (targetPWM - smoothedPWM) × 0.10
```

A floating-point accumulator is used to prevent fractional updates from being lost through integer truncation.

When cooling first becomes active, the output is raised to at least the minimum active PWM level.

When cooling is no longer required, the command decreases toward zero.

Once it falls below the minimum active fan command, the controller switches the output fully off.

The purpose of smoothing is to reduce abrupt changes in commanded fan output.

## MOSFET Output Stage

Arduino PWM pin 3 provides the control signal to the N-channel MOSFET circuit.

The Arduino does not directly supply the fan operating current.

Instead:

```text
Arduino PWM
    |
    v
MOSFET gate
    |
    v
MOSFET switches fan current
    |
    v
Two DC fans
```

## Fan Percentage

The displayed fan percentage is calculated from the PWM command:

```text
fan percentage =
PWM / 255 × 100
```

For example:

```text
PWM = 128
```

corresponds to approximately:

```text
50% commanded PWM
```

This does not mean the fan is rotating at 50% of its maximum RPM.

Actual fan speed was not measured.

## Controller Classification

The system should be described as:

> Threshold-based variable-speed fan control with PWM output smoothing.

It should not be described as:

- PID control
- Closed-loop fan-speed control
- Closed-loop RPM control

The temperature measurement influences fan command, but there is no feedback measurement of actual fan speed.

## Update Rate

The main control logic runs approximately every:

```text
500 ms
```

Additional time is required to acquire the averaged analog samples.

This update rate was sufficient for the relatively slow thermal behavior of the enclosure prototype.
