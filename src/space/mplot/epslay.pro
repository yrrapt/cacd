%
% ISC License
% Copyright (C) 1988-2018 by
%	Nick van der Meijs
%	Arjan van Genderen
%	Simon de Graaf
% Delft University of Technology
%
% This is the common part of the eps prolog for getepslay,
% the process-specific part is still to be added.
% This is to be done with %%Include directive of getepslay.

/fillBigPaths {/bigPathFill exch def} def

  true  fillBigPaths   % Goes well if poly has no holes.
% false fillBigPaths   % Uncomment this line if poly has holes that are
		       % plotted incorrectly. The result is that the masks
		       % that contain such polygons are not filled
		       % (contour only), but the layout is otherwise
		       % plotted normally.

% Assumes three pass plotting:
% pass 1: filled mask layers
% pass 2: contour of mask layers, and other stroked paths
% pass 3: text, so it is on top of everyting.

/EPSLayDict 140 dict def EPSLayDict begin
/Style 40 dict def
0 setlinewidth

% The following has to do with pathLimit of (Level 1) Postscript.
/pathCount 0 def
/pathLimit 1000 def
/pathLimitExceeded false def

% First, some procedure definitions for defining overal appearance,
% and setting of defaults.

% [min max lambda fontname] plotFont -
/plotFont {
    aload pop
    /FontName exch def
    /LambdaFs exch def
    /MaxFs    exch def
    /MinFs    exch def
    % [(FontName) FontName (LambdaFs) LambdaFs (MaxFs) MaxFs (MinFs) MinFs] ==
    % /FontSize MinFs def % avoid use before define
} def
[8 8 1 /Helvetica-Bold] plotFont % default 8 point non-proportional

% [factor] plotPatScale -
/patternScale {aload pop /Bfreq exch def} def
[1] patternScale % default

% bool plotRotate
/plotRotate {/Rotate exch def} def
false plotRotate % default

% Defaults
/Tailwd    1 def % width of arrow tails, in points
/Tipwd     6 def % width of arrow heads, in points

/plotPass {/passnumber exch def} def
1 plotPass

% Drawing patterns.
%
/basketweave  [1 8 <d1e3c5885c3e1d88>] def
/fishscale    [1 8 <3e418080e3140808>] def

/p2 [2 16 <1010200840048002000100000000000010100820044002800100000000000000>]
def
/p4 [2 16 <18000c00040000000004000c00180030006000c0008000008000c00060003000>]
def
/p5 [2 16 <c071c00101e001e001e0e001e001e3c103c003c0000f1c0f1c0f1c000070c071>]
def
/p6 [2 16 <1000001000000200000040000008010000000000100200000000004000000001>]
def
/p7 [2 16 <002100210021ffe1e1e1e1e1e1ffe1ff210021002100e1ffe1e1e1e1ffe1ffe1>]
def
/p8 [2 16 <0000000080c0c0c160033006180c0c1806300360c1c0c08000000000cccccccc>]
def
/p9 [2 16 <00000000c0c0c0c00000000000c000c000000000cccccccc0000000000c000c0>]
def
/p13 [2 16 <00ff00ff00ff00ff00ff00ff00ff00ffff00ff00ff00ff00ff00ff00ff00ff00>]
def
/p15 [2 16 <ffff0202040408081010202040408080ffff4040202010100808040402020101>]
def
/p16 [2 16 <4040202010100808040402020101808040402020101008080404020201018080>]
def
/p17 [2 16 <c0c06060303018180c0c060603038181c0c06060303018180c0c060603038181>]
def
/p18 [2 16 <0180024004200810100820044002800180014002200410080810042002400180>]
def
/p19 [2 16 <8080808080808080808080808080808080808080808080808080808080808080>]
def

