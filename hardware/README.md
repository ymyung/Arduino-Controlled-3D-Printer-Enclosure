# Hardware Documentation

This folder contains the retained electrical documentation and the native SolidWorks package for the 3D-printer enclosure controller.

- [`BOM.md`](BOM.md) - electrical bill of materials
- [`tinkercad_schematic.png`](tinkercad_schematic.png) - reconstructed Tinkercad circuit schematic
- [`tinkercad_component_list.png`](tinkercad_component_list.png) - component-list screenshot
- `part files/` - native enclosure parts, assembly, drawings, Arduino packaging model, and source screenshots

## Native CAD Safety

The files in `part files/` intentionally retain their current names and locations because SolidWorks assemblies and drawings can contain external references. Do not rename, move, or replace the native `.SLDPRT`, `.SLDASM`, or `.SLDDRW` files solely for repository organization.

The included `arduino uno.SLDPRT` is used as a packaging reference. Its original source and license are not documented here, so this project does not claim authorship of that model.

## Sensor Note

The physical prototype used an **LM35 analog temperature sensor**. Tinkercad did not provide an LM35 component, so the schematic uses a **TMP36 symbol only as a visual substitute**. The firmware is written for the LM35 transfer characteristic.
