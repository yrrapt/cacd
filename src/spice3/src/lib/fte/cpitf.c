/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
**********/

#include "spice.h"
#include "cpdefs.h"
#include "ftedefs.h"
#include "ftedata.h"
#include "fteparse.h"

/* Set some standard variables and aliases, etc, and init the ccom stuff.  */

void ft_cpinit()
{
    wordlist wl1, wl2, wl3, *wl;
    bool t = true;
    char buf[BSIZE_SP], **x, *s;
    struct comm *c;
    int i;
    FILE *fp;
    static char *predefs[] = {
        "yes",      "1",
        "true",     "1",
        "no",       "0",
        "false",    "0",
        "pi",       "3.14159265358979323846",
        "e",        "2.71828182844590452353",
        "c",        "2.997925e8",
        "i",        "0,1",
        "kelvin",   "-273.15",
        "echarge",  "1.60219e-19",
        "boltz",    "1.38062e-23",
        "planck",   "6.62620e-34"
    } ;
    static char *udfs[] = {
        "max(x,y)", "(x gt y) * x + (x le y) * y",
        "min(x,y)", "(x lt y) * x + (x ge y) * y",
        "vdb(x)",   "db(v(x))",
        "vdb(x,y)", "db(v(x) - v(y))",
        "vi(x)",    "im(v(x))",
        "vi(x,y)",  "im(v(x) - v(y))",
        "vm(x)",    "mag(v(x))",
        "vm(x,y)",  "mag(v(x) - v(y))",
        "vp(x)",    "ph(v(x))",
        "vp(x,y)",  "ph(v(x) - v(y))",
        "vr(x)",    "re(v(x))",
        "vr(x,y)",  "re(v(x) - v(y))"
    } ;

    cp_ccon(true);  /* So the user can type ahead... */
    cp_init();

    if (!cp_nocc) {
        /* Add commands... */
        for (c = cp_coms; c->co_comname; c++) {
            if (c->co_spiceonly && ft_nutmeg) continue;
            cp_addcomm(c->co_comname, c->co_cctypes[0], c->co_cctypes[1], c->co_cctypes[2], c->co_cctypes[3]);
            cp_addkword(CT_COMMANDS, c->co_comname);
        }
        /* And keywords... These are the ones that are constant... */
        if (!ft_nutmeg) {
            cp_addkword(CT_LISTINGARGS, "deck");
            cp_addkword(CT_LISTINGARGS, "logical");
            cp_addkword(CT_LISTINGARGS, "physical");
            cp_addkword(CT_LISTINGARGS, "expand");

            cp_addkword(CT_STOPARGS, "when");
            cp_addkword(CT_STOPARGS, "after");
        }

        cp_addkword(CT_PLOT, "new");

        cp_addkword(CT_PLOTKEYWORDS, "xlimit");
        cp_addkword(CT_PLOTKEYWORDS, "ylimit");
        cp_addkword(CT_PLOTKEYWORDS, "vs");
        cp_addkword(CT_PLOTKEYWORDS, "xindices");
        cp_addkword(CT_PLOTKEYWORDS, "xcompress");
        cp_addkword(CT_PLOTKEYWORDS, "xdelta");
        cp_addkword(CT_PLOTKEYWORDS, "ydelta");
        cp_addkword(CT_PLOTKEYWORDS, "lingrid");
        cp_addkword(CT_PLOTKEYWORDS, "loglog");
        cp_addkword(CT_PLOTKEYWORDS, "linear");
        cp_addkword(CT_PLOTKEYWORDS, "xlog");
        cp_addkword(CT_PLOTKEYWORDS, "ylog");
        cp_addkword(CT_PLOTKEYWORDS, "polar");
        cp_addkword(CT_PLOTKEYWORDS, "smith");
        cp_addkword(CT_PLOTKEYWORDS, "smithgrid");
        cp_addkword(CT_PLOTKEYWORDS, "nointerp");
        cp_addkword(CT_PLOTKEYWORDS, "title");
        cp_addkword(CT_PLOTKEYWORDS, "xlabel");
        cp_addkword(CT_PLOTKEYWORDS, "ylabel");
        cp_addkword(CT_PLOTKEYWORDS, "linplot");
        cp_addkword(CT_PLOTKEYWORDS, "combplot");
        cp_addkword(CT_PLOTKEYWORDS, "pointplot");

        cp_addkword(CT_RUSEARGS, "space");
        cp_addkword(CT_RUSEARGS, "faults");
        cp_addkword(CT_RUSEARGS, "elapsed");
        cp_addkword(CT_RUSEARGS, "totiter");
        cp_addkword(CT_RUSEARGS, "traniter");
        cp_addkword(CT_RUSEARGS, "tranpoints");
        cp_addkword(CT_RUSEARGS, "accept");
        cp_addkword(CT_RUSEARGS, "rejected");
        cp_addkword(CT_RUSEARGS, "time");
        cp_addkword(CT_RUSEARGS, "trantime");
        cp_addkword(CT_RUSEARGS, "lutime");
        cp_addkword(CT_RUSEARGS, "solvetime");
        cp_addkword(CT_RUSEARGS, "transolvetime");
        cp_addkword(CT_RUSEARGS, "loadtime");
        cp_addkword(CT_RUSEARGS, "all");

        cp_addkword(CT_VECTOR, "all");

        for (x = ft_setkwords; *x; x++) {
            cp_addkword(CT_VARIABLES, *x);
	}
        for (i = 0; ; i++) {
            if (!(s = ft_typenames(i))) break;
            cp_addkword(CT_TYPENAMES, s);
        }
    }

    cp_vset("program", VT_STRING, cp_program);

    sprintf(buf, "%s ! -> ", cp_program);
    cp_vset("prompt", VT_STRING, buf);

    cp_vset("noglob", VT_BOOL, (char *) &t);

    /* Now do a bunch of things that used to be in the spiceinit file
     * but were too slow to read in...
     */
    wl1.wl_next = &wl2;
    wl1.wl_prev = NULL;
    wl2.wl_next = NULL;
    wl2.wl_prev = &wl1;
    wl1.wl_word = "if";
    wl2.wl_word = "1";
    cp_setalias("begin", &wl1);
    wl1.wl_next = NULL;
    wl1.wl_word = "end";
    cp_setalias("endif", &wl1);
    cp_setalias("endwhile", &wl1);
    cp_setalias("endforeach", &wl1);
    cp_setalias("endrepeat", &wl1);
    cp_setalias("enddowhile", &wl1);
    wl1.wl_word = "help";
    cp_setalias("?", &wl1);

    wl1.wl_next = &wl2;
    wl2.wl_next = &wl3;
    wl2.wl_prev = &wl1;
    wl3.wl_prev = &wl2;
    wl3.wl_next = NULL;
    wl2.wl_word = "=";
    for (i = 0; i < sizeof (predefs) / sizeof (char *); i += 2) {
        wl1.wl_word = predefs[i];
        wl3.wl_word = predefs[i + 1];
        com_let(&wl1);
    }

    wl2.wl_next = NULL;
    for (i = 0; i < sizeof (udfs) / sizeof (char *); i += 2) {
        wl1.wl_word = udfs[i];
        wl2.wl_word = udfs[i + 1];
        com_define(&wl1);
    }

    if (Lib_Path && *Lib_Path) {
	cp_interactive = false;
	sprintf(buf, "sourcepath=( . %s )", Lib_Path);
	wl = cp_lexer(buf);
	com_set(wl);
	wl_free(wl);

	/* Now source the standard startup file. */
	sprintf(buf, "%s/spinit", Lib_Path);

	if ((fp = fopen(buf, "r"))) inp_spsource(fp, true, buf);
	else if (ft_controldb)
		fprintf(cp_err, "Note: can't open \"%s\".\n", buf);

	if (!fp) fprintf(cp_err, "Note: can't find init file.\n");
    }
}

