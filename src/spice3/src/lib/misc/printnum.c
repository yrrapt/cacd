/**********
Copyright 1990 Regents of the University of California. All rights reserved.
Author: 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
**********/

/* Print a number in a reasonable form. This is the sort of thing that
 * %G does, but more appropriate for spice.  Returns static data.
 */

#include <stdio.h>
#include "spice.h"

int cp_numdgt = -1;

char * printnum (double num)
{
    static char buf[128];
    int n = 6;

    if (cp_numdgt > 1) n = cp_numdgt;
    if (num < 0.0) n--;

    sprintf(buf, "%.*le", n, num);
    return (buf);
}
