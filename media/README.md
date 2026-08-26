# Arduino-Controlled 3D Printer Enclosure

An Arduino-based temperature-control system developed for a 3D printer enclosure. The system measures enclosure temperature using an LM35 sensor and automatically controls two ventilation fans through a MOSFET using PWM.

A potentiometer allows the user to select a desired temperature between approximately 25 °C and 40 °C, while a 16×2 I²C LCD provides real-time temperature, setpoint, and fan-command information.

## Physical Prototype

![Arduino-Controlled 3D Printer Enclosure](https://drive.google.com/file/d/1QVS3vhauEuZH-5TzZrHPzLvr3M1PnJcH/view?usp=sharing)

## Demo

**[Watch the working prototype demonstration →](https://drive.google.com/file/d/1cJ_dpvMLTJp6Dsobzo3_qu-2Yc8-_ou2/view?usp=sharing)**

## My Contribution

I served as the electrical lead for the project and worked on the sensing, control, wiring, firmware, and system testing.

My work included:

- Integrating the Arduino Uno, temperature sensor, potentiometer, MOSFET fan-control circuit, and LCD
- Developing the Arduino firmware used to read the sensors and control the fans
- Implementing PWM-based variable fan control
- Adding analog-input averaging to reduce measurement fluctuations
- Implementing serial diagnostics and LCD feedback
- Troubleshooting the electrical and control behavior during system integration
- Documenting the system architecture, hardware, control logic, and limitations

## System Architecture

The system follows this control path:

```text
LM35 Temperature Sensor
        |
        v
Arduino Uno ADC
        |
        +----------------------+
        |                      |
        v                      v
Temperature Measurement    Control Logic
                               |
Potentiometer -----------------+
                               |
                               v
                         PWM Command
                               |
                               v
                          MOSFET Driver
                               |
                               v
                           Two DC Fans

Arduino Uno
    |
    +---- I2C ----> 16x2 LCD
    |
    +---- USB ----> Serial Diagnostics
```

## Hardware

The prototype uses:

- Arduino Uno R3
- LM35 analog temperature sensor
- 10 kΩ potentiometer
- Two DC cooling fans
- N-channel MOSFET
- 100 Ω resistor
- 10 kΩ MOSFET gate pull-down resistor
- Two protection diodes
- PCF8574-based 16×2 I²C LCD
- Breadboard and jumper wiring

See [`hardware/BOM.md`](hardware/BOM.md) for additional hardware documentation.

## Temperature Measurement

The physical prototype uses an **LM35 temperature sensor**.

The Arduino reads the LM35 output using its 10-bit analog-to-digital converter.

The firmware converts the ADC result to voltage:

```text
voltage = ADC reading × (5.0 / 1023)
```

The LM35 produces approximately 10 mV per degree Celsius, so:

```text
temperature (°C) = voltage / 0.010
```

Twenty ADC samples are averaged for each temperature measurement to reduce small reading fluctuations.

### Tinkercad Sensor Note

The original Tinkercad environment used for documenting the circuit did not provide the LM35 component.

A TMP36 symbol therefore appears in the Tinkercad schematic as a **visual substitute only**.

The physical prototype used an LM35, and the firmware uses the LM35 conversion relationship.

## Adjustable Temperature Setpoint

A 10 kΩ potentiometer is connected to an Arduino analog input.

The potentiometer position is mapped to a desired enclosure temperature between:

```text
25 °C and 40 °C
```

This allows the user to adjust the temperature at which active ventilation begins.

## Fan Control

The controller uses threshold-based variable-speed fan control.

At or below the selected temperature:

```text
Fan PWM = 0
```

When the measured temperature rises above the selected temperature, the controller begins active cooling at a minimum PWM command of:

```text
80 / 255
```

The commanded PWM then increases proportionally as temperature rises.

At approximately 10 °C above the selected temperature:

```text
Fan PWM = 255 / 255
```

The output is constrained between 0 and 255.

A floating-point smoothing calculation reduces abrupt changes in the PWM command.

The system is **not a PID controller**.

It also does **not measure actual fan RPM**. Any fan percentage displayed by the system represents the commanded PWM duty cycle rather than measured mechanical fan speed.

## MOSFET Fan Driver

The Arduino PWM output controls the gate of an N-channel MOSFET.

The MOSFET allows the Arduino to control the fans without requiring the Arduino I/O pin to directly supply their operating current.

The driver circuit includes:

- MOSFET switching
- Gate resistance
- Gate pull-down resistance
- Protection diodes

This separates the low-current Arduino control signal from the higher-current fan load.

## LCD Interface

A PCF8574-based 16×2 LCD communicates with the Arduino over I²C.

The display provides:

- Measured enclosure temperature
- Selected temperature setpoint
- Commanded fan percentage

The LCD used for this project was configured at I²C address:

```text
0x27
```

## Serial Diagnostics

The Arduino also sends diagnostic information over the serial connection.

Example output:

```text
Temperature: 31.2 C | Setpoint: 29.5 C | Target PWM: 109 | Output PWM: 96 | Fan command: 38%
```

This was useful during development and troubleshooting because sensor measurements and controller outputs could be observed directly.

## Firmware Structure

The firmware separates the system into several functions:

```text
readAverageADC()
readTemperatureC()
readSetpointC()
calculateTargetPWM()
updateFanPWM()
updateLCD()
printSerialDiagnostics()
```

This makes the sensing, control, output, and user-interface logic easier to understand independently.

The complete firmware is available in:

[`firmware/fan_controller.ino`](firmware/fan_controller.ino)

## Repository Structure

```text
Arduino-Controlled-3D-Printer-Enclosure/
|
|-- README.md
|
|-- firmware/
|   `-- fan_controller.ino
|
|-- hardware/
|   |-- BOM.md
|   |-- README.md
|   |-- tinkercad_schematic.png
|   `-- tinkercad_component_list.png
|
|-- docs/
|   |-- control_logic.md
|   |-- design_decisions.md
|   `-- testing_and_limitations.md
|
`-- media/
    |-- prototype_overview.jpg
    `-- README.md
```

## Engineering Limitations

This project was developed as a student prototype rather than a production temperature-control system.

Important limitations include:

- No closed-loop fan-speed measurement
- No fan tachometer or RPM feedback
- No retained quantitative fan-performance data
- No retained calibrated thermal-response dataset
- No PID temperature controller
- Breadboard-based electronics rather than a custom PCB
- No production-grade enclosure or electrical protection design
- The Tinkercad schematic uses a TMP36 symbol in place of the physical LM35 sensor

The project documentation intentionally distinguishes implemented functionality from improvements that could be made in a future revision.

See [`docs/testing_and_limitations.md`](docs/testing_and_limitations.md) for further detail.

## Potential Future Improvements

A more advanced revision could include:

- Fan tachometer feedback
- Closed-loop fan-speed control
- Temperature calibration against a reference instrument
- Quantitative thermal-response testing
- Multiple temperature sensors
- Fault detection for sensor or fan failure
- A custom PCB
- Data logging
- Improved electrical protection
- Formal enclosure airflow testing

## What This Project Demonstrates

This project provided practical experience with:

- Arduino firmware development
- Analog sensor acquisition
- PWM control
- MOSFET switching
- Basic electronics
- I²C communication
- User-adjustable control inputs
- Signal averaging
- Hardware/software integration
- Electrical troubleshooting
- Technical documentation

The project also reinforced the importance of distinguishing commanded control outputs from measured physical system performance.
