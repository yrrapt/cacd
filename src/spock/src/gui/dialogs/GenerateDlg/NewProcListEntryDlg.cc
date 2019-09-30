/*
 * ISC License
 *
 * Copyright (C) 2000-2018 by
 *	Xander Burgerhout
 *	Simon de Graaf
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

#include "src/spock/src/gui/dialogs/GenerateDlg/NewProcListEntryDlg.h"

#include <qlayout.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include <qlabel.h>

#include <stdlib.h>

using namespace std;

CNewProcListEntryDlg::CNewProcListEntryDlg(QWidget* parent, const char* name)
    : QDialog(parent, name, true)
{
    setCaption ("New processlist entry");
    m_layout = new QVBoxLayout(this, 8, 8);
    m_layout->setAutoAdd(true);

    new QLabel("Enter a new process number:", this);
    m_editNr = new QLineEdit(this);

    new QLabel("Choose your process numbers carefully!!", this);

    QPushButton* okBut = new QPushButton("OK", this);
    okBut->setAutoDefault(true);
    connect(okBut, SIGNAL(clicked()), this, SLOT(accept()));

    QPushButton* cancelBut = new QPushButton("Cancel", this);
    connect(cancelBut, SIGNAL(clicked()), this, SLOT(reject()));
}

int CNewProcListEntryDlg::getNr()
{
    QString tmp = m_editNr->text();
    if (tmp.isNull())
        return -1;
    return atoi(tmp.latin1());
}
