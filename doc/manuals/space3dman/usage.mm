.H 1 "Program Usage"
.sY %SE_USAGE% \n(H1
.H 2 "General"
3D capacitance extraction can be performed using one of the
following versions of the \fISpace Software\fP:
.P= space3d
(for batch mode extraction)
and
.P= Xspace
(for interactive extraction and mesh visualization).
Both these tools can also be used from within the graphical user
interface
.P= helios.
.P
Normally, when performing a 3D capacitance extraction,
a flat extraction will be executed.
This implication can be disabled when turning on
the parameter
.I allow_hierarchical_cap3d.
.H 2 "Batch Mode Extraction"
In order to use the 3D capacitance extraction mode of
.P= space3d,
use the option
.O= "-3" "-3dcap" "" .
Also, use either the option 
.O= -c -cap
or the option
.O= -C -coupcap "" .
In both cases, 3D ground and coupling capacitances are computed.
However, only in the second case all these capacitances will be part
of the output circuit.
In the first case,
all coupling capacitances will be reconnected to ground.
.H 2 "Interactive Extraction"
For 3D capacitance extraction it may be helpful to use the
.P= Xspace
visualization program.
This program is not more an interactive version of the
.P= space3d
program.
But it runs
.P= space3d
in batch mode for each new extraction.
It is using a "display.out" file to show the results.
The
.P= Xspace
program runs under X Windows and 
uses a graphical window to,
among other things,
show the 3D mesh that is generated by the program.
Interactively, the user can select
the cell that is extracted, the options that are used,
and the items that are displayed.
However, for a complete graphical interface to all 
extraction tools, it is better to use the graphical
user interface
.P= helios
that includes
.P= space3d
as well as
.P= Xspace.
.P
For 3D capacitance extraction using
.P= Xspace, 
turn on "3D capacitance" and either "coupling cap"
or "capacitance"
in the menu "Options".
To display also the 3D mesh,
click on "DrawBEMesh" and "3 dimensional" (and possibly "DrawGreen")
in the menu "Display".
Then, after selecting the name of the cell in 
the menu "Database",
the extraction can be started by clicking on "extract" in the
menu "Extract".
.P
To preview the mesh for 3D capacitance computation, use
.P= Xspace
as described above and
also turn on "BE mesh only".
