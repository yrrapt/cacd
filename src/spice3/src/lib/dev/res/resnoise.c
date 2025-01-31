/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1987 Gary W. Ng
**********/

#include "spice.h"
#include <stdio.h>
#include "resdefs.h"
#include "cktdefs.h"
#include "fteconst.h"
#include "iferrmsg.h"
#include "noisedef.h"
#include "util.h"

/*
 * RESnoise (mode, operation, firstModel, ckt, data, OnDens)
 *    This routine names and evaluates all of the noise sources
 *    associated with resistors.  It starts with the model *firstModel
 *    and traverses all of its instances.  It then proceeds to any other
 *    models on the linked list.  The total output noise density
 *    generated by all the resistors is summed in the variable "OnDens".
 */

extern void   NevalSrc();
extern double Nintegrate();

int
RESnoise (mode, operation, genmodel, ckt, data, OnDens)
    int mode;
    int operation;
    GENmodel *genmodel;
    CKTcircuit *ckt;
    register Ndata *data;
    double *OnDens;
{
    RESmodel *firstModel = (RESmodel *) genmodel;
    register RESmodel *model;
    register RESinstance *inst;
    char name[N_MXVLNTH];
    double tempOutNoise;
    double tempInNoise;
    double noizDens;
    double lnNdens;


    for (model=firstModel; model != NULL; model=model->RESnextModel) {
	for (inst=model->RESinstances; inst != NULL; inst=inst->RESnextInstance) {
	    switch (operation) {

	    case N_OPEN:

		/* see if we have to to produce a summary report */
		/* if so, name the noise generator */

		if (((NOISEAN*)ckt->CKTcurJob)->NStpsSm != 0) {
		    switch (mode) {

		    case N_DENS:
			(void)sprintf(name,"onoise_%s",(char*)inst->RESname);


data->namelist = (IFuid *)trealloc((char *)data->namelist,(data->numPlots + 1)*sizeof(IFuid));
if (!data->namelist) return(E_NOMEM);
		(*(SPfrontEnd->IFnewUid))(ckt,
			&(data->namelist[data->numPlots++]),
			(IFuid)NULL,name,UID_OTHER,(GENERIC **)NULL);
				/* we've added one more plot */


			break;

		    case INT_NOIZ:
			(void)sprintf(name,"onoise_total_%s",(char*)inst->RESname);


data->namelist = (IFuid *)trealloc((char *)data->namelist,(data->numPlots + 1)*sizeof(IFuid));
if (!data->namelist) return(E_NOMEM);
		(*(SPfrontEnd->IFnewUid))(ckt,
			&(data->namelist[data->numPlots++]),
			(IFuid)NULL,name,UID_OTHER,(GENERIC **)NULL);
				/* we've added one more plot */


			(void)sprintf(name,"inoise_total_%s",(char*)inst->RESname);


data->namelist = (IFuid *)trealloc((char *)data->namelist,(data->numPlots + 1)*sizeof(IFuid));
if (!data->namelist) return(E_NOMEM);
		(*(SPfrontEnd->IFnewUid))(ckt,
			&(data->namelist[data->numPlots++]),
			(IFuid)NULL,name,UID_OTHER,(GENERIC **)NULL);
				/* we've added one more plot */


			break;
		    }
		}
		break;

	    case N_CALC:
		switch (mode) {

		case N_DENS:
		    NevalSrc(&noizDens,&lnNdens,ckt,THERMNOISE,
				 inst->RESposNode,inst->RESnegNode,inst->RESconduct);

		    *OnDens += noizDens;

		    if (data->delFreq == 0.0) {

			/* if we haven't done any previous integration, we need to */
			/* initialize our "history" variables                      */

			inst->RESnVar[LNLSTDENS] = lnNdens;

			/* clear out our integration variable if it's the first pass */

			if (data->freq == ((NOISEAN*)ckt->CKTcurJob)->NstartFreq) {
			    inst->RESnVar[OUTNOIZ] = 0.0;
			}
		    } else {   /* data->delFreq != 0.0 (we have to integrate) */
			tempOutNoise = Nintegrate(noizDens, lnNdens,
			       inst->RESnVar[LNLSTDENS], data);
			tempInNoise = Nintegrate(noizDens *
			       data->GainSqInv ,lnNdens + data->lnGainInv,
			       inst->RESnVar[LNLSTDENS] + data->lnGainInv,
			       data);
			inst->RESnVar[OUTNOIZ] += tempOutNoise;
			inst->RESnVar[INNOIZ] += tempInNoise;
			data->outNoiz += tempOutNoise;
			data->inNoise += tempInNoise;
			inst->RESnVar[LNLSTDENS] = lnNdens;
		    }
		    if (data->prtSummary) {
			data->outpVector[data->outNumber++] = noizDens;
		    }
		    break;

		case INT_NOIZ:        /* already calculated, just output */
		    if (((NOISEAN*)ckt->CKTcurJob)->NStpsSm != 0) {
			data->outpVector[data->outNumber++] = inst->RESnVar[OUTNOIZ];
			data->outpVector[data->outNumber++] = inst->RESnVar[INNOIZ];
		    }    /* if */
		    break;
		}    /* switch (mode) */
		break;

	    case N_CLOSE:
		return (OK);         /* do nothing, the main calling routine will close */
		break;               /* the plots */
	    }    /* switch (operation) */
	}    /* for inst */
    }    /* for model */

return(OK);
}


