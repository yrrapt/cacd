/*
 * ISC License
 *
 * Copyright (C) 1986-2018 by
 *	A.C. de Graaf
 *	A.J. van Genderen
 *	S. de Graaf
 *	N.P. van der Meijs
 * Delft University of Technology
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "src/sls/extern.h"

static int num_from_path (PATH_SPEC *path, PATH_SPEC *path_w);
static int num_of_signal (PATH_SPEC *path, RES_FILE *rf);
static char skipspace (FILE *fp);

RES_FILE * RF = NULL;    /* begin of res file list */
RES_FILE * end_RF;       /* end of res file list */

int ret_arr_size = 0;
int * ret_arr = NULL;

STRING_REF *names_from_path (int traillen, PATH_SPEC *path)
/* returns expanded names specified by path */
/* users use traillen = 0 */
{
    char *p, *p1, *p2, *indexp;
    int i, currlen, ready, ready2;
    STRING_REF * beg_ref = NULL;
    STRING_REF * end_ref = NULL;
    STRING_REF * str_ref;
    int xvector[MAXDIM];
    char name[DM_MAXNAME + 41];  /* 40 characters for the index */

    while (path) {

        p = name;
        if (traillen > 0) *p++ = '.';

        strcpy (p, path -> name);
	p = p + strlen (path -> name);
	indexp = p;

        xvector[0] = path -> xarray[0][0];
        for (i = path -> xarray[0][0]; i > 0; i--) {
            xvector[i] = path -> xarray[i][0];
        }

        ready = FALSE;
        do {
	    if (xvector[0] > 0) {
		p = indexp;
	        *p++ = '[';
                for (i = 1; i <= xvector[0]; i++) {
		    if ( *(p - 1) != '[' ) *p++ = ',';
	            sprintf (p, "%d", xvector[i]);
		    while (*++p != '\0') ;
		}
		*p++ = ']';
	    }
            *p = '\0';

            currlen = traillen + (p - name);

            if (path -> next == NULL) {
	        PALLOC (str_ref, 1, STRING_REF);
		str_ref -> next = NULL;
		PALLOC (str_ref -> str, currlen + 1, char);
		*(str_ref -> str + currlen) = '\0';
	    }
	    else {
		str_ref = names_from_path (currlen, path -> next);
	    }

            if (beg_ref == NULL) {
		beg_ref = end_ref = str_ref;
	    }
	    else {
		end_ref -> next = str_ref;
		end_ref = str_ref;
	    }

	    ready2 = FALSE;
	    do {
		p1 = name;
		p2 = end_ref -> str + traillen;
		while (*p1 != '\0') *p2++ = *p1++;
		if (end_ref -> next == NULL)
		    ready2 = TRUE;
		else
		    end_ref = end_ref -> next;
	    }
	    while (! ready2);

	    /* find new indices by incrementing or decrementing
	       the current vector */

	    if (xvector[0] > 0) {
                for (i = xvector[0]; i > 0; i--) {
		    if (path -> xarray[i][0] <= path -> xarray[i][1]) {
                        xvector[i]++;
	                if (xvector[i] <= path -> xarray[i][1]) break;
	                else {
		            xvector[i] = path -> xarray[i][0];
		            if (i == 1) ready = TRUE;
	                }
		    }
		    else {
                        xvector[i]--;
	                if (xvector[i] >= path -> xarray[i][1]) break;
	                else {
		            xvector[i] = path -> xarray[i][0];
		            if (i == 1) ready = TRUE;
	                }
		    }
                }
	    }
	    else
	        ready = TRUE;
        }
        while (! ready);

        path = path -> also;
    }

    return (beg_ref);
}

void wr_path (FILE *fp, PATH_SPEC *path)
/* writes path into file fp according the network format */
/* this function doesn't bother with the 'also' pointer */
{
    int i;

    fprintf (fp, " (");
    while (path) {
	if (path -> xarray[0][0] > 0) {
	    fprintf (fp, " (");
	    fprintf (fp, "%s", path -> name);
	    for (i = 1; i <= path -> xarray[0][0]; i++) {
		if (path -> xarray[i][0] == path -> xarray[i][1]) {
		    fprintf (fp, " %d", path -> xarray[i][0]);
		}
		else {
		    fprintf (fp, " (%d %d)",
		    path -> xarray[i][0], path -> xarray[i][1]);
		}
	    }
	    fprintf (fp, ")");
	}
	else {
	    fprintf (fp, " %s", path -> name);
	}
	path = path -> next;
    }
    fprintf (fp, " )");
}

