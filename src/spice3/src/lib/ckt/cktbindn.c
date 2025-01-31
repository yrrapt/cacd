/**********
Copyright 1990 Regents of the University of California. All rights reserved.
Author: 1985 Thomas L. Quarles
**********/

/* CKTbindNode
 *  bind a node of the specified device of the given type to its place
 *  in the specified circuit.
 */

#include "spice.h"
#include <stdio.h>
#include "ifsim.h"
#include "smpdefs.h"
#include "cktdefs.h"
#include "util.h"
#include "devdefs.h"
#include "sperror.h"

extern SPICEdev *DEVices[];

int CKTbindNode(GENERIC *ckt, GENERIC *fast, int term, GENERIC *node)
{
    int type = ((GENinstance *)fast)->GENmodPtr->GENmodType;

    if (*DEVices[type]->DEVpublic.terms >= term && term > 0) {
	int mappednode = ((CKTnode *)node)->number;
        switch (term) {
            default: return(E_NOTERM);
            case 1:
                ((GENinstance *)fast)->GENnode1 = mappednode;
                break;
            case 2:
                ((GENinstance *)fast)->GENnode2 = mappednode;
                break;
            case 3:
                ((GENinstance *)fast)->GENnode3 = mappednode;
                break;
            case 4:
                ((GENinstance *)fast)->GENnode4 = mappednode;
                break;
            case 5:
                ((GENinstance *)fast)->GENnode5 = mappednode;
                break;
        }
        return(OK);
    }
    return(E_NOTERM);
}
