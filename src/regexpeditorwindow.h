/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#ifndef regexpeditorwindow_h
#define regexpeditorwindow_h

#include <QWidget>

#include "widgetfactory.h"

class ConcWidget;
class QMenu;
class QHBoxLayout;

/** Widget representing the editor window of a regular expression editor.

    @internal
    This widget represent the editor part (That is the location where the
    regexp widgets are located).

    Furthermore, it also responsible for:
    @li Widget traversal operations (see e.g. @ref hasSelection, @ref
    clearSelection, and @ref applyRegExpToSelection ).
    @li Containing global information for regexp editing. (See @ref
    _pasteData and @ref _insertInAction )

    All subclasses of @ref RegExpWidget contains a pointer to the @ref
    RegExpEditorWindow which the widget is a child of. They use this
    pointer to start operations like rubber band selection, cut/paste etc.
*/
class RegExpEditorWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RegExpEditorWindow(QWidget *parent);
    ~RegExpEditorWindow() override;

    /**
       Returns an object which represent the regular expression "drawn" in
       the editor window. This object is capable of generating a textual
       representation of the regexp (see @ref RegExp::emacs, and @ref
       RegExp::perl etc)
    */
    RegExp *regExp() const;

    /**
       This method returns true if the rectangle starting at `globalPos' with
       size `size' overlaps the rubber-band rectangle.
       Note this method is only valid while doing rubber-band
       selection. Afterwards, use @ref pointSelected instead.
    */
    bool selectionOverlap(const QPointF &globalPos, QSize size) const;

    /**
       returns true if `pos' lays on top of a widget that is selected.
    */
    bool pointSelected(const QPointF &pos) const;

    /**
       returns true if the editor has a selection.
    */
    bool hasSelection() const;

    /**
       clears the selection, and if `update', invokes update on all the
       widgets
    */
    void clearSelection(bool update);

    /**
       invoked update on the top widget, and ensures that `focusChild' is
       visible. It's valid for `focusChild' to be 0.
    */
    void updateContent(QWidget *focusChild);

    RegExp *pasteData()
    {
        return _pasteData;
    }

    bool isPasteing() const
    {
        return _pasteInAction;
    }

    bool isInserting() const
    {
        return _insertInAction;
    }

    /**
       Returns the type currently being inserted.
       This is the type, which was given to @ref slotInsertRegExp
    */
    RegExpType insertType() const
    {
        return _insertTp;
    }

    /**
       Create a regexp widget, so that it wraps around the current selected
       regexp.
    */
    void applyRegExpToSelection(RegExpType tp);

    /**
       Pops up the RMB menu, which contains cut, copy, past, ...
    */
    void showRMBMenu(bool enableCutCopy);

    QSize sizeHint() const override;

public Q_SLOTS:

    /**
       Set the editor window to the regular expression given as argument
    */
    void slotSetRegExp(RegExp *regexp);

    /**
       Change editing mode to insertion. This means that the cursor will
       change to a cross (where ever inserting of the `type' is allowed),
       and the next mouse press in allowed places will result in that a
       regular expression of type `type' will be inserted there.

       Note this method do not do the actual insertion.

       This method is used when the user presses one of the buttons to the
       right of the editor window.
    */
    void slotInsertRegExp(RegExpType type);

    /**
       Change editing state to selection.
    */
    void slotDoSelect();

    /**
       Like @ref slotInsertRegExp above. This time, however,  data will be
       submitted in as a RegExp pointer.

       Note this method do not do the actual insertion.

       This method is called when the user pastes data (using the RPM menu),
       or when a regular expression is loaded from file.
    */
    void slotInsertRegExp(RegExp *regexp);

    /**
       see @ref RegExpWidget::deleteSelection
    */
    void slotDeleteSelection();

    /**
       This method is invoked when the user selects paste in the RMB menu. It
       will change the mouse cursor etc. as described in @ref
       slotInsertRegExp
    */
    void slotStartPasteAction();

    /**
       Ends an insert or paste action. This involves changing cursor back to
       normal cursor.
    */
    void slotEndActions();

    void emitChange()
    {
        Q_EMIT change();
    }

    void updateCursorUnderPoint();

    void slotCut();
    void slotCopy();
    void slotSave();

Q_SIGNALS:
    /**
       This signal is emitted whenever the content of the editor window is
       changed.

       If focusPoint is non-null then this point should be made visible
    */
    void contentChanged(QPointF focusPoint);

    /**
       This signal is emitted whenever mouse is being dragged in the editor
       window. `focusPoint' is the mouse' current position.
    */
    void scrolling(QPointF focusPoint);

    /**
       see @ref RegExpScrolledEditorWindow::doneEditing
    */
    void doneEditing();

    /**
       see @ref RegExpScrolledEditorWindow::change
    */
    void change();

    /**
       see @ref RegExpScrolledEditorWindow::savedRegexp
    */
    void savedRegexp();

    /**
       see @ref RegExpScrolledEditorWindow::verifyRegExp
    */
    void verifyRegExp();

    void anythingSelected(bool);
    void anythingOnClipboard(bool);
    void canSave(bool);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

protected Q_SLOTS:
    virtual void emitVerifyRegExp();
    void editWidget();

private:
    void cutCopyAux(QPointF pos);
    void copy(QPointF pos);
    void cut(QPointF pos);

private:
    /** This points to the top @ref RegExpWidget in the editor window. */
    ConcWidget *_top;

    /** This points to the layout manager for the editor window */
    QHBoxLayout *_layout;

    /** This points to the edit widget */
    QPointF _PosEdit;

    /** Start point and last point draw. Used when doing rubber band selection  */
    QPointF _start, _lastPoint;

    /** The area which the rubber band selection is over */
    QRectF _selection;

    /** true when a paste action is in action (see @ref isPasteing ). */
    bool _pasteInAction;

    /** true when an insert action is in action (see @ref isInserting ). */
    bool _insertInAction;

    /** The type being inserted (see @ref insertType ) */
    RegExpType _insertTp;

    /** The data being inserted (see @ref pasteData ) */
    RegExp *_pasteData = nullptr;

    /** Popup menu used for RMB */
    QMenu *_menu = nullptr;

    QAction *_cutAction = nullptr;
    QAction *_copyAction = nullptr;
    QAction *_pasteAction = nullptr;
    QAction *_editAction = nullptr;
    QAction *_saveAction = nullptr;

    QIcon getIcon(const QString &name);

    bool _isDndOperation;
};

#endif // regexpeditorwindow_h
