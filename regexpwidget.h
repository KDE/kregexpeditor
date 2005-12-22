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
#ifndef __regexpwidget
#define __regexpwidget

#include "editorwindow.h"

class DragAccepter;
class RegExp;
class ConcWidget;
class RegExpEditorWindow;

/**
   Base class for all regular expression widgets.
   @internal
*/
class RegExpWidget :public QWidget
{
Q_OBJECT

public:
  RegExpWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
               const char *label = 0);

  /*======================================================================
     Construction, child management
     ======================================================================*/
  /**
     Add `child' to the parent of this widget in place of `accepter'
     This method only applies to container widgets (see @ref
     SingleContainerWidget, and @ref MultiContainerWidget).
  */
  virtual void addNewChild(DragAccepter *accepter, RegExpWidget *child);

  /**
     Inserts all the children of `child' into this widget in place of
     `accepter'.
     This method only applies to container widgets (see @ref
     SingleContainerWidget, and @ref MultiContainerWidget).
  */
  virtual void addNewConcChild(DragAccepter *accepter, ConcWidget *child);

  /**
     Set `child' as the ConcWidget. This method should only be invoked when
     the widget has just been constructed.
     This method only applies to container widgets (see @ref
     SingleContainerWidget, and @ref MultiContainerWidget).
  */
  virtual void setConcChild(ConcWidget *child);

  /* ======================================================================
     Selection
     ======================================================================*/

  /**
     Marks this widget as being the immediate child of the editor
     window. That is the topmost regular expression widget.

     This information is necessary when drawing the widget, and when
     drawing the selection.
  */
  void setToplevel() { _isToplevel = true; }

  /**
     Returns true if this widget is selected.
  */
  virtual bool isSelected() const;

  /**
     Returns true if this widget or one of its children is selected.
  */
  virtual bool hasSelection() const;

  virtual void updateAll();

  /**
     Update selection information for this widget.
     @param parentSelected indicates whether the parent is selected.
     @return true if the selection state has changed for the widget since
     the last time the widget was painted. `repaint' is invoked on the widget if
     selection state is changed, to ensure that selection is visible.
  */
  virtual bool updateSelection(bool parentSelected);

  /**
     Clears the selection and repaints the widget if `update' is true.
  */
  virtual void clearSelection();

  /**
     Deletes the regexp widgets containing selection.
  */
  virtual void deleteSelection();

  /**
     See @ref RegExpEditorWindow::applyRegExpToSelection
  */
  virtual void applyRegExpToSelection( RegExpType type );

  /**
     Validate that selection is OK. Returns false if not.

     Only one alternative may be selected in @ref AltnWidget. This
     limitation is to make implementation of a number of functions simpler.
  */
  virtual bool validateSelection() const { return true; }

  /**
     Returns the rectangle which makes up the selection.
  */
  virtual QRect selectionRect() const;

  /** Selects the RegExp widget and all its chidlren*/
  virtual void selectWidget( bool sel );

  /*======================================================================
    Misc
    ======================================================================*/

  /** Returns the RegExp widget, which is rooted in this regexp widget. */
	virtual RegExp* regExp() const = 0;

  /** returns the current selection */
  virtual RegExp* selection() const;

  /** returns the type of this regular expression widget. */
  virtual RegExpType type() const = 0;

  /**
     updates the cursor to be one of
     @li normal selection cursor
     @li accept cursor for insert/paste
     @li reject cursor for insert/paste
  */
  void updateCursorShape();

  virtual void updateCursorRecursively();

  /**
     Returns the regexp widget under point. If `justVisibleWidgets' is
     true, @ref ConcWidget is ignored. That is, a ConcWidget will not be returned.
  */
  virtual RegExpWidget* widgetUnderPoint( QPoint globalPos, bool justVisibleWidgets );

  /**
     Returns the widget under point which can be edited. That is the
     "innermost" widget editable.
  */
  virtual RegExpWidget* findWidgetToEdit( QPoint /* globalPos */) { return 0; }

  /**
     Edits the current widget. That is, bring up the dialog, which is
     available for @ref ConcWidget, @ref CompoundWidget, and @ref
     CharactersWidget.
  */
  virtual int edit();

protected:
  /** Draws a selection rectangle for the current widget.  */
  virtual void drawPossibleSelection( QPainter& painter, QSize mySize);

  /** Returns true if the widget accepts paste actions. */
  virtual bool acceptWidgetPaste() const;

  /**
     Returns true if the widget accept the given RegExpType to be inserted
   */
  virtual bool acceptWidgetInsert( RegExpType ) const;

  virtual void mousePressEvent ( QMouseEvent* event );

  virtual void mouseReleaseEvent( QMouseEvent* );
  virtual void enterEvent( QEvent * );

  RegExpEditorWindow* _editorWindow;

  /** True if the widget is selected. */
  bool _isSelected;

  /** See @ref setToplevel */
  bool _isToplevel;

  static const int pw;      // width of pen
  static const int bdSize;  // width between borders and text etc.
  static const int space;   // TODO.
};

#endif // __regexpwidget
