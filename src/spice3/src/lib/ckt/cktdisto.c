/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1988 Jaijeet S Roychowdhury
**********/

#include "spice.h"
#include "cktdefs.h"
#include "devdefs.h"
#include "vsrc/vsrcdefs.h"
#include "isrc/isrcdefs.h"
#include "fteconst.h"
#include "iferrmsg.h"
#include "distodef.h"
#include "sperror.h"
#include <stdio.h>
#include "util.h"

int CKTdisto (register CKTcircuit *ckt, int mode)
{
    extern SPICEdev *DEVices[];
    register DISTOAN *cv = (DISTOAN*) (ckt->CKTcurJob);
    register int i;
    int error = 0; /* init, prevent compiler warning */
    int size;

    switch (mode) {

    case D_SETUP:

	for (i = 0; i < DEVmaxnum; i++) {
	    if (DEVices[i]->DEVdisto && ckt->CKThead[i]) {
		error = (*(DEVices[i]->DEVdisto))(mode, ckt->CKThead[i], ckt);
		if (error) return(error);
	    }
	}
	break;

    case D_TWOF1:
    case D_THRF1:
    case D_F1PF2:
    case D_F1MF2:
    case D_2F1MF2:

	size = SMPmatSize(ckt->CKTmatrix);
	for (i = 1; i <= size; i++) {
	    ckt->CKTrhs[i] = 0;
	    ckt->CKTirhs[i] = 0;
	}

	for (i = 0; i < DEVmaxnum; i++) {
	    if (DEVices[i]->DEVdisto && ckt->CKThead[i]) {
		error = (*(DEVices[i]->DEVdisto))(mode, ckt->CKThead[i], ckt);
		if (error) return(error);
	    }
	}
	break;

    case D_RHSF1:

	cv->Df2given = 0; /* will change if any F2 source is found */

    case D_RHSF2:
    {
	double mag, phase;
	int vcode, icode;

	size = SMPmatSize(ckt->CKTmatrix);
	for (i = 0; i <= size; i++) {
	    ckt->CKTrhs[i] = 0;
	    ckt->CKTirhs[i] = 0;
	}

	vcode = CKTtypelook("Vsource");
	icode = CKTtypelook("Isource");

        if (vcode >= 0) {
            /* voltage sources are in this version, so use them */
            register VSRCinstance *here;
            register VSRCmodel *model;
            for (model = (VSRCmodel *)ckt->CKThead[vcode]; model; model = model->VSRCnextModel) {
                for (here = model->VSRCinstances; here; here = here->VSRCnextInstance) {

/* check if the source has a distortion input*/

if (here->VSRCdGiven) {
    if (here->VSRCdF2given) cv->Df2given = 1;
    if ((here->VSRCdF1given && mode == D_RHSF1) ||
	(here->VSRCdF2given && mode == D_RHSF2)) {

	if (mode == D_RHSF1) {
	     mag   = here->VSRCdF1mag;
	     phase = here->VSRCdF1phase;
	}
	else {
	     mag   = here->VSRCdF2mag;
	     phase = here->VSRCdF2phase;
	}
	ckt->CKTrhs [here->VSRCbranch] = 0.5 * mag * cos(M_PI*phase/180.0);
	ckt->CKTirhs[here->VSRCbranch] = 0.5 * mag * sin(M_PI*phase/180.0);
    }
}
                }
            }
        }
        if (icode >= 0) {
            /* current sources are in this version, so use them */
            register ISRCinstance *here;
            register ISRCmodel *model;

            for (model= (ISRCmodel *)ckt->CKThead[icode]; model; model = model->ISRCnextModel) {
                for (here = model->ISRCinstances; here; here = here->ISRCnextInstance) {

/* check if the source has a distortion input*/

if (here->ISRCdGiven) {
    if (here->ISRCdF2given) cv->Df2given = 1;
    if ((here->ISRCdF1given && mode == D_RHSF1) ||
	(here->ISRCdF2given && mode == D_RHSF2)) {

	if (mode == D_RHSF1) {
	     mag   = here->ISRCdF1mag;
	     phase = here->ISRCdF1phase;
	} else {
	     mag   = here->ISRCdF2mag;
	     phase = here->ISRCdF2phase;
	}
	ckt->CKTrhs [here->ISRCposNode] = -0.5 * mag * cos(M_PI*phase/180.0);
	ckt->CKTrhs [here->ISRCnegNode] =  0.5 * mag * cos(M_PI*phase/180.0);
	ckt->CKTirhs[here->ISRCposNode] = -0.5 * mag * sin(M_PI*phase/180.0);
	ckt->CKTirhs[here->ISRCnegNode] =  0.5 * mag * sin(M_PI*phase/180.0);
    }
}
                }
            }
        }
    }
	error = 0;
	break;

    default:
	error = E_BADPARM;
	break;
    }

    return(error);
}
