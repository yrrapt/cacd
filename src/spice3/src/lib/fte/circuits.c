/**********
Copyright 1990 Regents of the University of California. All rights reserved.
Author: 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
**********/

/*
 * Routines for dealing with the circuit database. This is currently
 * unimplemented.
 */

#include "spice.h"
#include "cpdefs.h"
#include "ftedefs.h"
#include "ftedata.h"

struct circ *ft_curckt = NULL; /* The default active circuit. */
struct circ *ft_circuits = NULL;

/* Add a circuit to the circuit list */

void ft_newcirc (struct circ *ckt)
{
    ckt->ci_next = ft_circuits;
    ft_circuits = ckt;
}
