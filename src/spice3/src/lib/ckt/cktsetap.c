/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/

#include "spice.h"
#include <stdio.h>
#include "tskdefs.h"
#include "jobdefs.h"
#include "ifsim.h"
#include "util.h"
#include "iferrmsg.h"
#include "cktdefs.h"

extern SPICEanalysis *analInfo[];

int CKTsetAnalPm(GENERIC *ckt, GENERIC *analPtr, int parm, IFvalue *value, IFvalue *selector)
{
    int type = ((JOB *)analPtr)->JOBtype;
    if ((analInfo[type]->setParm) == NULL) return(E_BADPARM);
    return((*(analInfo[type]->setParm))(ckt, analPtr, parm, value));
}
