/**********
Copyright 1990 Regents of the University of California. All rights reserved.
Author: 1985 Thomas L. Quarles
**********/

/* INPlist(file,deck,type)
 *  provide an input listing on the specified file of the given
 *  card deck. The listing should be of either PHYSICAL or LOGICAL
 *  lines as specified by the type parameter.
 */

#include "spice.h"
#include <stdio.h>
#include "inpdefs.h"
#include "util.h"

void INPlist(FILE *file, card *deck, int type)
{
    card *here;
    card *there;

    if (type == LOGICAL) {
        for (here = deck; here; here = here->nextcard) {
            fprintf(file, "%6d : %s\n", here->linenum, here->line);
            if (here->error) fprintf(file, "%s", here->error);
        }
    } else if (type == PHYSICAL) {
        for (here = deck; here; here = here->nextcard) {
            if (!here->actualLine) {
                fprintf(file, "%6d : %s\n", here->linenum, here->line);
                if (here->error) fprintf(file, "%s", here->error);
            } else {
                for (there = here->actualLine; there; there = there->nextcard) {
                    fprintf(file, "%6d : %s\n", there->linenum, there->line);
                    if (there->error) fprintf(file, "%s", there->error);
                }
            }
        }
    }
}
