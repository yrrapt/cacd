/**********
Copyright 1990 Regents of the University of California. All rights reserved.
**********/

/*
 * Memory alloction functions
 */

#include "spice.h"
#include <stdio.h>
#include "misc.h"

/* Malloc num bytes and initialize to zero. Fatal error if the space can't
 * be malloc'd.  Return NULL for a request for 0 bytes.
 */

char * tmalloc (int num)
{
    char *s;

    if (!num) return NULL;

    s = malloc((unsigned) num);
    if (!s) {
        fprintf(stderr, "malloc: Internal Error: can't allocate %d bytes.\n", num);
        exit(EXIT_BAD);
    }

    (void) memset(s, 0, (unsigned) num);

    return(s);
}

char * trealloc (char *str, int num)
{
    char *s;

    if (!num) {
	if (str) free(str);
	return NULL;
    }

    if (!str)
	s = tmalloc(num);
    else
        s = realloc(str, (unsigned) num);

    if (!s) {
        fprintf(stderr, "realloc: Internal Error: can't allocate %d bytes.\n", num);
        exit(EXIT_BAD);
    }
    return(s);
}

void txfree (char *ptr)
{
	if (ptr) free(ptr);
}
