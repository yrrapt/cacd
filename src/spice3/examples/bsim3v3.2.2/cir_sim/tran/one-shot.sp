*One-shot Trigger.
*This cicruit generates a pulse of a predetermined width
*triggered by an input event. BSIM3 test.

*A two-gate delay element
Md1 4 in Vdd Vdd  PMOS w=3.6u l=1.2u
Md2 4 in 0 0 NMOS w=1.8u l=1.2u
c4 4 0 30f
Md3 A 4 Vdd Vdd  PMOS w=3.6u l=1.2u
Md4 A 4 0 0 NMOS w=1.8u l=1.2u
ca a 0 30f

*EXOR gate
*A inverter
M1 Anot A Vdd Vdd  PMOS w=3.6u l=1.2u
M2 Anot A 0 0 NMOS w=1.8u l=1.2u

M3 Bnot in Vdd Vdd  PMOS w=3.6u l=1.2u
M4 Bnot in 0 0 NMOS w=1.8u l=1.2u

M5 AorBnot 0 Vdd Vdd PMOS w=1.8u l=3.6u
M6 AorBnot in 1 0 NMOS w=1.8u l=1.2u
M7 1 Anot 0 0 NMOS w=1.8u l=1.2u

M8 Lnot 0 Vdd Vdd PMOS w=1.8u l=3.6u
M9 Lnot Bnot 2 0 NMOS w=1.8u l=1.2u
M10 2 A 0 0 NMOS w=1.8u l=1.2u

M11 out 0 Vdd Vdd PMOS w=3.6u l=3.6u
M12 out AorBnot 3 0 NMOS w=1.8u l=1.2u
M13 3 Lnot 0 0 NMOS w=1.8u l=1.2u
*end of EXOR gate

Vcc vdd 0 5
vin in 0 pulse 0 5 1ns .1ns .1ns .8ns 5ns

.model nmos nmos level=8
.model pmos pmos level=8

.tran 1ns 10ns
.print tran in out
.option post

.END

