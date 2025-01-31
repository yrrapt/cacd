/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/

/* Pretty print the sensitivity info for
 * all the VCCS in the circuit.
 */

#include "spice.h"
#include <stdio.h>
#include "smpdefs.h"
#include "cktdefs.h"
#include "vccsdefs.h"
#include "sperror.h"

void
VCCSsPrint(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;
{
    register VCCSmodel *model = (VCCSmodel *)inModel;
    register VCCSinstance *here;

    printf("VOLTAGE CONTROLLED CURRENT SOURCES-----------------\n");
    /*  loop through all the source models */
    for( ; model != NULL; model = model->VCCSnextModel ) {

        printf("Model name:%s\n",(char*)model->VCCSmodName);

        /* loop through all the instances of the model */
        for (here = model->VCCSinstances; here != NULL ;
                here=here->VCCSnextInstance) {

            printf("    Instance name:%s\n",(char*)here->VCCSname);
            printf("      Positive, negative nodes: %s, %s\n",
		(char*)CKTnodName(ckt,here->VCCSposNode),
		(char*)CKTnodName(ckt,here->VCCSnegNode));
            printf("      Controlling Positive, negative nodes: %s, %s\n",
		(char*)CKTnodName(ckt,here->VCCScontPosNode),
		(char*)CKTnodName(ckt,here->VCCScontNegNode));
            printf("      Coefficient: %f\n",here->VCCScoeff);
            printf("    VCCSsenParmNo:%d\n",here->VCCSsenParmNo);
        }
    }
}
