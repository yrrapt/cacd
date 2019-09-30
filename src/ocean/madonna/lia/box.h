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
#ifndef __BOX_H
#define __BOX_H

#ifndef __IOSTREAM_H
#include <iostream>
using namespace std;
#define __IOSTREAM_H
#endif

#ifndef __BASEDEFS_H
#include "src/ocean/madonna/lia/basedefs.h"
#endif

#ifndef __ITEM_H
#include "src/ocean/madonna/lia/item.h"
#endif

class BoxIterator;

class Box : public Item
{
public:
  Box() { itemsInBox = 0; }
  Box(const Box&);
  virtual ~Box() {};

  virtual BoxIterator& initIterator() const { return (BoxIterator&)NOITEM; };

  virtual classType myNo() const = 0;
  virtual char   *myName() const = 0;
  virtual int     isEqual(const Item&) const;
  virtual void    print(ostream&) const;
  virtual void    makeHeader(ostream&) const;
  virtual void    makeSeparator(ostream&) const;
  virtual void    makeFooter(ostream&) const;

  int             isEmpty() const { return itemsInBox == 0; }
  countType       getItemsInBox() const { return itemsInBox; }

protected:
  countType itemsInBox;

private:
  friend class BoxIterator;
};

class BoxIterator
{
public:
  BoxIterator() {};
  virtual ~BoxIterator();

  virtual         operator int() { return 0; };
  virtual         operator Item&() { return NOITEM; };
  virtual Item&   get() { return NOITEM; };
  virtual Item&   operator ++(int) = 0;
  virtual void    restart() = 0;
};

#endif
