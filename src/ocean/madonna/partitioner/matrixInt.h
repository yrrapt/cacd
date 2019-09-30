/*
 * ISC License
 *
 * Copyright (C) 1991-2018 by
 *	Ireneusz Karkowski
 *	Patrick Groeneveld
 *	Paul Stravers
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
#ifndef __MATRIX_H
#define __MATRIX_H

enum Transposition { NotTransposed, Transposed };

// This implements a 2 dimensional integer matrix.

class matrixInt
{
private:
   int **thematrix;
   int dim1, dim2;
   void createAndInitialize (int dimension1, int dimension2);

public:
   matrixInt (int dimension1, int dimension2); // constructor
   matrixInt (matrixInt&);		      // copy operator
  ~matrixInt ();			      // destructor
   int* operator[] (int i) { return thematrix[i]; }
   int  dimension1 () { return dim1; }
   int  dimension2 () { return dim2; }
   matrixInt& exchangeRows (int a, int b);
   matrixInt& exchangeColumns (int a, int b);
   void print (Transposition = NotTransposed);
};

#endif
