/* rcsid = "$Id: type.h,v 1.1 2018/04/30 12:17:53 simon Exp $" */
/*
 * ISC License
 *
 * Copyright (C) 1986-2018 by
 *	T. Vogel
 *	A.J. van Genderen
 *	S. de Graaf
 *	A.J. van der Hoeven
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

struct model_info {
    char name[DM_MAXNAME + 1];
    char orig_name[DM_MAXNAME + 1];
    DM_PROJECT *impproj;
    DM_PROJECT *proj;
    struct model_info *next;
};

struct cir {
    char name[DM_MAXNAME + 1];
    DM_PROJECT *proj;
    struct cir *cir_next;
    struct cir *cir_up;
};