/lines0    [1 8 <ff000000ff000000>] def % lines 0 degrees inclined (horiz)
/lines45   [1 8 <0102040810204080>] def % lines 45 degrees inclined (/)
/lines90   [1 8 <1111111111111111>] def % lines 90 degrees inclined (vert)
/lines135  [1 8 <8040201008040201>] def % lines 135 degrees inclined (\)
/flines0   [1 8 <ffff000000000000>] def % fat lines 0 dg. inclined (horiz)
/flines45  [1 8 <03060C183060C081>] def % fat lines 45 dg. inclined (/)
/flines90  [1 8 <3333333333333333>] def % fat lines 90 dg. inclined (vert)
/flines135 [1 8 <81C06030180C0603>] def % fat lines 135 dg. inclined (\)
/cross0    [1 8 <ff01010101010101>] def % lines 0 and 90
/cross45   [1 8 <110a040a11a040a0>] def % lines 45 and 135
/fcross45  [1 8 <331e0c1e33e1c0e1>] def % lines 45 and 135
/black     [1 8 <ffffffffffffffff>] def % black (or dots 100%)
/dots50    [1 8 <aa55aa55aa55aa55>] def % dots 50%
/dots25    [1 8 <8822882288228822>] def % dots 25%
/dots12    [1 8 <8800220088002200>] def % dots ~12% (1 of 8)
/dots6     [1 8 <8800000088000000>] def % dots ~ 6% (1 of 16)
/dots3     [1 8 <8000000080000000>] def % dots ~ 3% (1 of 32)
/dots2     [1 8 <8000000000000000>] def % dots ~ 2% (1 of 64)
/white     [1 8 <0000000000000000>] def % white (or dots 0%)

/bitison {
    /ybit exch def /xbit exch def
    bstring ybit bwidth mul
    xbit 8 idiv add get
    1 7 xbit 8 mod sub bitshift
    and 0 ne
} def

/setpattern {
    /bstring exch def
    /onbits 0 def /offbits 0 def
    freq 0 {
      /y exch def /x exch def
      /xindex x 1 add 2 div bpside mul cvi def
      /yindex y 1 add 2 div bpside mul cvi def
      xindex yindex bitison
        {/onbits onbits 1 add def 1}
        {/offbits offbits 1 add def 0}
        ifelse
    } setscreen
    {} settransfer
    offbits offbits onbits add div setgray
} def

% Adapted from cookbook example program.
% x1 y1 x2 y2 Tailwd headwd headlen type arrow
/arrow {
    0 begin
    /ArrowType exch def
    /headlength exch def
    /halfheadthickness exch 2 div def
    /halfthickness exch 2 div def
    /tipy exch def /tipx exch def
    /taily exch def /tailx exch def
    /dx tipx tailx sub def
    /dy tipy taily sub def
    /arrowlength dx dx mul dy dy mul add sqrt def
    /angle dy dx atan def
    /base arrowlength headlength sub def

    /savematrix matrix currentmatrix def
    tailx taily translate
    angle rotate

    /Forward false def
    /Backward false def
    ArrowType 1 eq {/Forward  true def} if
    ArrowType 3 eq {/Forward  true def} if
    ArrowType 2 eq {/Backward true def} if
    ArrowType 3 eq {/Backward true def} if
    0 0 moveto

    Backward {
        headlength halfheadthickness neg lineto
        headlength halfthickness neg lineto
    } { 0 halfthickness neg lineto
    } ifelse

    Forward {
        base halfthickness neg lineto
        base halfheadthickness neg lineto
        arrowlength 0 lineto
        base halfheadthickness lineto
        base halfthickness lineto
    } { arrowlength halfthickness neg lineto
        arrowlength halfthickness lineto
    } ifelse

    Backward {
        headlength halfthickness lineto
        headlength halfheadthickness lineto
    } { 0 halfthickness lineto
    } ifelse

    closepath savematrix setmatrix end
} def
/arrow load 0 16 dict put

% X Y AdjustType String plotText -
% shows string at x y, on white background
% AdjustType has meaning as define in dali/comment.h
/plotText {
    /String exch def
    /AdjustType exch def
    /Y exch def
    /X exch def
    String stringwidth pop /W exch def
    /S Tailwd 2 mul def
    /Y Y FontSize 2 div sub def
    AdjustType 0 eq { /X X S add def} if       % left adjust
    AdjustType 1 eq { /X X W sub S sub def} if % right adjust
    AdjustType 2 eq { /X X W 2 div sub def} if % center

    % determine bbox of text.
    gsave newpath X Y moveto String true charpath pathbbox
    /st exch def /sr exch def /sb exch def /sl exch def
    % define a margin around bbox of text
    /Margin st sb sub 0.3 mul def
    /sb sb Margin sub def
    /sl sl Margin sub def
    /sr sr Margin add def
    /st st Margin add def
    % construct path corresponding to bbox
    newpath sl sb moveto sr sb lineto sr st lineto sl st lineto closepath
    % make it white.
    1 setgray fill grestore

    X Y moveto String show
} def

