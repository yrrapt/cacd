/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1988 Thomas L. Quarles
**********/

#include "spice.h"
#include <stdio.h>
#include "ifsim.h"
#include "inpdefs.h"
#include "inpmacs.h"
#include "fteext.h"

void INP2Z(GENERIC *ckt, INPtables *tab, card *current)
{
    /* Zname <node> <node> <node> <model> [<val>] [OFF] [IC=<val>,<val>] */

    int mytype; /* the type we looked up */
    int type;   /* the type the model says it is */
    char *line; /* the part of the current line left to parse */
    char *name; /* the resistor's name */
    char *nname1;   /* the first node's name */
    char *nname2;   /* the second node's name */
    char *nname3;   /* the third node's name */
    GENERIC *node1; /* the first node's node pointer */
    GENERIC *node2; /* the second node's node pointer */
    GENERIC *node3; /* the third node's node pointer */
    int error;      /* error code temporary */
    GENERIC *fast;  /* pointer to the actual instance */
    IFvalue ptemp;  /* a value structure to package resistance into */
    int waslead;    /* flag to indicate that funny unlabeled number was found */
    double leadval; /* actual value of unlabeled number */
    char *model;    /* the name of the model */
    INPmodel *thismodel;/* pointer to model description for user's model */
    GENERIC *mdfast;    /* pointer to the actual model */
    IFuid uid;      /* uid for default model */

    mytype = INPtypelook("MES");
    if (mytype < 0) {
        LITERR("Device type MES not supported by this binary\n")
        return;
    }

    line = current->line;
    INPgetTok(&line, &name,   1); INPinsert(&name, tab);
    INPgetTok(&line, &nname1, 1); INPtermInsert(ckt, &nname1, tab, &node1);
    INPgetTok(&line, &nname2, 1); INPtermInsert(ckt, &nname2, tab, &node2);
    INPgetTok(&line, &nname3, 1); INPtermInsert(ckt, &nname3, tab, &node3);
    INPgetTok(&line, &model,  1); INPinsert(&model, tab);

    thismodel = NULL;
    current->error = INPgetMod(ckt, model, &thismodel, tab);
    if (thismodel) {
        if (mytype != thismodel->INPmodType) {
            LITERR("incorrect model type")
            return;
        }
        type = mytype;
        mdfast = thismodel->INPmodfast;
    } else {
        type = mytype;
        if (!tab->defZmod) {
            /* create default Z model */
            IFnewUid(ckt, &uid, (IFuid)NULL, "Z", UID_MODEL, (GENERIC**)NULL);
            IFC(newModel, (ckt, type, &tab->defZmod, uid))
        }
        mdfast = tab->defZmod;
    }
    IFC(newInstance, (ckt, mdfast, &fast, name))
    IFC(bindNode, (ckt, fast, 1, node1))
    IFC(bindNode, (ckt, fast, 2, node2))
    IFC(bindNode, (ckt, fast, 3, node3))
    PARSECALL((&line, ckt, type, fast, &leadval, &waslead, tab))
    if (waslead) {
        ptemp.rValue = leadval;
        GCA(INPpName, ("area", &ptemp, ckt, type, fast))
    }
}
