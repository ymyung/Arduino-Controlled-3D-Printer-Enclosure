# Hardware Documentation

This folder contains the retained electrical documentation for the 3D printer enclosure controller.

- [`BOM.md`](BOM.md) — bill of materials
- [`tinkercad_schematic.png`](tinkercad_schematic.png) — reconstructed Tinkercad circuit schematic
- [`tinkercad_component_list.png`](tinkercad_component_list.png) — component-list screenshot

## Sensor note

The **physical prototype used an LM35** temperature sensor. Tinkercad did not provide an LM35 component, so the schematic uses a **TMP36 symbol only as a visual substitute**. The firmware is written for the LM35 transfer characteristic.
