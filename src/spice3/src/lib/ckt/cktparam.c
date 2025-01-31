/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/

/* CKTparam
 *  attach the given parameter to the specified device in the given circuit
 */

#include "spice.h"
#include <stdio.h>
#include "cktdefs.h"
#include "ifsim.h"
#include "util.h"
#include "devdefs.h"
#include "sperror.h"

extern SPICEdev *DEVices[];

int CKTparam(GENERIC *ckt, GENERIC *fast, int param, IFvalue *val, IFvalue *selector)
{
    int type;
    GENinstance *myfast = (GENinstance *)fast;
    type = myfast->GENmodPtr->GENmodType;
    if (DEVices[type]->DEVparam) {
	return((*(DEVices[type]->DEVparam)) (param, val, myfast, selector));
    }
    return(E_BADPARM);
}
