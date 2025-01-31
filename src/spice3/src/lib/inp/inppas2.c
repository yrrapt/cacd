/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/

#include "spice.h"
#include "ifsim.h"
#include "cpdefs.h"
#include "fteext.h"
#include "ftedefs.h"
#include "inpdefs.h"
#include "util.h"
#include "iferrmsg.h"
#include "tskdefs.h"
#include "inpmacs.h"

/* pass 2 - Scan through the lines.  ".model" cards have processed
 *  in pass1 and are ignored here.
 */

void INPpas2(GENERIC *ckt, card *data, INPtables *tab, GENERIC *task)
{
    card *current;
    char *gname;
    GENERIC *gnode;
    int c, error;

    gname = copy("0");
    error = INPgndInsert(ckt, &gname, tab, &gnode);
    if (error && error != E_EXISTS)
	data->error = INPerrCat(data->error, INPmkTemp("can't insert internal ground node in symbol table!\n"));

    for (current = data; current; current = current->nextcard) {

        c = *current->line;
        if (islower(c)) c = toupper(c);

	switch (c) {
        case ' ':
        case '\t':  /* blank line (space/tab leading) */
            break;
        case 'R':   /* Rname <node> <node> [<val>][<mname>][w=<val>][l=<val>] */
            INP2R(ckt, tab, current);
            break;
        case 'C':   /* Cname <node> <node> <val> [IC=<val>] */
            INP2C(ckt, tab, current);
            break;
        case 'L':   /* Lname <node> <node> <val> [IC=<val>] */
            INP2L(ckt, tab, current);
            break;
        case 'G':   /* Gname <node> <node> <node> <node> <val> */
            INP2G(ckt, tab, current);
            break;
        case 'E':   /* Ename <node> <node> <node> <node> <val> */
            INP2E(ckt, tab, current);
            break;
        case 'F':   /* Fname <node> <node> <vname> <val> */
            INP2F(ckt, tab, current);
            break;
        case 'H':   /* Hname <node> <node> <vname> <val> */
            INP2H(ckt, tab, current);
            break;
        case 'D':   /* Dname <node> <node> <model> [<val>] [OFF] [IC=<val>] */
            INP2D(ckt, tab, current);
            break;
        case 'J':   /* Jname <node> <node> <node> <model> [<val>] [OFF] [IC=<val>,<val>] */
            INP2J(ckt, tab, current);
            break;
        case 'Z':   /* Zname <node> <node> <node> <model> [<val>] [OFF] [IC=<val>,<val>] */
            INP2Z(ckt, tab, current);
            break;
        case 'M':   /* Mname <node> <node> <node> <node> <model> [L=<val>]
                     *       [W=<val>] [AD=<val>] [AS=<val>] [PD=<val>]
                     *       [PS=<val>] [NRD=<val>] [NRS=<val>] [OFF] [IC=<val>,<val>,<val>] */
            INP2M(ckt, tab, current);
            break;
	case 'O':   /* Oname <node> <node> <node> <node> <model> [IC=<val>,<val>,<val>,<val>] */
	    INP2O(ckt, tab, current);
	    break;
        case 'V':   /* Vname <node> <node> [[DC] <val>] [AC [<val> [<val>]]] [<tran function>] */
            INP2V(ckt, tab, current);
            break;
        case 'I':   /* Iname <node> <node> [[DC] <val>] [AC [<val> [<val>]]] [<tran function>] */
            INP2I(ckt, tab, current);
            break;
        case 'Q':   /* Qname <node> <node> <node> [<node>] <model> [<val>] [OFF] [IC=<val>,<val>] */
            INP2Q(ckt, tab, current, gnode);
            break;
        case 'T':   /* Tname <node> <node> <node> <node> [TD=<val>]
                     *       [F=<val> [NL=<val>]][IC=<val>,<val>,<val>,<val>] */
            INP2T(ckt, tab, current);
            break;
        case 'S':   /* Sname <node> <node> <node> <node> [<modname>] [IC] */
            INP2S(ckt, tab, current);
            break;
        case 'W':   /* Wname <node> <node> <vctrl> [<modname>] [IC] */
            /* CURRENT CONTROLLED SWITCH */
            INP2W(ckt, tab, current);
            break;
        case 'U':   /* Uname <node> <node> <model> [l=<val>] [n=<val>] */
            INP2U(ckt, tab, current);
            break;
        case 'K':   /* Kname Lname Lname <val> */
            INP2K(ckt, tab, current);
            break;
        case '*':   /* *<anything> - a comment - ignore */
            break;
        case 'B':   /* Bname <node> <node> [V=expr] [I=expr] */
            /* Arbitrary source. */
            INP2B(ckt, tab, current);
            break;
        case '.':   /* .<something> Many possibilities */
            if (INP2dot(ckt, tab, current, task, gnode)) return;
            break;
	case 0:
	    break;
        default:
            /* the un-implemented device */
            LITERR("unknown device type - error\n")
            break;
        }
    }
}