% x y dx dy text Text -
% Shows string with (dx,dy) relative to bounding box at (x,y)
% Dx and dy work same as in mathematica
% On white background.
/Text {
    0 begin
    /text exch def
    /dy exch def /dx exch def
    /Y exch def /X exch def
    text stringwidth pop /W exch def
    /H FontSize def

    % center text
    /Y Y FontSize 2 div sub def
    /X X W 2 div sub def

    % adjust according to dx,dy
    /X X dx W mul 2 div sub def
    /Y Y dy H mul 2 div sub def

    gsave
	% determine bbox of text.
	newpath X Y moveto text true charpath pathbbox
	/st exch def /sr exch def /sb exch def /sl exch def

	% define a margin around bbox of text
	/Margin st sb sub 0.2 mul def
	/sb sb Margin sub def
	/sl sl Margin sub def
	/sr sr Margin add def
	/st st Margin add def

	% construct path corresponding to bbox
	newpath sl sb moveto sr sb lineto sr st lineto sl st lineto
	closepath

	% make it white.
	gsave 1 setgray fill grestore

	% stroke outline
	gsave 0 setlinewidth stroke grestore

    grestore

    % print text.
    X Y moveto text show

    end
} def
/Text load 0 20 dict put

/BeginLayout {
    /EPSLayGC save def
    /TForm exch def % TForm ==

    /bbxl TForm 4 get def
    /bbxr TForm 6 get def
    /bbyb TForm 5 get def
    /bbyt TForm 7 get def
    /Resolution TForm 8 get def

    % Rotate if necessary
    Rotate {
        /xl TForm 7 get neg def
        /xr TForm 5 get neg def
        /yb TForm 4 get def
        /yt TForm 6 get def

        TForm 4 xl put
        TForm 6 xr put
        TForm 5 yb put
        TForm 7 yt put
    } if % TForm ==

    /DrawWd TForm 2 get TForm 0 get sub def
    /DrawHt TForm 3 get TForm 1 get sub def

    /PicHt TForm 7 get TForm 5 get sub def
    /PicWd TForm 6 get TForm 4 get sub def

    /WdScale DrawWd PicWd div def
    /HtScale DrawHt PicHt div def

    /ScaleFactor
        WdScale HtScale lt {WdScale} {HtScale} ifelse def

    % now move to center
    TForm 0 get DrawWd PicWd ScaleFactor mul sub 2 div add
    TForm 1 get DrawHt PicHt ScaleFactor mul sub 2 div add
    translate

    % Finally, perform scaling.  [(ScaleFactor) ScaleFactor] ==
    ScaleFactor ScaleFactor scale

    % but following items should not be scaled, so undo it.
    /Tailwd Tailwd ScaleFactor div def
    /Tipwd Tipwd ScaleFactor div def

    % Compute {min,max}fontsize and target fontsize in scaled coordinates.
    /MinFs MinFs ScaleFactor div def
    /MaxFs MaxFs ScaleFactor div def
    /FontSize LambdaFs Resolution mul def
    % [MinFs MaxFs FontSize] ==

    % Apply min and max, and set font
    FontSize MinFs lt {/FontSize MinFs def} if
    FontSize MaxFs gt {/FontSize MaxFs def} if
    FontName findfont FontSize scalefont setfont

    % set user origin
    TForm 4 get neg TForm 5 get neg translate
    Rotate {90 rotate} if
} def

/SetClipPath {
    newpath
    bbxl bbyb moveto
    bbxr bbyb lineto
    bbxr bbyt lineto
    bbxl bbyt lineto
    closepath
    clip
} def

/SetLayer {
    /Layer exch def
    gsave
    passnumber 1 eq {SetClipPath} if
    SetStyle clear newpath
    /pathLimitExceeded false def
} def

