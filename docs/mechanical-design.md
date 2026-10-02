# Mechanical Design Notes

## Scope and Project History

The electronics/control enclosure is an independent 2026 extension of the original January-April 2025 group thermal-control project. It packages the Arduino/control hardware and provides the required external connections; it does not relocate the temperature sensor or cooling fans from the main printer enclosure.

The values below are the approximate dimensions recorded for this design. The native SolidWorks parts and drawings remain the governing design files.

## Enclosure Base

The control-enclosure base is approximately 120 mm x 85 mm x 35 mm overall, with approximately 2.5 mm walls and an R3 exterior corner treatment. Its functional features include:

- mounting bosses/standoffs for an Arduino Uno R3-style controller;
- access for the board's USB-B and power connections;
- an opening for external sensor/fan cable routing; and
- rail/groove geometry that guides the removable lid.

The base is represented by `hardware/part files/Control_Enclosure_Base.SLDPRT` and drawing ACE-001.

## Sliding Lid

The lid is approximately 116 mm x 80.5 mm x 1.6 mm. It slides within the enclosure guide geometry and includes a front pull/finger feature. Rear geometry limits lid travel/closure. The design does not use screws, and no snap-fit behaviour is claimed.

The lid is represented by `hardware/part files/Enclosure_lid.SLDPRT` and drawing ACE-002.

## Assembly Model

The assembly combines the enclosure base, sliding lid, and an Arduino Uno R3-style packaging model. It includes OPEN and CLOSED configurations with configuration-specific mates controlling lid position, plus an exploded representation.

The Arduino model supports placement and access studies. Its original source and license are not documented in this repository, so the project does not claim authorship of that model.

The assembly is represented by `hardware/part files/enclosure_assembly.SLDASM` and drawing ACE-003.

## Electromechanical Interfaces

The packaging work addresses the interfaces visible in the retained CAD:

- controller support and placement inside the base;
- external access to USB-B and power connections;
- routing for wiring to the printer-enclosure temperature sensor and fan load; and
- access to the electronics through the removable lid.

The temperature sensor and cooling fans remain part of the main printer-enclosure system rather than components inside this control box.

## Native CAD and Drawing Package

The native directory intentionally retains the original filenames because SolidWorks parts, assemblies, and drawings can depend on path and filename references. The directory contains:

| File | Purpose |
| --- | --- |
| `Control_Enclosure_Base.SLDPRT` | Enclosure base part |
| `Enclosure_lid.SLDPRT` | Sliding-lid part |
| `enclosure_assembly.SLDASM` | Control-enclosure assembly |
| `arduino uno.SLDPRT` | Arduino packaging model; authorship/source not established here |
| `ACE-001_Arduino_Control_Enclosure_Base.SLDDRW` | Base drawing |
| `ACE-002_Sliding_Lid.SLDDRW` | Lid drawing |
| `ACE-003_Arduino_Control_Enclosure_Assembly.SLDDRW` | Assembly drawing |

## Current Validation Boundary

The repository does not contain evidence that the mechanical enclosure was fabricated or physically fit-checked. It also does not establish production tolerances, material specifications, surface finish, tolerance-stack analysis, interference validation, DFM validation, FEA, CFD, thermal simulation, or airflow improvement. Those items are outside the documented scope until supported by future build and validation work.
