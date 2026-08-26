# Control Logic

## Inputs

The controller uses two analog inputs:

- **A0 — LM35 temperature sensor:** provides the measured enclosure temperature.
- **A1 — 10 kΩ potentiometer:** sets the user's desired temperature threshold.

Both analog inputs are averaged over **20 ADC samples** before being used by the control logic.

## Temperature conversion

The firmware assumes a 5 V Arduino ADC reference and converts the averaged LM35 reading using:

```text
ADC reading → voltage = reading × 5.0 / 1023
Temperature (°C) = voltage × 100
```

This matches the LM35-style 10 mV/°C relationship used in the physical prototype.

## Adjustable setpoint

The potentiometer reading is mapped to a desired temperature range of **25–40 °C**.

```text
Potentiometer ADC value 0 … 1023
             ↓
Desired temperature 25 … 40 °C
```

## Fan command

When the measured temperature is at or below the setpoint, the target PWM command is zero.

When the measured temperature exceeds the setpoint, the firmware maps temperature over the next 10 °C to a PWM command between **80 and 255**, then constrains the result to that range.

```text
Measured temperature <= setpoint
        → target PWM = 0

Measured temperature > setpoint
        → target PWM increases with temperature
        → constrained to 80 … 255
```

This is a threshold-based variable-speed control strategy. It is **not PID control** and it does not measure actual fan RPM.

## Output smoothing

The commanded PWM value is smoothed each cycle using:

```text
smoothedPWM = smoothedPWM + 0.1 × (targetPWM - smoothedPWM)
```

This causes the PWM command to move gradually toward the current target rather than changing immediately in one step.

## Outputs

The controller produces three user-visible/system outputs:

1. **PWM output on pin D3** to the MOSFET gate, which controls the two fan loads.
2. **16×2 I²C LCD** showing measured temperature, desired temperature, and commanded fan percentage.
3. **Serial monitor output at 9600 baud** for testing and diagnostics.
