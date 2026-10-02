# Testing, Evidence, and Limitations

## Retained Evidence

The original electrical-control project has the following retained evidence:

- a [video of the physical circuit operating](../media/enclosure_demo.MOV.MOV);
- a [physical prototype photo](../media/enclosure_photo.HEIC);
- the Arduino firmware;
- a Tinkercad electrical schematic; and
- a Tinkercad component list / BOM.

The later independent mechanical extension adds native SolidWorks parts, an assembly, five CAD screenshots, and three native SolidWorks drawings. These mechanical artifacts are design documentation, not evidence of fabrication or physical fit validation.

## What Was Tested

During the original project, the system was operated while monitoring sensor readings and temperature response to support troubleshooting and verify integrated fan-control behaviour. The retained working-circuit video provides qualitative evidence that the hardware and firmware operated together.

No quantitative calibration dataset, response-time dataset, or repeatability dataset has been retained, so this repository does **not** claim numerical temperature-control accuracy, fan-speed accuracy, or closed-loop performance.

## Important Documentation Note: LM35 vs TMP36

The physical build and retained firmware documentation identify an **LM35** analog temperature sensor. The Tinkercad environment used for the retained schematic did not provide an LM35 component, so a **TMP36 component was used only as a schematic stand-in**.

The firmware uses the LM35 conversion relationship:

```cpp
float temperatureC = tempVoltage * 100.0;
```

A TMP36 requires a different conversion because it includes an output-voltage offset. The Tinkercad sensor symbol therefore should not be interpreted as the sensor model used in the physical prototype.

## Current Limitations

- The controller estimates commanded PWM percentage, not actual fan RPM.
- The system does not include tachometer feedback from the fans.
- No retained reference-thermometer calibration data is available.
- Temperature conversion assumes an approximately 5 V Arduino ADC reference.
- The control strategy is threshold-based variable-speed control rather than PID or model-based control.
- The electrical schematic is a retrospective Tinkercad representation and uses a TMP36 symbol in place of the documented LM35.
- The mechanical control enclosure has not been documented as fabricated or physically fit-checked.

## Possible Future Improvements

These are proposed improvements, not completed features:

- calibrate the LM35 against a reference thermometer;
- record temperature-versus-time data during controlled tests;
- add fan tachometer feedback to measure actual RPM;
- compare the current threshold-based controller with proportional or PID control;
- log data automatically to a computer for analysis;
- replace breadboard wiring with a purpose-built PCB or more permanent wiring harness;
- document measured fan current and verify component ratings under worst-case operating conditions;
- fabricate the control enclosure and verify board, connector, cable, and lid fit; and
- revise the CAD/drawings based on measured physical results.
