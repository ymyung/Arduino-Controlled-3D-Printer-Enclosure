# Bill of Materials

The following list is based on the retained Tinkercad component list and the known physical build.

| Component | Quantity | Notes |
|---|---:|---|
| Arduino Uno R3 | 1 | Main controller |
| LM35 temperature sensor | 1 | Used in the physical prototype |
| 10 kΩ potentiometer | 1 | User-adjustable temperature setpoint |
| 16×2 I²C LCD (PCF8574-based, address 0x27 in firmware) | 1 | Displays measured temperature, setpoint, and commanded fan output |
| DC fan / DC motor | 2 | Cooling loads controlled together |
| n-channel MOSFET | 1 | Switches the fan load from the Arduino PWM signal |
| 100 Ω resistor | 1 | Shown in the MOSFET gate path in the schematic |
| 10 kΩ resistor | 1 | Shown as the MOSFET gate pull-down in the schematic |
| Diode | 2 | Included with the two motor branches in the retained schematic |
| Small breadboard | 1 | Prototype wiring |

## Tinkercad sensor substitution

The physical prototype used an **LM35** temperature sensor. Tinkercad did not provide an LM35 component, so the retained Tinkercad schematic uses a **TMP36 symbol as a visual stand-in**. The firmware is written for the LM35 transfer characteristic, not the TMP36 transfer characteristic.

See [`tinkercad_component_list.png`](tinkercad_component_list.png) for the original component-list screenshot.