/* Decide whether a condition is true or not. */

bool cp_istrue (wordlist *wl)
{
    int i;
    struct dvec *v;
    struct pnode *pn;

/* fprintf(stderr, "istrue: "); wl_print(wl, stderr); fprintf(stderr, "\n"); */
    /* First do all the csh-type stuff here... */
    wl = wl_copy(wl);
    wl = cp_variablesubst(wl);
    wl = cp_bquote(wl);
    cp_striplist(wl);

    pn = ft_getpnames(wl, true);
    v = ft_evaluate(pn);
    free_pnode(pn);

    /* It makes no sense to say while (all), but what the heck... */
    while (v) {
        if (isreal(v)) {
            for (i = 0; i < v->v_length; i++)
                if (v->v_realdata[i] != 0.0) return (true);
        } else {
            for (i = 0; i < v->v_length; i++)
                if ((realpart(&v->v_compdata[i]) != 0.0) ||
                    (imagpart(&v->v_compdata[i]) != 0.0)) return (true);
        }
        v = v->v_link2;
    }
    return (false);
}

/* This gets called before every command is executed... */

void cp_periodic()
{
    ft_setflag = ft_intrpt = false;
    ft_ckspace();
    ft_checkkids();
    vec_gc();
}

void cp_doquit()
{
    com_quit((wordlist *) NULL);
}

/* This is how we deal with emulation of commands by scripts... If the script
 * is found, then set the variables argc and argv and call the script.  Note
 * that this also allows us to type a filename to load a spice deck...
 */

bool cp_oddcomm (wordlist *wlist)
{
    FILE *fp;
    char buf[BSIZE_SP];
    wordlist *wl = wlist->wl_next;

    if ((fp = inp_pathopen(wlist->wl_word, "r"))) {
        fclose(fp);
        sprintf(buf, "argc=%d argv= ( ", wl_length(wl));
        while (wl) {
            strcat(buf, wl->wl_word);
            strcat(buf, " ");
            wl = wl->wl_next;
        }
        strcat(buf, ")");
	wl = cp_lexer(buf);
	com_set(wl);
	wl_free(wl);
        inp_source(wlist->wl_word);
        cp_remvar("argc");
        cp_remvar("argv");
        return (true);
    }
    if (wl && eq(wl->wl_word, "=")) {
        com_let(wlist);
        return (true);
    }
    return (false);
}
