/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/

/* Pretty print the sensitivity info for all
 * the CCVS in the circuit.
 */

#include "spice.h"
#include <stdio.h>
#include "cktdefs.h"
#include "ccvsdefs.h"
#include "util.h"
#include "sperror.h"

void
CCVSsPrint(inModel,ckt)
    GENmodel *inModel;
    register CKTcircuit *ckt;
{
    register CCVSmodel *model = (CCVSmodel*)inModel;
    register CCVSinstance *here;

    printf("CURRENT CONTROLLED VOLTAGE SOURCES-----------------\n");
    /*  loop through all the voltage source models */
    for( ; model != NULL; model = model->CCVSnextModel ) {

        printf("Model name:%s\n",(char*)model->CCVSmodName);

        /* loop through all the instances of the model */
        for (here = model->CCVSinstances; here != NULL ;
                here=here->CCVSnextInstance) {
            printf("    Instance name:%s\n",(char*)here->CCVSname);
            printf("      Positive, negative nodes: %s, %s\n",
                    (char*)CKTnodName(ckt,here->CCVSposNode),
                    (char*)CKTnodName(ckt,here->CCVSnegNode));
            printf("      Controlling source name: %s\n",
                    (char*)here->CCVScontName);
            printf("      Branch equation number: %s\n",
                    (char*)CKTnodName(ckt,here->CCVSbranch));
            printf("      Controlling Branch equation number: %s\n",
                    (char*)CKTnodName(ckt,here->CCVScontBranch));
            printf("      Coefficient: %f\n",here->CCVScoeff);
            printf("    CCVSsenParmNo:%d\n",here->CCVSsenParmNo);
        }
    }
}

