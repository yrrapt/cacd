*One Bit Comparator. Takes Two Inputs (A and B), and returns Two Ouputs -
*node 8 - (high when two signalsare equal) and node 9 (high when A is Larger Then B).
* Transient Analysis

*circuit description

M1 Anot A Vdd Vdd  PMOS w=3.6u l=1.2u
M2 Anot A 0 0 NMOS w=1.8u l=1.2u
M3 Bnot B Vdd Vdd  PMOS w=3.6u l=1.2u
M4 Bnot B 0 0 NMOS w=1.8u l=1.2u
M5 AorBnot 0 Vdd Vdd PMOS w=1.8u l=3.6u
M6 AorBnot B 1 0 NMOS w=1.8u l=1.2u
M7 1 Anot 0 0 NMOS w=1.8u l=1.2u
M8 Lnot 0 Vdd Vdd PMOS w=1.8u l=3.6u
M9 Lnot Bnot 2 0 NMOS w=1.8u l=1.2u
M10 2 A 0 0 NMOS w=1.8u l=1.2u
M11 Qnot 0 Vdd Vdd PMOS w=3.6u l=3.6u
M12 Qnot AorBnot 3 0 NMOS w=1.8u l=1.2u
M13 3 Lnot 0 0 NMOS w=1.8u l=1.2u
MQLO 8 Qnot Vdd Vdd  PMOS w=3.6u l=1.2u
MQL1 8 Qnot 0 0 NMOS w=1.8u l=1.2u
MLTO 9 Lnot Vdd Vdd  PMOS w=3.6u l=1.2u
MLT1 9 Lnot 0 0 NMOS w=1.8u l=1.2u
CQ Qnot 0 30f
CL Lnot 0 10f

Vdd Vdd 0 5
Va A 0  pulse 0 5 10ns .1ns .1ns 15ns 30ns
Vb B 0 0

.model nmos nmos level=8
.model pmos pmos level=8

* transient analysis
.tran 1ns 60ns
.option post
.print tran a b v(9) v(8)

.END

