#
# space element definition file for scmos_n example process
# with transistor bulk connections and substrate terminals
# for substrate contacts and nmos bulk connections, and
# with information for 3D capacitance extraction and
# substrate resistance extraction.
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

colors :
    cpg   red
    caa   @00cc00
    cmf   @0000cc
    cms   gold
    cca   black
    ccp   black
    cva   black
    cwn   @660066
    csn   glass
    cog   glass
    @sub  pink

conductors :
  # name    : condition     : mask : resistivity : type
    cond_mf : cmf           : cmf  : 0.045       : m    # first metal
    cond_ms : cms           : cms  : 0.030       : m    # second metal
    cond_pg : cpg           : cpg  : 40          : m    # poly interconnect
    cond_pa : caa !cpg !csn : caa  : 70          : p    # p+ active area
    cond_na : caa !cpg  csn : caa  : 50          : n    # n+ active area
    cond_well : cwn         : cwn  : 0           : n    # n well

fets :
  # name : condition    : gate d/s : bulk
    nenh : cpg caa  csn : cpg  caa : @sub  # nenh MOS
    penh : cpg caa !csn : cpg  caa : cwn   # penh MOS

contacts :
  # name   : condition         : lay1 lay2 : resistivity
    cont_s : cva cmf cms       : cmf  cms  :   1   # metal to metal2
    cont_p : ccp cmf cpg       : cmf  cpg  : 100   # metal to poly
    cont_a : cca cmf caa !cpg cwn !csn
           | cca cmf caa !cpg !cwn csn
                               : cmf  caa  : 100   # metal to active area

    cont_w : cca cmf cwn csn   : cmf  cwn  :  80   # metal to well
    cont_b : cca cmf !cwn !csn : cmf  @sub :  80   # metal to subs

junction capacitances ndif :
  # name    :  condition                 : mask1 mask2 : capacitivity
    acap_na :  (caa csn)     !cpg  !cwn  : @gnd  caa   : 100  # n+ bottom
    ecap_na : !(caa csn) (-caa -csn) !-cpg !-cwn : @gnd -caa : 300  # n+ sidewall

junction capacitances nwell :
    acap_cw :  cwn                  : @gnd  cwn : 100  # bottom
    ecap_cw : !cwn -cwn             : @gnd -cwn : 800  # sidewall

junction capacitances pdif :
  # name    :  condition                 : mask1 mask2 : capacitivity
    acap_pa :  (caa !csn)    !cpg   cwn  :  caa  cwn   : 500 # p+ bottom
    ecap_pa : !(caa !csn) (-caa !-csn) !-cpg cwn -cwn : -caa cwn : 600 # p+ sidewall

capacitances :
  # polysilicon capacitances
    acap_cpg_sub :  cpg                !caa !cwn :  cpg @gnd : 49
    acap_cpg_cwn :  cpg                !caa  cwn :  cpg cwn  : 49
    ecap_cpg_sub : !cpg -cpg !cmf !cms !caa !cwn : -cpg @gnd : 52
    ecap_cpg_cwn : !cpg -cpg !cmf !cms !caa  cwn : -cpg cwn  : 52

  # first metal capacitances
    acap_cmf_sub :  cmf           !cpg !caa !cwn :  cmf @gnd : 25
    acap_cmf_cwn :  cmf           !cpg !caa  cwn :  cmf cwn  : 25
    ecap_cmf_sub : !cmf -cmf !cms !cpg !caa !cwn : -cmf @gnd : 52
    ecap_cmf_cwn : !cmf -cmf !cms !cpg !caa  cwn : -cmf cwn  : 52

    acap_cmf_caa :  cmf      caa !cpg !cca :  cmf  caa : 49
    ecap_cmf_caa : !cmf -cmf caa !cms !cpg : -cmf  caa : 59

    acap_cmf_cpg :  cmf      cpg !ccp :  cmf  cpg : 49
    ecap_cmf_cpg : !cmf -cmf cpg !cms : -cmf  cpg : 59

  # second metal capacitances
    acap_cms_sub :  cms      !cmf !cpg !caa !cwn :  cms @gnd : 16
    acap_cms_cwn :  cms      !cmf !cpg !caa  cwn :  cms cwn  : 16
    ecap_cms_sub : !cms -cms !cmf !cpg !caa !cwn : -cms @gnd : 51
    ecap_cms_cwn : !cms -cms !cmf !cpg !caa  cwn : -cms cwn  : 51

    acap_cms_caa :  cms      caa !cmf !cpg :  cms caa : 25
    ecap_cms_caa : !cms -cms caa !cmf !cpg : -cms caa : 54

    acap_cms_cpg :  cms      cpg !cmf :  cms cpg : 25
    ecap_cms_cpg : !cms -cms cpg !cmf : -cms cpg : 54

    acap_cms_cmf :  cms      cmf !cva :  cms cmf : 49
    ecap_cms_cmf : !cms -cms cmf      : -cms cmf : 61

    lcap_cms     : !cms -cms =cms    : -cms =cms : 0.07

