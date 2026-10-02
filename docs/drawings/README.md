# Engineering Drawing PDFs

This directory contains one-sheet PDF exports of the native SolidWorks engineering drawings retained in `hardware/part files/`. The native files remain the governing CAD documents and must not be renamed or moved because their references may depend on the existing paths.

The PDFs were opened with the Windows PDF renderer and visually checked for readable sheet output and the contents summarized below.

| Native drawing | PDF export | Verified sheet content |
| --- | --- | --- |
| `ACE-001_Arduino_Control_Enclosure_Base.SLDDRW` | [ACE-001 Base](ACE-001_Arduino_Control_Enclosure_Base.pdf) | Dimensioned base views, section A-A, and isometric view |
| `ACE-002_Sliding_Lid.SLDDRW` | [ACE-002 Sliding Lid](ACE-002_Arduino_Control_Enclosure_Sliding_Lid.pdf) | Dimensioned lid views and isometric view |
| `ACE-003_Arduino_Control_Enclosure_Assembly.SLDDRW` | [ACE-003 Assembly](ACE-003_Arduino_Control_Enclosure_Assembly.pdf) | Closed, open, and exploded assembly views; three-item BOM; item balloons |

When a native drawing changes, regenerate and revalidate its PDF without saving documentation-only changes back into the SolidWorks file.
