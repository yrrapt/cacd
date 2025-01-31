/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/

/* Pretty print the sensitivity info for
 * all the VCVS in the circuit.
 */

#include "spice.h"
#include <stdio.h>
#include "cktdefs.h"
#include "vcvsdefs.h"
#include "sperror.h"

void
VCVSsPrint(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;
{
    register VCVSmodel *model = (VCVSmodel *)inModel;
    register VCVSinstance *here;

    printf("VOLTAGE CONTROLLED VOLTAGE SOURCES-----------------\n");
    /*  loop through all the voltage source models */
    for( ; model != NULL; model = model->VCVSnextModel ) {

        printf("Model name:%s\n",(char*)model->VCVSmodName);

        /* loop through all the instances of the model */
        for (here = model->VCVSinstances; here != NULL ;
                here=here->VCVSnextInstance) {

            printf("    Instance name:%s\n",(char*)here->VCVSname);
            printf("      Positive, negative nodes: %s, %s\n",
                    (char*)CKTnodName(ckt,here->VCVSposNode),
                    (char*)CKTnodName(ckt,here->VCVSnegNode));
            printf("      Controlling Positive, negative nodes: %s, %s\n",
                    (char*)CKTnodName(ckt,here->VCVScontPosNode),
                    (char*)CKTnodName(ckt,here->VCVScontNegNode));
            printf("      Branch equation number: %s\n",
                    (char*)CKTnodName(ckt,here->VCVSbranch));
            printf("      Coefficient: %f\n",here->VCVScoeff);
            printf("    VCVSsenParmNo:%d\n",here->VCVSsenParmNo);
        }
    }
}
