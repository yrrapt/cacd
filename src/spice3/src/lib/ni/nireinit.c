/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/

/* NIreinit(ckt)
 *  Perform reinitialization necessary for the numerical iteration
 *  package - the matrix has now been fully accessed once, so we know
 *  how big it is, so allocate RHS vector
 */

#include "spice.h"
#include "cktdefs.h"
#include <stdio.h>
#include "util.h"
#include "smpdefs.h"
#include "sperror.h"

#define CKALLOC(ptr, size, type) \
if (!(ckt->ptr = (type *) MALLOC((size)*sizeof(type)))) return(E_NOMEM)

int NIreinit (register CKTcircuit *ckt)
{
    register int size;
#ifdef PREDICTOR
    register int i;
#endif

    size = SMPmatSize(ckt->CKTmatrix);
    CKALLOC(CKTrhs, size+1, double);
    CKALLOC(CKTrhsOld, size+1, double);
    CKALLOC(CKTrhsSpare, size+1, double);
    CKALLOC(CKTirhs, size+1, double);
    CKALLOC(CKTirhsOld, size+1, double);
    CKALLOC(CKTirhsSpare, size+1, double);
#ifdef PREDICTOR
    CKALLOC(CKTpred, size+1, double);
    for (i = 0; i < 8; i++) {
        CKALLOC(CKTsols[i], size+1, double);
    }
#endif /* PREDICTOR */
    ckt->CKTniState = NISHOULDREORDER | NIACSHOULDREORDER | NIPZSHOULDREORDER;
    return(0);
}
