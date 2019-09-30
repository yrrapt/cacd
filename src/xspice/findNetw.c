
/*
 * ISC License
 *
 * Copyright (C) 1987-2011 by
 *	Arjan van Genderen
 *	Simon de Graaf
 *	Nick van der Meijs
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

#include "src/xspice/incl.h"

extern long *Nil;
extern int dialect;
extern int out_indent;

struct model_info *Netws = NULL;

struct model_info *findNetw (char *name, int imported, DM_PROJECT *father_proj, int submod)
{
    DM_PROJECT *proj;
    char *real_name;
    struct model_info *ntw;

    for (ntw = Netws; ntw; ntw = ntw -> next) {
	if (strcmp (ntw -> name, name) == 0) return (ntw); /* found */
    }
    /* not found, read it ! */

    if (imported == IMPORTED) {
	IMPCELL *icp = dmGetImpCell (father_proj, name, 1);
	proj = dmOpenProject (icp -> dmpath, PROJ_READ);
	name = icp -> alias;
	real_name = icp -> cellname;
    }
    else {
	proj = father_proj;
	real_name = name = strsave (name);
    }

    ntw = newNetw (name, proj, imported, real_name, submod);

    return (ntw);
}

struct model_info *newNetw (char *name, DM_PROJECT *proj, int imported, char *orig_name, int submod)
{
    struct model_info *ntw;

    PALLOC (ntw, 1, struct model_info);
    ntw -> next = Netws;
    Netws = ntw;
    ntw -> name = name;
    ntw -> out_name = name;
    if (dialect == PSTAR) {
	char *p = name - 1;
	while (*++p)
	    if (*p == '_') {
		ntw -> out_name = p = strsave (name);
		--p;
		while (*++p) if (*p == '_') *p = '-';
		break;
	    }
    }
    ntw -> orig_name = orig_name;
    ntw -> prefix[0] = 'x';
    ntw -> proj = proj;
    ntw -> imported = imported;

    readTerm (ntw, 0, submod);

    return (ntw);
}

/* readTerm checks out the model and reads the terminals
   of the model.  If the second argument is 1, the model
   is considered to be a function.
*/

void readTerm (struct model_info *m, int funf, int submod)
{
    DM_STREAM *dsp;

    /*
    ** In the current database implementation, function blocks
    ** and network desciptions are both stored in the same
    ** cell directory within the circuit view.
    */
    m -> dkey = dmCheckOut (m -> proj, m -> orig_name, submod ? ACTUAL : WORKING, DONTCARE, CIRCUIT, READONLY);

    dsp = dmOpenStream (m -> dkey, funf ? "fterm" : "term", "r");
    m -> terms = NULL;
    termRun (m, dsp);
    dmCloseStream (dsp, COMPLETE);
    if (funf) dmCheckIn (m -> dkey, COMPLETE);
}

static int findTermType (char *attrString)
{
    char *val;
    char buf[256];

    /* first, check possible escher specification */

    if (sscanf (attrString, "%s", buf) > 0) {
	switch (*buf) {
	case 'i':
	    if (strcmp (buf, "input") == 0) return (INPUT);
	    if (strcmp (buf, "inout") == 0) return (INOUT);
	    if (strcmp (buf, "in/output") == 0) return (INOUT);
	    break;
	case 'o':
	    if (strcmp (buf, "output") == 0) return (OUTPUT);
	    break;
	case 't':
	    if (strcmp (buf, "tristate") == 0) {
		return (OUTPUT3);
	    }
	/*
	case 'g':
	    if (strcmp (buf, "global") == 0) return (GLOBAL);
	*/
	}
    }

    /* next, check possible functional specification */

    if ((val = getAttrValue (attrString, "ftt"))) {
	switch (val[0]) {
	case '0': return (OUTPUT);
	case '1': return (INPUT);
	case '2': return (INREAD);
	case '3': return (INOUT);
	case '4': return (OUTPUT3);
	}
    }
    return (0); /* unknown */
}

void structcpy (char *s, char *t, int size)
{
    while (size-- > 0) *s++ = *t++;
}

void termRun (struct model_info *m, DM_STREAM *dsp)
{
    static struct term_ref *treflast;
    struct term_ref *tref;
    char attribute_string[256];
    long lower[10], upper[10];
    int i, type;

    cterm.term_attribute = attribute_string;
    cterm.term_lower = lower;
    cterm.term_upper = upper;

    while (dmGetDesignData (dsp, CIR_TERM) > 0) {

	type = findTermType (cterm.term_attribute);

	/* if (type == GLOBAL) continue; */

	PALLOC (tref, 1, struct term_ref);
	PALLOC ((tref -> t), 1, struct cir_term);
	structcpy ((char *)(tref -> t), (char *)&cterm, (int) sizeof (struct cir_term));

	if (cterm.term_dim) {
	    PALLOC (tref -> t -> term_lower, cterm.term_dim, long);
	    PALLOC (tref -> t -> term_upper, cterm.term_dim, long);
	    for (i = 0; i < cterm.term_dim; ++i) {
		tref -> t -> term_lower[i] = lower[i];
		tref -> t -> term_upper[i] = upper[i];
	    }
	}

	tref -> type = type;
	tref -> next = NULL;
	if (!m -> terms) m -> terms = tref;
	else treflast -> next = tref;
	treflast = tref;
    }

    dmSeek (dsp, 0, 0);
}

void prImpNetw ()
{
    prImpMod (Netws, "circuits");
}

void prImpMod (struct model_info *m, char *str)
{
    int first, newproj;
    DM_PROJECT *proj;
    struct model_info *m2;
    char buf[512];

    first = 1;
    for (; m; m = m -> next) {
	if (m -> imported == IMPORTED) {

	    /* list imported cells per project */

	    newproj = 1;
	    proj = m -> proj;

	    for (m2 = m; m2; m2 = m2 -> next) {
		if (m2 -> proj == proj) {
		    if (first) {
			first = 0;
			oprint (0, "\n");
			if (dialect == PSTAR) oprint (0, "/*\n");
			startComment ();
			out_indent = 0;
			oprint (0, "  -- imported ");
			oprint (1, str);
			oprint (1, " --\n");
			startComment ();
			oprint (0, "\n");
		    }
		    if (newproj) {
			newproj = 0;
			startComment ();
			out_indent = 0;
			oprint (0, "");
			out_indent = 4;
			if (!proj) fatalErr ("prImpMod: projkey == NULL", 0);
			oprint (1, proj -> dmpath);
			oprint (1, ":\n");
		    }

		    startComment ();
		    out_indent = 0;
		    oprint (0, "");
		    out_indent = 8;
		    if (strlen (m2 -> orig_name) < 16)
			sprintf (buf, "%-15s %s\n", m2 -> orig_name, m2 -> name);
		    else {
			sprintf (buf, "%s\n", m2 -> orig_name);
			oprint (0, buf);
			startComment ();
			out_indent = 0;
			oprint (0, "");
			out_indent = 8;
			sprintf (buf, "%16s%s\n", "", m2 -> name);
		    }
		    oprint (0, buf);

		    m2 -> imported = LOCAL;
		    /* so the input is partly destroyed ! */
		}
	    }
        }
    }

    if (!first) {
	if (dialect == PSTAR) oprint (0, "*/\n");
	oprint (0, "");
	out_indent = 0;
    }
}
