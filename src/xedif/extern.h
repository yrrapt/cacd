/*
 * ISC License
 *
 * Copyright (C) 1994-2011 by
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

#ifdef __cplusplus
  extern "C" {
#endif

/* findFunc.c */
struct model_info *findFunc (char *name, int imported, DM_PROJECT *father_proj, int flag);

/* nextAttr.c */
char *nextAttr (char **p, char **v, char *a);
char *getAttrValue (char *a, char *par);

/* prFoot.c */
void prFoot (struct model_info *ntw, int submod);

/* prExt.c */
void prExt (void);

/* cirTree.c */
struct cir *cirTree (char *name);
void scanInst (char *name, DM_PROJECT *proj);
int is_func (void);
IMPCELL *dmGetImpCell (DM_PROJECT *proj, char *name, int msg);

/* findNetw.c */
struct model_info *findNetw (char *name, int imported, DM_PROJECT *father_proj, int submod);
struct model_info *newNetw (char *name, DM_PROJECT *proj, int imported, char *orig_name, int submod);
void readTerm (struct model_info *m, int funf, int submod);
void termRun (struct model_info *m, DM_STREAM *dsp);

/* findDev.c */
void Cds_initDevs (void);
void initDevs (void);
struct model_info *findDev (char *name, int imported, DM_PROJECT *proj);
char *findDevType (char *type);
void termstore (struct model_info *m, char *name, long dim, long *lower, long *upper, int type);
void printDevs (void);
char *strsave (char *s);

/* findNet.c */
void findNetInit (struct model_info *ntw, struct net_ref *nets);
struct net_el *findNet (struct net_el *eqv);

/* main.c */
int isCurrentDialect (char *buf);
void fatalErr (char *s1, char *s2);
void die (void);
void cannot_die (int nr, char *fn);
void prCdsTechnology (void);

/* prHead.c */
void prHead (struct model_info *ntw, int submod);

/* prInst.c */
void prInst (struct model_info *ntw, struct net_ref *nets);
int is_ap (void);

/* prNets.c */
void prNets (struct model_info *ntw, struct net_ref *nets);

/* print.c */
void nmprint (int concat, char *name, long dim, long *lower, long *upper, int pr_range);
void oprint (int concat, char *s);
int outPos (void);
char *projname (DM_PROJECT *proj);

/* xnetwork.c */
void xnetwork (char *ntwname, DM_PROJECT *proj, int imported, char *orig_name, int submod);
char *newStringSpace (char *s, int stock);
void resetStringSpace (int stock);
long *newIndexSpace (long cnt, int stock);
void resetIndexSpace (int stock);

/* ctrlFile.c */
void readControl (void);
char *maplibname (char *s);
void ctrlFileErr (char *s1, char *s2);

#ifdef __cplusplus
  }
#endif
