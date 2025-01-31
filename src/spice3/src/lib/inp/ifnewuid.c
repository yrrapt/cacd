/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1988 Thomas L. Quarles
**********/

#include "spice.h"
#include <stdio.h>
#include "inpdefs.h"
#include "util.h"
#include "ifsim.h"
#include "iferrmsg.h"
#include "cpdefs.h"
#include "fteext.h"

int IFnewUid(GENERIC *ckt, IFuid *newuid, IFuid olduid, char *suffix, int type, GENERIC **nodedata)
{
    char *newname;
    int error;

    if (olduid) {
        newname = MALLOC(strlen(suffix) + strlen((char*)olduid) + 2);
        sprintf(newname, "%s#%s", (char*)olduid, suffix);
    } else {
        newname = MALLOC(strlen(suffix) + 1);
        sprintf(newname, "%s", suffix);
    }

    switch (type) {
        case UID_ANALYSIS:
        case UID_TASK:
        case UID_INSTANCE:
        case UID_OTHER:
        case UID_MODEL:
            error = INPinsert(&newname, (INPtables *)ft_curckt->ci_symtab);
            if (error && error != E_EXISTS) return(error);
            *newuid = (IFuid) newname;
            break;

        case UID_SIGNAL:
            error = INPmkTerm(ckt, &newname, (INPtables *)ft_curckt->ci_symtab, nodedata);
            if (error && error != E_EXISTS) return(error);
            *newuid = (IFuid) newname;
	    break;

        default:
            return(E_BADPARM);
    }
    return(OK);
}

int IFdelUid(GENERIC *ckt, IFuid uid, int type)
{
    int error;

    switch (type) {
        case UID_ANALYSIS:
        case UID_TASK:
        case UID_INSTANCE:
        case UID_OTHER:
        case UID_MODEL:
            error = INPremove(uid, (INPtables *)ft_curckt->ci_symtab);
            if (error && error != E_EXISTS) return(error);
            break;

        case UID_SIGNAL:
            error = INPremTerm(uid, (INPtables *)ft_curckt->ci_symtab);
            if (error && error != E_EXISTS) return(error);
	    break;

        default:
            return(E_BADPARM);
    }
    return(OK);
}
