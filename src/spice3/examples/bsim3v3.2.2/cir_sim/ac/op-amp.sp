* Operational Amplifier.  AC Analysis.
* BSIM3 testing, 3/16/96.

*Operational Amplifier
M1 bias1 1 cm cm nmos w=10u l=1u
M2 bias2 in2 cm cm  nmos w=10u l=1u
M3 vdd bias1 bias1 vdd pmos w=2u l=1u
M4 bias2 bias1 vdd vdd pmos w=2u l=1u

m5 cm bias vss vss nmos w=2u l=1u
mbias bias bias vss vss nmos w=2u l=1u
rbias 0 bias 195k

m6 8 bias vss vss  nmos w=2u l=1u
m7 8 bias2 vdd out nmos w=2u l=1u

Cfb bias2 8 2p

Vid 1 c 0 ac 0.1
eid in2 c 1 c -1
vic c 0 dc 0
vss vss 0 -5
Vdd vdd 0 5

*AC analysis
.ac dec 10 100 100Meg
.print ac vdb(8)

.model nmos nmos level=8
.model pmos pmos level=8

.end