/EndLayer {
    pathCount 0 gt {DrawPolygons} if
    grestore
} def

% Start a polygon: x0 y0 P -
/P {moveto /pathCount pathCount 1 add def} def

% Continue a polygon: x y T -
/T {lineto /pathCount pathCount 1 add def} def

% Close a polygon: x y L -
/L {
    T closepath
    pathCount pathLimit gt {
	/pathLimitExceeded true def DrawPolygons
    } if
} def

% Rectangular box: x0 y0 w h B -
/B {
    /dy exch def /dx exch def /yb exch def /xl exch def
    /xr xl dx add def
    /yt yb dy add def
    xl yb P xr yb T xr yt T xl yt L
} def

/R {
    /dy exch def /dx exch def /y0 exch def /x0 exch def
    x0 y0 P dx 0 rlineto 0 dy rlineto
    /pathCount pathCount 2 add def
} def

/X {
    exch 0 rlineto 0 exch rlineto
    /pathCount pathCount 2 add def
} def

/C {
    exch 0 rlineto 0 exch rlineto
    % [(current) currentpoint (x0) x0 (y0) y0 ] ==
    x0 currentpoint pop sub 0 rlineto
    closepath
    /pathCount pathCount 4 add def
    pathCount pathLimit gt {
	/pathLimitExceeded true def DrawPolygons
    } if
} def

/EndLayout {
    EndLayer showpage
    % EPSLayDict length =
    EPSLayGC restore
    end
} def

/DrawPolygons {
    % pathLimitExceeded { [(pathLimitExceeded) pathCount Layer] == } if
    passnumber 1 eq {
	pathLimitExceeded false eq bigPathFill true eq or {
	    gsave FillPolygons grestore
	} if
    } if
    % Always stroke polygon boundaries, because a mask
    % might be skipped in pass 2.
    OutlinePolygons
    /pathCount 0 def
} def

/FillPolygons {FillPattern setpattern fill} def

/OutlinePolygons {
    /bwidth 1    def
    /bpside 8    def
    <ffffffffffffffff> setpattern
    LineWd setlinewidth
    0 setgray
    stroke
} def

/SetStyle {
    Style Layer get aload pop
    /LineWd exch Resolution mul def % in lambda's
    /Pfreq exch def
    aload pop
    /FillPattern exch def
    /bpside exch def
    /bwidth exch def
    /freq 300 bpside div Pfreq div Bfreq div def
    % [(bpside, bwidth, freq) bpside bwidth freq (FillPattern) FillPattern]==
} def

% x1 y1 x2 y2 ArrowType plotArrow
% ArrowType has meaning as defined in dali/comment.h
/plotArrow {
    /ArrowType exch def
    /y2 exch def
    /x2 exch def
    /y1 exch def
    /x1 exch def
    newpath
    x1 y1 x2 y2 Tailwd Tipwd Tipwd 2 mul ArrowType arrow
    fill
} def

/align false def
%[dx dy] termTextAlignment
/termTextAlignment {
    aload pop
    /ady exch def
    /adx exch def
    /align true def
} def

% xl xr yb yt dx dy termname plotTerminal
/plotTerminal {
    /name exch def
    /dy exch def /dx exch def
    /yt exch def /yb exch def /xr exch def /xl exch def
    passnumber 2 eq {
	LineWd setlinewidth
	newpath
	xl yb moveto xr yb lineto xr yt lineto xl yt lineto
	closepath stroke
	newpath xl yb moveto xr yt lineto stroke
	newpath xr yb moveto xl yt lineto stroke
    } if
    passnumber 3 eq {
	align true eq {/dx adx def /dy ady def} if
        xl xr add 2 div yb yt add 2 div dx dy name Text
    } if
} def

/PlotBBox {
    passnumber 2 eq {
	1 setlinewidth newpath
	bbxl bbyb moveto
	bbxr bbyb lineto
	bbxr bbyt lineto
	bbxl bbyt lineto
	closepath stroke
    } if
} def

% [ (maskname) pattern pfreq linewidth ] defineStyle -
/defineStyle {
    /stylearg exch def
    Style stylearg 0 get stylearg 1 3 getinterval put
} def