void write_paths (FILE *fp, RES_PATH *paths)
/* writes full paths into file fp according the network format */
/* this function doesn't bother with the 'also' pointer */
{
    while (paths) {
	wr_path (fp, paths -> path);
	paths = paths -> next;
    }
}

RES_FILE *read_paths (FILE *fp, char *fn)
/* reads the signal paths for file pointed to by fp and with name fn */
/* returns the pointer to resfile information */
{
    RES_PATH * end_r_path = 0;
    RES_PATH * r_path;
    PATH_SPEC * end_p = 0;
    PATH_SPEC * p;
    int i;
    int up;
    int low;
    int help;

    if (RF == NULL) {
	PALLOC (RF, 1, RES_FILE);
	end_RF = RF;
    }
    else {
	RES_FILE * rf;
	for (rf = RF; rf; rf = rf -> next) {
	    if (strcmp (rf -> name, fn) == 0) return (rf);
	}
	PALLOC (end_RF -> next, 1, RES_FILE);
	end_RF = end_RF -> next;
    }
    end_RF -> next = NULL;
    end_RF -> signals = NULL;
    end_RF -> sig_cnt = 0;
    end_RF -> fp = fp;
    strcpy (end_RF -> name, fn);

    rewind (fp);
    fscanf (fp, "%le", &(end_RF -> scaling));
    while (skipspace (fp) == '(') {
	getc (fp);  /* '(' */

        PALLOC (r_path, 1, RES_PATH);
	if (end_RF -> signals == NULL) {
	    end_RF -> signals = r_path;
	    end_r_path = r_path;
	}
	else {
	    end_r_path -> next = r_path;
	    end_r_path = r_path;
	}
	end_r_path -> next = NULL;
	end_r_path -> totnum = 1;

	while (skipspace (fp) != ')') {

	    PALLOC (p, 1, PATH_SPEC);
	    if (end_r_path -> path == NULL) {
	        end_r_path -> path = p;
		end_p = p;
	    }
	    else {
		end_p -> next = p;
		end_p = p;
	    }
            end_p -> next = NULL;
	    end_p -> xarray[0][0] = 0;

            if (skipspace (fp) == '(') {
	        getc (fp);  /* '(' */
		fscanf (fp, "%s", end_p -> name);

	        i = 1;
	        while (skipspace (fp) != ')') {
	            if (skipspace (fp) == '(') {
                        getc (fp);  /* '(' */
	                fscanf (fp, "%d", &low);
	                fscanf (fp, "%d", &up);
		        skipspace (fp);
                        getc (fp);  /* ')' */
	            }
	            else {
	                fscanf (fp, "%d", &up);
			low = up;
                    }
		    end_p -> xarray[i][0] = low;
		    end_p -> xarray[i][1] = up;
		    if (low > up) { help = up; up = low; low = help; }
	            (end_p -> xarray[0][0])++;
		    end_r_path -> totnum = end_r_path -> totnum * (up - low + 1);
	            i++;
	        }
                getc (fp);  /* ')' */
	    }
	    else {
		fscanf (fp, "%s", end_p -> name);
	    }

	}

	end_RF -> sig_cnt += end_r_path -> totnum;
	skipspace (fp);
	getc (fp);  /* ')' */
    }

    while (getc (fp) != '\n') ;

    end_RF -> offset = ftell (fp);

    return (end_RF);
}

static char skipspace (FILE *fp)
/* skips all spaces and tabs until next non-space */
/* or newline character (which is returned) */
{
    char c;
    while ( (c = getc (fp)) == ' ' || c == '\t' ) ;
    ungetc (c, fp);
    return (c);
}

