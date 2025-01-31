.H 1 "Examples"
.H 2 "Example of 5 Parallel Conductors"
As a first example we show how 
.P= space3d
is used to compute 3D capacitances for
a configuration consisting of 5 parallel conductors.
To run the example, first create a project, e.g. with name "exam1",
for an scmos_n process and lambda of 0.05 micron:
.fS I
% mkpr -l 0.05 exam1
available processes:
process id       process name
         1       nmos
         3       scmos_n
         ...
select process id (1 - 60): 3
mkpr: -- project created --
.fE
Next, go to the project directory
and copy the example source files from the
directory /usr/cacd/share/demo/poly5 (it is supposed that the demo
directory has been installed under \fB/usr/cacd\fP).
.fS I
% cd exam1
% cp /usr/cacd/share/demo/poly5/* .
.fE
The layout description is put into the database using the program
.P= cgi.
.fS I
% cgi poly5.gds
.fE
The layout of the configuration is shown below (e.g. use
.P= dali
to inspect the layout).
The conductors have a length of 5 micron, a width of 0.5 micron,
a height of 0.5 micron
and their separation is also 0.5 micron.
.F+
.PSPIC "../space3dman/poly_5_10.eps" 2.4i
.F-
.P
.ne 3
An appropriate element definition file (with name "tech.s") is as follows:
.fS I
% cat tech.s
unit vdimension    1e-6  # micron

colors :
    cpg  red

conductors :
    resP : cpg : cpg : 0.0

vdimensions :
    dimP : cpg : cpg : 0.5 0.5

dielectrics :
    # Dielectric consists of 5 micron thick SiO2
    # (epsilon = 3.9) on a conducting plane.
    SiO2   3.9   0.0
    air    1.0   5.0
%
.fE
Furthermore, we use the following parameter file ("param.p"):
.fS I
% cat param.p
BEGIN cap3d
be_mode           0c
max_be_area       0.5
be_window         1
END cap3d
%
.fE
Then, after having run
.P= tecc
on the element definition file,
.fS I
% tecc tech.s
.fE
we extract a circuit description for the layout of the cell as follows:
.fS I
% space3d -C3 -E tech.t -P param.p poly5
.fE
Alternatively
.P= Xspace
can be used.
.fS I
% Xspace -E tech.t -P param.p
.fE
Click button "poly5" in the menu "Database",
click button "coupling cap" and "3D capacitance" in the menu "Options",
click button "DrawBEMesh", "DrawGreen" and "3 dimensional" in the menu "Display",
and click button "extract" in the menu "Extract".
This will yield the following picture:
.F+
.PSPIC "../space3dman/poly.wd.ps" 5i
.F-
.P
The circuit that has been extracted can be inspected using the program
.P= xspice
.fS I
% xspice -a poly5
poly5

* Generated by: xspice 2.39 25-Jan-2006
* Date: 20-Jun-06 10:30:48 GMT
* Path: /users/space/exam1
* Language: SPICE

* circuit poly5 e d c b a
c1 a b 253.3136e-18
c2 a GND 624.1936e-18
c3 b c 253.3136e-18
c4 b GND 457.9544e-18
c5 c d 253.3136e-18
c6 c GND 457.9544e-18
c7 e d 253.3136e-18
c8 e GND 624.1936e-18
c9 d GND 457.9544e-18
* end poly5

.fE
Note that there are no capacitances between conductors that are
more than a distance 2 * be_window apart
(e.g. conductor "a" and conductor "d" or conductor "a" and conductor "e").
In the table below,
the capacitances of conductor "a" are given as a function of the window size.
In the column denoted by $C sub {s~a}$, the short-circuit capacitance
of node "a" is given, which is the sum of all capacitances that
are connected to "a".
Note that the value of this capacitance is almost independent
on the size of the window.
.P
.DS
.TS
center, tab (:);
c:c:s:s:s:s:s
c:c:c:c:c:c:c
n1:n1:n1:n1:n1:n1:n.
_
w:capacitances ($10 sup -18 ~ roman F$)
(\(*m):$C sub {a~gnd}$:$C sub {a~b}$:$C sub {a~c}$:$C sub {a~d}$:$C sub {a~e}$:$C sub {s~a}$
_
1:  624.5 : 253.3 : - : - : -        : 877.8
2:  599.9 : 256.0 : 16.35 : 7.93 : - : 880.2
3:  593.4 : 256.8 : 16.64 : 7.14 : 4.50 : 878.5
4:  591.0 : 257.3 : 17.11 : 7.22 : 4.73 : 877.4
5:  590.5 : 257.4 : 17.18 : 7.27 : 4.79 : 877.1
_
.TE
.DE
.SK
.H 2 "Example of CMOS Static RAM Cell"
The next example consists of a cmos static RAM cell
in 0.5$mu$ technology.
To run the example, first create a project, e.g. with name "exam2",
for an scmos_n process and lambda of 0.25 micron:
.fS I
% mkpr -l 0.25 exam2
available processes:
process id       process name
         1       nmos
         3       scmos_n
         ...
select process id (1 - 60): 3
mkpr: -- project created --
.fE
Next, go to the project directory
and copy the example source files from the 
directory /usr/cacd/share/demo/sram.
.fS I
% cd exam2
% cp /usr/cacd/share/demo/sram/* .
.fE
The layout of the ram cell is put into the database as follows:
.fS I
% cgi sram.gds
.fE
Use the layout editor
.P= dali
to inspect the layout of the sram,
as shown below.
.F+
.PSPIC "../space3dman/sram.eps" 4.8i
.F-
The following technology file ("sram.s") is used for extraction:
.br
.ne 5
.gS
.in -0.8c
# space element definition file for scmos_n example process
# with vertical dimensions for conductors for 3D capacitance
# extraction.
#
# masks:
# cpg - polysilicon interconnect        ccp - contact metal to poly
# caa - active area                     cva - contact metal to metal2
# cmf - metal interconnect              cwn - n-well
# cms - metal2 interconnect             csn - n-channel implant
# cca - contact metal to diffusion      cog - contact to bondpads
#
# See also: maskdata

unit resistance    1     # ohm
unit c_resistance  1e-12 # ohm um^2
unit a_capacitance 1e-6  # aF/um^2
unit e_capacitance 1e-12 # aF/um
unit capacitance   1e-15 # fF
unit vdimension    1e-6  # um
unit shape         1e-6  # um

maxkeys 10

colors :
    cpg   red
    caa   green
    cmf   blue
    cms   gold
    cca   black
    ccp   black
    cva   black
    cwn   glass
    csn   glass
    cog   glass
    cx    glass

conductors :
  # name    : condition     : mask : resistivity : type
    cond_mf : cmf           : cmf  : 0.045       : m    # first metal
    cond_ms : cms           : cms  : 0.030       : m    # second metal
    cond_pg : cpg           : cpg  : 40          : m    # poly interconnect
    cond_pa : caa !cpg !csn : caa  : 70          : p    # p+ active area
    cond_na : caa !cpg  csn : caa  : 50          : n    # n+ active area

fets :
  # name : condition    : gate d/s
    nenh : cpg caa  csn : cpg  caa     # nenh MOS
    penh : cpg caa !csn : cpg  caa     # penh MOS

contacts :
  # name   : condition        : lay1 lay2 : resistivity
    cont_s : cva cms cmf      : cms  cmf  :   1    # metal to metal2
    cont_p : ccp cmf cpg      : cmf  cpg  : 100    # metal to poly
    cont_a : cca cmf caa !cpg : cmf  caa  : 100    # metal to active area

capacitances :

  # active area capacitances
  # name    :  condition                 : mask1 mask2 : capacitivity
    acap_na :  caa  !cpg  csn !cwn       : @gnd caa  : 100  # n+ bottom
    ecap_na : !caa !-cpg -csn !-cwn -caa : @gnd -caa : 300  # n+ sidewall

    acap_pa :  caa  !cpg  !csn cwn           :  caa @gnd : 500 # p+ bottom
    ecap_pa : !caa !-cpg !-csn cwn -cwn -caa : -caa @gnd : 600 # p+ sidewall

  # polysilicon capacitances
    acap_cpg_sub :  cpg                !caa :  cpg @gnd : 49 # bot  to sub
    ecap_cpg_sub : !cpg -cpg !cmf !cms !caa : -cpg @gnd : 52 # edge to sub

  # first metal capacitances
    acap_cmf_sub :  cmf           !cpg !caa :  cmf @gnd : 25
    ecap_cmf_sub : !cmf -cmf !cms !cpg !caa : -cmf @gnd : 52

    acap_cmf_caa :  cmf      caa !cpg !cca !cca :  cmf caa : 49
    ecap_cmf_caa : !cmf -cmf caa !cms !cpg      : -cmf caa : 59

    acap_cmf_cpg :  cmf      cpg !ccp :  cmf cpg : 49
    ecap_cmf_cpg : !cmf -cmf cpg !cms : -cmf cpg : 59

  # second metal capacitances
    acap_cms_sub :  cms      !cmf !cpg !caa :  cms @gnd : 16
    ecap_cms_sub : !cms -cms !cmf !cpg !caa : -cms @gnd : 51

    acap_cms_caa :  cms      caa !cmf !cpg :  cms caa : 25
    ecap_cms_caa : !cms -cms caa !cmf !cpg : -cms caa : 54

    acap_cms_cpg :  cms      cpg !cmf :  cms cpg : 25
    ecap_cms_cpg : !cms -cms cpg !cmf : -cms cpg : 54

    acap_cms_cmf :  cms      cmf !cva :  cms cmf : 49
    ecap_cms_cmf : !cms -cms cmf      : -cms cmf : 61

    lcap_cms     : !cms -cms =cms      : -cms =cms : 0.07

vdimensions :
    ver_caa_on_all : caa !cpg           : caa : 0.30 0.00
    ver_cpg_of_caa : cpg !caa           : cpg : 0.60 0.50
    ver_cpg_on_caa : cpg caa            : cpg : 0.35 0.70
    ver_cmf        : cmf                : cmf : 1.70 0.70
    ver_cms        : cms                : cms : 2.80 0.70
    
eshapes :
    cpg_edge : !cpg -cpg : cpg : 0 0
    cmf_edge : !cmf -cmf : cmf : 0 0
    cms_edge : !cms -cms : cms : 0 0

.in +0.8c
dielectrics :
    # Dielectric consists of 5 micron thick SiO2
    # (epsilon = 3.9) on a conducting plane.
    SiO2   3.9   0.0
    air    1.0   5.0

#EOF
.gE
Note that for the diffusion area a conductor of thickness 0 is used
that is 0.30$mu$ above the substrate.
.P
The contents of the parameter file ("sram.p") is as follows:
.gS
BEGIN cap3d
be_mode                 0c
be_window               2.0
max_be_area             1.0
omit_gate_ds_cap        on
END cap3d
.gE
After running
.P= tecc
on the element definition file,
.fS I
% tecc sram.s
.fE
extraction in batch mode is done by using
.P= space3d.
.fS I
% space3d -C3 -E sram.t -P sram.p sram
.fE
For interactive extraction,
.P= Xspace
is used.
.fS I
% Xspace -E sram.t -P sram.p
.fE
Click button "sram" in the menu "Database",
click button "coupling cap" and "3D capacitance" in the menu "Options",
click button "DrawBEMesh", "DrawGreen" and "3 dimensional" in the
menu "Display",
and click button "extract" in the menu "Extract".
This will yield the following picture:
.F+
.PSPIC "../space3dman/sram.wd.ps" 5i
.F-
The extraction result is retrieved using
.P= xspice:
.gS
% xspice -a sram
sram

* Generated by: xspice 2.39 25-Jan-2006
* Date: 20-Jun-06 10:40:34 GMT
* Path: /users/space/exam2
* Language: SPICE

* circuit sram pbulk nbulk word vdd b_vss t_vss c1 c2 bit notbit
m1 vdd c1 c2 pbulk penh_0 w=500n l=500n
m2 vdd c2 c1 pbulk penh_0 w=500n l=500n
m3 b_vss c1 c2 nbulk nenh_0 w=500n l=500n
m4 t_vss c2 c1 nbulk nenh_0 w=500n l=500n
m5 notbit word c2 nbulk nenh_0 w=500n l=500n
m6 bit word c1 nbulk nenh_0 w=500n l=500n
c1 b_vss word 114.8418e-18
c2 b_vss vdd 43.84559e-18
c3 b_vss c2 74.079e-18
c4 b_vss c1 212.2091e-18
c5 b_vss notbit 610.2115e-18
c6 b_vss GND 1.80899f
c7 notbit bit 107.3522e-18
c8 notbit word 115.7189e-18
c9 notbit vdd 6.609528e-18
c10 notbit c2 360.549e-18
c11 notbit c1 66.39728e-18
c12 notbit GND 2.597814f
c13 t_vss word 114.8457e-18
c14 t_vss vdd 43.82952e-18
c15 t_vss bit 610.3344e-18
c16 t_vss c2 212.2316e-18
c17 t_vss c1 73.78773e-18
c18 t_vss GND 1.808788f
c19 word bit 115.8338e-18
c20 word c2 113.7943e-18
c21 word c1 111.6566e-18
c22 word GND 344.449e-18
c23 vdd bit 6.60235e-18
c24 vdd c2 56.77325e-18
c25 vdd c1 58.65703e-18
c26 vdd GND 9.5875f
c27 bit c1 360.2213e-18
c28 bit c2 67.05254e-18
c29 bit GND 2.597536f
c30 c2 c1 998.1198e-18
c31 c2 GND 5.426256f
c32 c1 GND 5.425883f
* end sram

\&.model penh_0 pmos(level=2 ld=0 tox=25n nsub=50e15 vto=-1.1 uo=200 uexp=100m 
+ ucrit=10k delta=200m xj=500n vmax=50k neff=1 rsh=0 nfs=0 js=10u cj=500u 
+ cjsw=600p mj=500m mjsw=300m pb=800m cgdo=300p cgso=300p)
\&.model nenh_0 nmos(level=2 ld=0 tox=25n nsub=20e15 vto=700m uo=600 uexp=100m 
+ ucrit=10k delta=200m xj=500n vmax=50k neff=1 rsh=0 nfs=0 js=2u cj=100u 
+ cjsw=300p mj=500m mjsw=300m pb=800m cgdo=300p cgso=300p)


vpbulk pbulk 0 5
rpbulk pbulk 0 100meg

vnbulk nbulk 0 0
rnbulk nbulk 0 100meg
.gE
