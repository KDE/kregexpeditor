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
 *  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *  Boston, MA 02111-1307, USA.
 **/

#ifdef QT_ONLY
  #include "compat.h"
  #include <qmessagebox.h>
//Added by qt3to4:
#include <QResizeEvent>
#else
  #include <kmessagebox.h>
  #include "kmultiformlistbox-multivisible.moc"
#endif

#include "kmultiformlistbox-multivisible.h"
#include "indexWindow.h"
#include "ccp.h"

#include <qbitmap.h>

const int indexButtonWidth = 16;
const int indexButtonHeight = 12;
const uchar indexButtonBits[] = {
	0x00, 0x00, 0x00, 0x00, 0x0e, 0x02, 0x04, 0x02, 0x04, 0x02, 0xc4, 0x8a,
	0x24, 0x53, 0x14, 0x22, 0x14, 0x22, 0x24, 0x53, 0xce, 0x8a, 0x00, 0x00
};


KMultiFormListBoxMultiVisible::KMultiFormListBoxMultiVisible(KMultiFormListBoxFactory *fact, QWidget *parent, const char *name)
  : Q3ScrollView(parent, name)
{
  factory = fact;

  // Initialize the element list
  elms = new WidgetList();

  // Initialize the clipper.
  enableClipper(true);
  resizeContents(50,50); // This is required for proper functionality
}



//----------------------------------------------------------------------
// This function returns a list of the elements in the KMultiFormListBox widget.
//----------------------------------------------------------------------
KMultiFormListBoxEntryList KMultiFormListBoxMultiVisible::elements()
{
  KMultiFormListBoxEntryList res;
  for (QWidget *child = elms->first(); child; child=elms->next()) {
    if (strcmp(child->name(),"seperator") != 0) {
      res.append((KMultiFormListBoxEntry *) child);
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
  Q3ScrollView::resizeEvent(e);

  updateClipperContent();
}

void KMultiFormListBoxMultiVisible::updateClipperContent()
{
  // Extract the current size of the clipper
  int ClipperWidth = clipper()->size().width();
  int ClipperHeight = clipper()->size().height();

  // Initialize the calculation of the size of the new clipper.
  int totalHeight = 0;
  int maxWidth = ClipperWidth;
  int count = 0;


  // calculate the required size.
  for (QWidget *child = elms->first(); child; child=elms->next()) {
    maxWidth = QMAX(maxWidth, child->sizeHint().width());
    if (strcmp(child->name(), "seperator") != 0) {
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
  for (QWidget *child2 = elms->first(); child2; child2=elms->next()) {
    int h;
    if ( strcmp(child2->name(),"seperator") != 0) {
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

  // Finally call the resize procedure for the clipper to ensure that the
  // new sizes is shown properly.
  resizeContents(maxWidth, totalHeight);
}


void KMultiFormListBoxMultiVisible::addElement()
{
  addElement(0);
}

void KMultiFormListBoxMultiVisible::addElement(KMultiFormListBoxEntry *after)
{
  KMultiFormListBoxEntry *elm = factory->create(viewport());
  insertElmIntoWidget(elm, after);
}

void KMultiFormListBoxMultiVisible::append(KMultiFormListBoxEntry *elm)
{
  elm->reparent(viewport(), 0, QPoint(0,0), false);
  insertElmIntoWidget(elm, 0);
}

void KMultiFormListBoxMultiVisible::delElement(QWidget *elm)
{
  int index = elms->find(elm);
  QWidget *next = elms->at(index+1);
  if (strcmp(next->name(),"seperator") != 0) {
    elms->removeRef(next);
    removeChild(next);
  }

  elms->removeRef(elm);
  removeChild(elm);

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
    elm->indexButton()->setPixmap(QBitmap(indexButtonWidth, indexButtonHeight,
																					indexButtonBits, true));
    connect(elm->indexButton(), SIGNAL(clicked()), elm, SLOT(acceptIndexButton()));
    connect(elm, SIGNAL(gotoIndex(KMultiFormListBoxEntry *)),
            this, SLOT(showIndexList(KMultiFormListBoxEntry *)));
  }

  // Find the location to insert the new element.
  int index = elms->count();
  if (after) {
    index = elms->findRef(after);
  }

  // Now show the new element.
  elms->insert(index, elm);
  elm->show();
  addChild(elm,0,0); // updateClipperContent will place the child correctly.

  QWidget *sep = factory->separator(viewport());
  if (sep != 0) {
    sep->setName("seperator");
    sep->show();
    addChild(sep,0,0); // updateClipperContent will place the child correctly.
    elms->insert(index+1, sep);
  }

  updateClipperContent();

  showWidget(elm); // scroll to show the new widget.

  // install cut'n'paste functionallity
  new CCP(this,elm);
}


//----------------------------------------------------------------------
// This function shows the list of available Idx elements.
//----------------------------------------------------------------------
void KMultiFormListBoxMultiVisible::showIndexList(KMultiFormListBoxEntry *elm)
{
  indexWindow *menu = new indexWindow();

  // Insert the elements into the menu item.
  for (QWidget *child = elms->first(); child; child=elms->next()) {
    if ( strcmp(child->name(), "seperator") != 0) {
      QString txt = ((KMultiFormListBoxEntry *) child)->idxString();
      menu->insertItem(txt);
    }
  }

  // Calculate the location of the window
  QPoint start;
  int width;
  elm->indexWindowPos(&start, &width);

  // Show the window.
  int index = menu->exec(start,width);

  if (index != -1) {
    for (QWidget *child = elms->first(); child; child=elms->next()) {
      if ( strcmp(child->name(), "seperator") != 0) {

        if (index == 0) {
          showWidget((KMultiFormListBoxEntry *) child);
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
  setContentsPos(childX(elm), childY(elm));
}


void KMultiFormListBoxMultiVisible::cut(KMultiFormListBoxEntry *elm)
{
	if (countElements(elms) == 1) {
		KMessageBox::information(this, i18n("Due to a bug, it is not possible to remove the last element."), i18n("Internal Error") );
		return;
	}

  QDataStream stream(&clipboard, QIODevice::WriteOnly);


  stream.setVersion(QDataStream::Qt_3_1);
  factory->toStream( elm, stream );
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
  if (clipboard.size() == 0) {
    KMessageBox::information(this, i18n("There is no element on the clipboard to paste in."));
    return;
  }

  KMultiFormListBoxEntry *newElm = factory->create(viewport());
  QDataStream stream( &clipboard, QIODevice::ReadOnly );

  stream.setVersion(QDataStream::Qt_3_1);
  factory->fromStream(stream, newElm);
  insertElmIntoWidget(newElm,oldElm);
}


int KMultiFormListBoxMultiVisible::countElements(WidgetList *elms)
{
  int count = 0;

  for (QWidget *child = elms->first(); child; child=elms->next()) {
    if (dynamic_cast<const KMultiFormListBoxEntry *>(child))
      count++;
  }

  return count;
}


