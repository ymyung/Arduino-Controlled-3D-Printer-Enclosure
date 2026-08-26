# Testing, Evidence, and Limitations

## Retained evidence

The project currently has the following retained documentation:

- a video showing the physical circuit operating;
- the Arduino firmware;
- a Tinkercad electrical schematic;
- a Tinkercad component list / BOM.

The demo video should be placed in the repository under `media/` when available.

## What was tested

During the original project, the system was operated while monitoring sensor readings and temperature response to support troubleshooting and verify the fan-control behavior. The retained working-circuit video provides qualitative evidence that the integrated hardware and firmware operated together.

No quantitative calibration dataset, response-time dataset, or repeatability dataset has been retained, so this repository does **not** claim numerical temperature-control accuracy, fan-speed accuracy, or closed-loop performance.

## Important documentation note: LM35 vs TMP36

The physical build used an **LM35** temperature sensor. The Tinkercad environment used for the retained schematic did not provide an LM35 component, so a **TMP36 component was used only as a schematic stand-in**.

The firmware uses the LM35 conversion relationship:

```cpp
float temperatureC = tempVoltage * 100.0;
```

A TMP36 requires a different conversion because it includes an output-voltage offset. Therefore, the Tinkercad sensor symbol should not be interpreted as the sensor model used in the physical prototype.

## Current limitations

- The controller estimates **commanded PWM percentage**, not actual fan RPM.
- The system does not include tachometer feedback from the fans.
- No retained reference-thermometer calibration data is available.
- Temperature conversion assumes an approximately 5 V Arduino ADC reference.
- The control strategy is threshold-based variable-speed control rather than PID or model-based control.
- The electrical schematic is a retrospective Tinkercad representation and uses a TMP36 symbol in place of the physical LM35.

## Possible future improvements

These are proposed improvements, not features of the original prototype:

- calibrate the LM35 against a reference thermometer;
- record temperature-versus-time data during step tests;
- add fan tachometer feedback to measure actual RPM;
- compare the current threshold-based controller with proportional or PID control;
- log data automatically to a computer for analysis;
- replace breadboard wiring with a purpose-built PCB or more permanent wiring harness;
- document measured fan current and verify component ratings under worst-case operating conditions.
