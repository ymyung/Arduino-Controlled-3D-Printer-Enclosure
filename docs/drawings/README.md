# Engineering Drawing PDFs

PDF exports are pending. The native SolidWorks drawings remain in `hardware/part files/` and must not be renamed or moved because their references may depend on the existing paths.

Export the drawings with the installed SolidWorks application using these exact source and destination names:

| Native drawing | PDF output |
| --- | --- |
| `ACE-001_Arduino_Control_Enclosure_Base.SLDDRW` | `ACE-001_Arduino_Control_Enclosure_Base.pdf` |
| `ACE-002_Sliding_Lid.SLDDRW` | `ACE-002_Arduino_Control_Enclosure_Sliding_Lid.pdf` |
| `ACE-003_Arduino_Control_Enclosure_Assembly.SLDDRW` | `ACE-003_Arduino_Control_Enclosure_Assembly.pdf` |

Place the three exported PDFs in this directory. Open each drawing read-only where practical, export all drawing sheets, and do not save documentation-only changes back into the native file. Before linking the PDFs from the main README, verify page count, sheet proportions, major views, annotations, and any BOM/balloons actually present in the source drawing.