vdimensions :
    v_caa_on_all : caa !cpg           : caa : 0.30 0.00
    v_cpg_of_caa : cpg !caa           : cpg : 0.60 0.50
    v_cpg_on_caa : cpg caa            : cpg : 0.35 0.70
    v_cmf        : cmf                : cmf : 1.70 0.70
    v_cms        : cms                : cms : 2.80 0.70

dielectrics :
   # Dielectric consists of 5 micron thick SiO2
   # (epsilon = 3.9) on a conducting plane.
   SiO2   3.9   0.0
   air    1.0   5.0

sublayers :
  # name       conductivity  top
    substrate  6.7           0.0

selfsubres :
#   Generated by subresgen on 10:56:15 13-5-2003
#     area    perim          r   rest
      0.64      3.2    81286.8   0.01  # w=0.8 l=0.8
      0.64        4   73678.39   0.01  # w=0.4 l=1.6
      0.48      3.2   88205.12   0.01  # w=0.4 l=1.2
      2.56      6.4    40643.4   0.01  # w=1.6 l=1.6
      2.56        8    36839.2   0.01  # w=0.8 l=3.2
      1.92      6.4   44102.56   0.01  # w=0.8 l=2.4
     10.24     12.8    20321.7   0.01  # w=3.2 l=3.2
     10.24       16    18419.9   0.01  # w=1.6 l=6.4
      7.68     12.8   22051.22   0.01  # w=1.6 l=4.8
     40.96     25.6   10160.85   0.01  # w=6.4 l=6.4
     40.96       32   9209.799   0.01  # w=3.2 l=12.8
     30.72     25.6   11025.61   0.01  # w=3.2 l=9.6
    163.84     51.2   5080.426   0.01  # w=12.8 l=12.8
    163.84       64   4604.902   0.01  # w=6.4 l=25.6
    122.88     51.2   5512.804   0.01  # w=6.4 l=19.2
    655.36    102.4   2540.212   0.01  # w=25.6 l=25.6
    655.36      128   2302.451   0.01  # w=12.8 l=51.2
    491.52    102.4   2756.403   0.01  # w=12.8 l=38.4
   2621.44    204.8   1270.106   0.01  # w=51.2 l=51.2
   2621.44      256   1151.225   0.01  # w=25.6 l=102.4
   1966.08    204.8   1378.201   0.01  # w=25.6 l=76.8

coupsubres :
#   Generated by subresgen on 10:56:15 13-5-2003
#    area1      area2     dist          r    decr
      0.64       0.64      1.6     648598  0.873512  # w=0.8 d=1.6
      0.64       0.64      3.2    1101504  0.925946  # w=0.8 d=3.2
      0.64       0.64      6.4    1996617  0.959256  # w=0.8 d=6.4
      0.64       0.64     25.6    7341756  0.988935  # w=0.8 d=25.6
      2.56       2.56      3.2   324299.1  0.873515  # w=1.6 d=3.2
      2.56       2.56      6.4   550752.1  0.925953  # w=1.6 d=6.4
      2.56       2.56     12.8   998307.9  0.959253  # w=1.6 d=12.8
      2.56       2.56     51.2    3670877  0.988967  # w=1.6 d=51.2
     10.24      10.24      6.4   162149.6  0.873515  # w=3.2 d=6.4
     10.24      10.24     12.8     275376  0.925950  # w=3.2 d=12.8
     10.24      10.24     25.6   499154.2  0.959259  # w=3.2 d=25.6
     10.24      10.24    102.4    1835439  0.988967  # w=3.2 d=102.4
    655.36     655.36     51.2   20268.69  0.873515  # w=25.6 d=51.2
    655.36     655.36    102.4      34422  0.925953  # w=25.6 d=102.4
    655.36     655.36    204.8   62394.28  0.959257  # w=25.6 d=204.8
    655.36     655.36    819.2   229429.8  0.988985  # w=25.6 d=819.2

#EOF
