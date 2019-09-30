/*
 * ISC License
 *
 * Copyright (C) 1984-2018 by
 *	Pieter van der Wolf
 *	Simon de Graaf
 * Delft University of Technology
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "src/ocean/seadali/header.h"

extern qtree_t **quad_root;
extern INST *inst_root;
extern TERM **term_root;
extern Coor xlim, xrim, ybim, ytim; /* bounding box of instances and boxes without the image */
extern Coor xlbb, xrbb, ybbb, ytbb; /* bounding box boxes + terminals */
extern Coor xlmb, xrmb, ybmb, ytmb; /* bounding box instances */
extern Coor xltb, xrtb, ybtb, yttb; /* total bounding box */
extern int  NR_lay;
extern int  ImageMode;
extern char ImageInstName[];

static void b_box (qtree_t *Q);
static int  bounding_box (Coor *xl, Coor *yb, Coor *xr, Coor *yt);

void upd_boundb ()
{
    register int lay;
    int validflag, inst_flag, im_flag;
    register INST *ip;
    register TERM *tp;
    Coor ll, rr, bb, tt;

    xlim = xrim = ybim = ytim = 0;
    xlmb = xrmb = ybmb = ytmb = 0;

    inst_flag = im_flag = 0;
    for (ip = inst_root; ip; ip = ip -> next) {
#ifdef PR_BBOX
	fprintf (stderr, "inst: %s: %6ld, %6ld, %6ld, %6ld\n", ip->inst_name,
	    (long) ip->bbxl, (long) ip->bbxr, (long) ip->bbyb, (long) ip->bbyt);
#endif
	inst_window (ip, &ll, &rr, &bb, &tt);
	if (!inst_flag) { inst_flag = 1;
	    xlmb = ll; xrmb = rr; ybmb = bb; ytmb = tt;
	}
	else {
	    if (ll < xlmb) xlmb = ll;
	    if (rr > xrmb) xrmb = rr;
	    if (bb < ybmb) ybmb = bb;
	    if (tt > ytmb) ytmb = tt;
	}

	/* Patrick */
	if (ImageMode && strcmp (ip->inst_name, ImageInstName) == 0) {
#ifdef PR_BBOX
	    fprintf (stderr, "skip the image: %s\n", ip->inst_name);
#endif
	    continue; /* skip the image! */
	}
	else {
	    if (!im_flag) { im_flag = 1;
		xlim = ll; xrim = rr; ybim = bb; ytim = tt;
	    }
	    else {
		if (ll < xlim) xlim = ll;
		if (rr > xrim) xrim = rr;
		if (bb < ybim) ybim = bb;
		if (tt > ytim) ytim = tt;
	    }
	}
	/* end Patrick */
    }

    validflag = bounding_box (&xlbb, &ybbb, &xrbb, &ytbb);

    for (lay = 0; lay < NR_lay; ++lay) {
	for (tp = term_root[lay]; tp; tp = tp -> nxttm) {
	    if (validflag) {
		term_win (tp, &ll, &rr, &bb, &tt);
		if (ll < xlbb) xlbb = ll;
		if (rr > xrbb) xrbb = rr;
		if (bb < ybbb) ybbb = bb;
		if (tt > ytbb) ytbb = tt;
	    }
	    else {
		term_win (tp, &xlbb, &xrbb, &ybbb, &ytbb);
		validflag = 1;
	    }
	}
    }
#ifdef PR_BBOX
    fprintf (stderr, "im-bbox: %6ld, %6ld, %6ld, %6ld\n", (long) xlim, (long) xrim, (long) ybim, (long) ytim);
    fprintf (stderr, "mc-bbox: %6ld, %6ld, %6ld, %6ld\n", (long) xlmb, (long) xrmb, (long) ybmb, (long) ytmb);
    fprintf (stderr, "el-bbox: %6ld, %6ld, %6ld, %6ld\n", (long) xlbb, (long) xrbb, (long) ybbb, (long) ytbb);
#endif

    if (validflag) {
	if (im_flag) {
	    if (xlbb < xlim) xlim = xlbb;
	    if (xrbb > xrim) xrim = xrbb;
	    if (ybbb < ybim) ybim = ybbb;
	    if (ytbb > ytim) ytim = ytbb;
	}
	else {
	    xlim = xlbb; xrim = xrbb; ybim = ybbb; ytim = ytbb;
	}
    }
#ifdef PR_BBOX
    fprintf (stderr, "im-bbox: %6ld, %6ld, %6ld, %6ld\n", (long) xlim, (long) xrim, (long) ybim, (long) ytim);
#endif

    if (comment_win (&ll, &rr, &bb, &tt)) {
	if (validflag) {
	    if (ll < xlbb) xlbb = ll;
	    if (rr > xrbb) xrbb = rr;
	    if (bb < ybbb) ybbb = bb;
	    if (tt > ytbb) ytbb = tt;
	}
	else {
	    validflag = 1;
	    xlbb = ll; xrbb = rr; ybbb = bb; ytbb = tt;
	}
    }

    if (inst_flag && !validflag) {
	xltb = xlmb;
	xrtb = xrmb;
	ybtb = ybmb;
	yttb = ytmb;
    }
    else if (!inst_flag && validflag) {
	xltb = xlbb;
	xrtb = xrbb;
	ybtb = ybbb;
	yttb = ytbb;
    }
    else {
	xltb = Min (xlbb, xlmb);
	xrtb = Max (xrbb, xrmb);
	ybtb = Min (ybbb, ybmb);
	yttb = Max (ytbb, ytmb);
    }
#ifdef PR_BBOX
    fprintf (stderr, "to-bbox: %6ld, %6ld, %6ld, %6ld\n", (long) xltb, (long) xrtb, (long) ybtb, (long) yttb);
#endif
}