int *nums_of_signals (PATH_SPEC *path, RES_FILE *rf)
/* returns sequence numbers of the signals specified by 'path' */
/* and in resfile with pointer 'rf' */
{
    PATH_SPEC *tmp1_path, *tmp2_path;
    PATH_SPEC help_path[MAXHIERAR];
    int * help_arr;
    int nbr, nbr_path;
    int i, n, m, r, ready, ready2;

    nbr_path = 0;
    tmp1_path = path;
    tmp2_path = help_path;
    while (tmp1_path) {
	nbr_path++;
	strcpy (tmp2_path -> name, tmp1_path -> name);
	tmp2_path -> xarray[0][0] = tmp1_path -> xarray[0][0];
	for (i = tmp1_path -> xarray[0][0]; i > 0; i--) {
	    tmp2_path -> xarray[i][0] = tmp1_path -> xarray[i][0];
	}
	tmp1_path = tmp1_path -> next;
	if (tmp1_path) {
	    tmp2_path -> next = tmp2_path + 1;
	    tmp2_path = tmp2_path + 1;
	}
	else
	    tmp2_path -> next = NULL;
    }

    n = 0;
    ready = FALSE;
    while ( ! ready ) {

	if (n + 1 > ret_arr_size) {
	    if (ret_arr_size == 0)       /* current ret_arr is too small */
		ret_arr_size = 50;
	    else
		ret_arr_size = ret_arr_size * 2;
	    PALLOC (help_arr, ret_arr_size, int);
	    if (ret_arr) {
	        for (m = 1; m <= n; m++) {
		    help_arr[m] = ret_arr[m];
	        }
	        CFREE (ret_arr);
	    }
	    ret_arr = help_arr;
	}
        ret_arr[++n] = num_of_signal (help_path, rf);

        ready = TRUE;
        for (nbr = nbr_path; nbr > 0; nbr--) {

            /* first the current hierarchical depth is searched */
            tmp1_path = path;
	    tmp2_path = help_path;
            r = 1;
	    while (r != nbr) {
		tmp1_path = tmp1_path -> next;
		tmp2_path = tmp2_path -> next;
		r++;
	    }

            /* look whether an index of this path can be in or decremented */
            ready2 = FALSE;
	    if (tmp2_path -> xarray[0][0] > 0) {
                for (i = tmp2_path -> xarray[0][0]; i > 0; i--) {
		    if (tmp1_path -> xarray[i][0]
			<= tmp1_path -> xarray[i][1]) {
                        tmp2_path -> xarray[i][0]++;
	                if (tmp2_path -> xarray[i][0]
			    <= tmp1_path -> xarray[i][1])
		            break;
	                else {
		            tmp2_path -> xarray[i][0] =
			    tmp1_path -> xarray[i][0];
		            if (i == 1)
		                ready2 = TRUE;
	                }
		    }
		    else {
                        tmp2_path -> xarray[i][0]--;
	                if (tmp2_path -> xarray[i][0]
			    >= tmp1_path -> xarray[i][1])
		            break;
	                else {
		            tmp2_path -> xarray[i][0] =
			    tmp1_path -> xarray[i][0];
		            if (i == 1)
		                ready2 = TRUE;
	                }
		    }
                }
	    }
	    else
	        ready2 = TRUE;

            if (ready2 == FALSE) {
		ready = FALSE;
		break;              /* index has been in or decremented */
	    }
        }
    }
    ret_arr[0] = n;   /* total number of signals */

    return (ret_arr);
}

static int num_of_signal (PATH_SPEC *path, RES_FILE *rf)
/* returns the sequence number of the signal specified by 'path' */
/* and in resfile with pointer 'rf' */
{
    RES_PATH * r_p;
    int n;
    int nprev = 0;

    for (r_p = rf -> signals; r_p; r_p = r_p -> next) {
	n = num_from_path (r_p -> path, path);
	if (n > 0) {
	    return (nprev + n);
	}
	else {
	    /*
	    if (n < 0)        for now we assume that the index
		return (n);   can be correct in another path item
	    */
	    nprev += r_p -> totnum;
	}
    }
    return (0);   /* signal not found */
}

static int num_from_path (PATH_SPEC *path, PATH_SPEC *path_w)
/* returns serial number of path_w in path */
/* returns 0 when path_w is not in path    */
/* returns -1 when path_w has a bad index  */
/* returns -2 when path_w is not a node    */
{
    int i, num, w, low, up;

    num = 0;

    while (path && path_w) {
	if ( strcmp (path -> name, path_w -> name) != 0 )
	    return (0);
	else {
	    if (path_w -> xarray[0][0] != path -> xarray[0][0]) {
		return (-1);
	    }
	    if (path -> xarray[0][0] > 0) {
		for (i = 1; i <= path -> xarray[0][0]; i++) {
		    w = path_w -> xarray[i][0];
		    low = path -> xarray[i][0];
		    up = path -> xarray[i][1];
		    if (w < low || w > up) {
			return (-1);
		    }
		    num = num * (up - low + 1) + w - low;
		}
            }
	}
	path = path -> next;
	path_w = path_w -> next;
    }

    if (path || path_w) return (-2);

    return (num + 1);
}
