/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
**********/

/*
 * Macros for complex mathematical functions.
 */

/* Some defines used mainly in cmath.c. */
#define alloc_c(len) ((complex *) tmalloc((len) * sizeof (complex)))
#define alloc_d(len) ((double  *) tmalloc((len) * sizeof (double)))
#define FTEcabs(d)  (((d) < 0.0) ? - (d) : (d))
#define cph(c)   (atan2(imagpart(c), (realpart(c))))
#define cmag(c)  (sqrt(imagpart(c) * imagpart(c) + realpart(c) * realpart(c)))
#define radtodeg(c) (cx_degrees ? ((c) / 3.14159265358979323846 * 180) : (c))
#define degtorad(c) (cx_degrees ? ((c) * 3.14159265358979323846 / 180) : (c))
#define rcheck(cond, name) if (!(cond)) { \
fprintf(cp_err, "Error: argument out of range for %s\n", name); return (NULL); }

#define cdiv(r1, i1, r2, i2, r3, i3) cx_cdiv(r1, i1, r2, i2, &r3, &i3)
extern void cx_cdiv(double, double, double, double, double*, double*);