static Coor bb_x1, bb_y1, bb_x2, bb_y2;
static int  firsttime;

/*
** Find bounding box of the primitives of the parent cell.
** INPUT: the addresses of where the bounding box must be returned.
*/
static int bounding_box (Coor *xl, Coor *yb, Coor *xr, Coor *yt)
{
    register int lay;

    bb_x1 = bb_y1 = bb_x2 = bb_y2 = 0;

    firsttime = 1;

    /* find bounding box of quad tree */
    for (lay = 0; lay < NR_lay; ++lay) b_box (quad_root[lay]);

    while (bb_x1 % QUAD_LAMBDA) --bb_x1;
    while (bb_y1 % QUAD_LAMBDA) --bb_y1;
    while (bb_x2 % QUAD_LAMBDA) ++bb_x2;
    while (bb_y2 % QUAD_LAMBDA) ++bb_y2;

    *xl = bb_x1;
    *yb = bb_y1;
    *xr = bb_x2;
    *yt = bb_y2;

    return (firsttime ? 0 : 1);
}

/*
** Find the bounding box of the quad tree Q.
** INPUT: a pointer to a quad tree node.
*/
static void b_box (qtree_t *Q)
{
    register struct obj_node *p;
    register struct ref_node *r;

    if (!Q) return;

    if (!firsttime) {
	if (Q -> quadrant[0] > bb_x1 && Q -> quadrant[1] > bb_y1 &&
	    Q -> quadrant[2] < bb_x2 && Q -> quadrant[3] < bb_y2) return;
    }

    /* scan the object list */
    for (p = Q -> object; p; p = p -> next) { /* update bounding box */
	if (firsttime) {
	    firsttime = 0;
	    bb_x1 = p -> ll_x1;
	    bb_x2 = p -> ll_x2;
	    bb_y1 = p -> ll_y1;
	    bb_y2 = p -> ll_y2;
	}
	else {
	    if (p -> ll_x1 < bb_x1) bb_x1 = p -> ll_x1;
	    if (p -> ll_x2 > bb_x2) bb_x2 = p -> ll_x2;
	    if (p -> ll_y1 < bb_y1) bb_y1 = p -> ll_y1;
	    if (p -> ll_y2 > bb_y2) bb_y2 = p -> ll_y2;
	}
    }

    /* scan the reference list */
    for (r = Q -> reference; r; r = r -> next) {
        p = r -> ref;
	if (firsttime) {
	    firsttime = 0;
	    bb_x1 = p -> ll_x1;
	    bb_x2 = p -> ll_x2;
	    bb_y1 = p -> ll_y1;
	    bb_y2 = p -> ll_y2;
	}
	else {
	    if (p -> ll_x1 < bb_x1) bb_x1 = p -> ll_x1;
	    if (p -> ll_x2 > bb_x2) bb_x2 = p -> ll_x2;
	    if (p -> ll_y1 < bb_y1) bb_y1 = p -> ll_y1;
	    if (p -> ll_y2 > bb_y2) bb_y2 = p -> ll_y2;
	}
    }

    /* search quad nodes recursively */
    if (Q -> Uleft ) b_box (Q -> Uleft);
    if (Q -> Uright) b_box (Q -> Uright);
    if (Q -> Lright) b_box (Q -> Lright);
    if (Q -> Lleft ) b_box (Q -> Lleft);
}
