/*
 *  Copyright (c) 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 **/

#include "kmultiformlistbox-multivisible.h"

#include <QBitmap>
#include <QVBoxLayout>
#include <QPushButton>

#include <KMessageBox>

#include "kmultiformlistboxfactory.h"
#include "indexWindow.h"
#include "ccp.h"

const int indexButtonWidth = 16;
const int indexButtonHeight = 12;
const uchar indexButtonBits[] = {
    0x00, 0x00, 0x00, 0x00, 0x0e, 0x02, 0x04, 0x02, 0x04, 0x02, 0xc4, 0x8a,
    0x24, 0x53, 0x14, 0x22, 0x14, 0x22, 0x24, 0x53, 0xce, 0x8a, 0x00, 0x00
};

KMultiFormListBoxMultiVisible::KMultiFormListBoxMultiVisible(KMultiFormListBoxFactory *fact, QWidget *parent)
    : QScrollArea(parent)
{
    factory = fact;

    // Initialize the element list
    elms = new WidgetList();

    QWidget *widget = new QWidget();
    layout = new QVBoxLayout(widget);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    setWidget(widget);
    setWidgetResizable(true);
}

//----------------------------------------------------------------------
// This function returns a list of the elements in the KMultiFormListBox widget.
//----------------------------------------------------------------------
KMultiFormListBoxEntryList KMultiFormListBoxMultiVisible::elements()
{
    KMultiFormListBoxEntryList res;
    foreach (QWidget *child, *elms) {
        if (child->objectName() != QStringLiteral("separator")) {
            res.append((KMultiFormListBoxEntry *)child);
        }
    }
    return res;
}

//----------------------------------------------------------------------
// This function is called whenever the KMultiFormListBox widget is resized. It is
// necessary to ensure that the content of the clipper is resized.
//----------------------------------------------------------------------
void KMultiFormListBoxMultiVisible::resizeEvent(QResizeEvent *e)
{
    // The call of the super class ensures that the outer border is updated.
    QScrollArea::resizeEvent(e);

    updateClipperContent();
}

void KMultiFormListBoxMultiVisible::updateClipperContent()
{
    // Extract the current size of the clipper
    /*
    int ClipperWidth = clipper()->size().width();
    int ClipperHeight = clipper()->size().height();

    // Initialize the calculation of the size of the new clipper.
    int totalHeight = 0;
    int maxWidth = ClipperWidth;
    int count = 0;

    // calculate the required size.
    foreach (QWidget *child , *elms) {
      maxWidth = qMax(maxWidth, child->sizeHint().width());
      if ( child->objectName() != "separator" ) {
        totalHeight += child->sizeHint().height();
        count++;
      }
      else {
        totalHeight += child->size().height();
      }
    }

    // Calculate the extra height for the elements.
    int extra = 0;
    if (totalHeight < ClipperHeight && count != 0) {
      extra = (ClipperHeight - totalHeight) / count;
      totalHeight = ClipperHeight;
    }

    // Now place the elements in the clipper.
    int yPos = 0;
    foreach (QWidget *child2 , *elms) {
      int h;
      if ( child2->objectName() != "separator" ) {
        h = child2->sizeHint().height();
        h += extra;
      }
      else {
        h = child2->size().height();
      }

      moveChild(child2, 0,yPos);

      child2->resize(maxWidth,h);
      yPos += h;
    }
    */

    // Finally call the resize procedure for the clipper to ensure that the
    // new sizes is shown properly.
    //resizeContents(maxWidth, totalHeight);
}

void KMultiFormListBoxMultiVisible::addElement()
{
    addElement(nullptr);
}

void KMultiFormListBoxMultiVisible::addElement(KMultiFormListBoxEntry *after)
{
    KMultiFormListBoxEntry *elm = factory->create(widget());
    insertElmIntoWidget(elm, after);
}

void KMultiFormListBoxMultiVisible::append(KMultiFormListBoxEntry *elm)
{
    elm->setParent(widget());
    insertElmIntoWidget(elm, nullptr);
}

void KMultiFormListBoxMultiVisible::delElement(QWidget *elm)
{
    int index = elms->indexOf(elm);
    QWidget *next = elms->at(index + 1);
    if (next->objectName() != QStringLiteral("separator")) {
        elms->removeOne(next);
        layout->removeWidget(next);
    }

    elms->removeOne(elm);
    layout->removeWidget(elm);

    updateClipperContent();
}

void KMultiFormListBoxMultiVisible::delAnElement()
{
    delElement(elms->at(0));
}

void KMultiFormListBoxMultiVisible::insertElmIntoWidget(KMultiFormListBoxEntry *elm, KMultiFormListBoxEntry *after)
{
    // Bind the index button if it exists.
    if (elm->indexButton()) {
        elm->indexButton()->setIcon(static_cast<QIcon>(QBitmap::fromData(QSize(indexButtonWidth, indexButtonHeight),
                                                                         indexButtonBits, QImage::Format_MonoLSB)));
        connect(elm->indexButton(), &QAbstractButton::clicked, elm, &KMultiFormListBoxEntry::acceptIndexButton);
        connect(elm, SIGNAL(gotoIndex(KMultiFormListBoxEntry*)),
                this, SLOT(showIndexList(KMultiFormListBoxEntry*)));
    }

    // Find the location to insert the new element.
    int index = elms->count();
    if (after) {
        index = elms->indexOf(after);
    }

    // Now show the new element.
    elms->insert(index, elm);
    layout->insertWidget(index, elm);
    elm->show();
    ensureWidgetVisible(elm, 0, 0);
    //addChild(elm,0,0); // updateClipperContent will place the child correctly.

    QWidget *sep = factory->separator(widget());
    if (sep != nullptr) {
        sep->setObjectName(QStringLiteral("separator"));
        sep->show();
        layout->insertWidget(index + 1, sep); // updateClipperContent will place the child correctly.
        elms->insert(index + 1, sep);
    }

    updateClipperContent();

    showWidget(elm); // scroll to show the new widget.

    // install cut'n'paste functionallity
    new CCP(this, elm);
}

//----------------------------------------------------------------------
// This function shows the list of available Idx elements.
//----------------------------------------------------------------------
void KMultiFormListBoxMultiVisible::showIndexList(KMultiFormListBoxEntry *elm)
{
    indexWindow *menu = new indexWindow();

    // Insert the elements into the menu item.
    foreach (QWidget *child, *elms) {
        if (child->objectName() != QStringLiteral("separator")) {
            QString txt = ((KMultiFormListBoxEntry *)child)->idxString();
            menu->insertItem(txt);
        }
    }

    // Calculate the location of the window
    QPoint start;
    int width;
    elm->indexWindowPos(&start, &width);

    // Show the window.
    int index = menu->exec(start, width);

    if (index != -1) {
        foreach (QWidget *child, *elms) {
            if (child->objectName() != QLatin1String("separator")) {
                if (index == 0) {
                    showWidget((KMultiFormListBoxEntry *)child);
                    break;
                }
                index--;
            }
        }
    }
    delete menu;
}

//----------------------------------------------------------------------
// Scroll to the loaction of the given KMultiFormListBoxEntry element.
//----------------------------------------------------------------------
void KMultiFormListBoxMultiVisible::showWidget(KMultiFormListBoxEntry *elm)
{
    ensureWidgetVisible(elm);
}

void KMultiFormListBoxMultiVisible::cut(KMultiFormListBoxEntry *elm)
{
    if (countElements(elms) == 1) {
        KMessageBox::information(this, i18n("Due to a bug, it is not possible to remove the last element."), i18n("Internal Error"));
        return;
    }

    QDataStream stream(&clipboard, QIODevice::WriteOnly);

    stream.setVersion(QDataStream::Qt_3_1);
    factory->toStream(elm, stream);
    delElement(elm);
}

void KMultiFormListBoxMultiVisible::copy(KMultiFormListBoxEntry *elm)
{
    QDataStream stream(&clipboard, QIODevice::WriteOnly);

    stream.setVersion(QDataStream::Qt_3_1);
    factory->toStream(elm, stream);
}

void KMultiFormListBoxMultiVisible::paste(KMultiFormListBoxEntry *oldElm)
{
    if (clipboard.isEmpty()) {
        KMessageBox::information(this, i18n("There is no element on the clipboard to paste in."));
        return;
    }

    KMultiFormListBoxEntry *newElm = factory->create(widget());
    QDataStream stream(&clipboard, QIODevice::ReadOnly);

    stream.setVersion(QDataStream::Qt_3_1);
    factory->fromStream(stream, newElm);
    insertElmIntoWidget(newElm, oldElm);
}

int KMultiFormListBoxMultiVisible::countElements(WidgetList *elms)
{
    int count = 0;

    foreach (QWidget *child, *elms) {
        if (dynamic_cast<const KMultiFormListBoxEntry *>(child)) {
            count++;
        }
    }

    return count;
}
