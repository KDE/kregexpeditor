/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *  SPDX-FileCopyrightText: 2011 Morten A. B. Sjøgren <m_abs@mabs.dk>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "multicontainerwidget.h"

#include "dragaccepter.h"

MultiContainerWidget::MultiContainerWidget(RegExpEditorWindow *editorWindow, QWidget *parent)
    : RegExpWidget(editorWindow, parent)
{
}

void MultiContainerWidget::append(RegExpWidget *child)
{
    child->setParent(this);
    _children.append(child);
    _children.append(new DragAccepter(_editorWindow, this));
}

bool MultiContainerWidget::hasSelection() const
{
    if (_isSelected) {
        return true;
    }

    QList<RegExpWidget *>::const_iterator it = _children.constBegin();
    ++it; // Move past the first dragAccepter
    for (; it != _children.constEnd(); it += 2) {
        if ((*it)->hasSelection()) {
            return true;
        }
    }
    return false;
}

void MultiContainerWidget::clearSelection()
{
    _isSelected = false;
    for (int i = 0; i < _children.count(); i++) {
        _children.at(i)->clearSelection();
    }
}

void MultiContainerWidget::deleteSelection()
{
    // run from the back to the front (which we do since we delete items on the run)
    // When deleting children, delete the drag accepter to its right.
    for (int i = (int)_children.count() - 2; i > 0; i -= 2) {
        RegExpWidget *child = _children.at(i);
        if (child->isSelected()) {
            delete _children.at(i + 1);
            _children.removeAt(i + 1);
            delete child;
            _children.removeAt(i);
        } else if (child->hasSelection()) {
            child->deleteSelection();
        }
    }
    _isSelected = false;
    update();
}

void MultiContainerWidget::addNewChild(DragAccepter *accepter, RegExpWidget *child)
{
    for (int i = 0; i < _children.count(); i += 2) {
        RegExpWidget *ch = _children.at(i);
        if (ch == accepter) {
            // Insert the new child
            _children.insert(i + 1, child);

            // Insert an accepter as the next element.
            DragAccepter *accepter = new DragAccepter(_editorWindow, this);

            _children.insert(i + 2, accepter);

            // These two show's must come here otherwise a paintevent
            // will be invoked, where the invariant, that a accepter is located at
            // every second element.
            child->show();
            accepter->show();

            update();

            return;
        }
    }
    qFatal("Accepter not found in list");
}

bool MultiContainerWidget::updateSelection(bool parentSelected)
{
    bool changed = false;
    bool isSel = _isSelected;
    QVector<bool> oldState(_children.count());
    QVector<bool> newState(_children.count());

    for (int i = 0; i < (int)_children.count(); i++) {
        oldState[i] = _children.at(i)->isSelected();
    }

    RegExpWidget::updateSelection(parentSelected);

    int first;
    int last;

    // scan for the first selected item.
    for (first = 1; first < (int)_children.count(); first += 2) {
        RegExpWidget *child = _children.at(first);
        changed = child->updateSelection(_isSelected) || changed;
        newState[first] = child->isSelected();
        if (child->isSelected()) {
            break;
        }
    }

    // scan for the last selected item
    for (last = _children.count() - 2; last > first; last -= 2) {
        RegExpWidget *child = _children.at(last);
        changed = child->updateSelection(_isSelected) || changed;
        newState[last] = child->isSelected();
        if (child->isSelected()) {
            break;
        }
    }

    // everything between first and last must be selected.
    for (int j = first + 2; j < last; j += 2) {
        RegExpWidget *child = _children.at(j);

        changed = child->updateSelection(true) || changed;
        newState[j] = true;
    }

    // update drag accepters.
    for (int k = 0; k < (int)_children.count(); k += 2) {
        RegExpWidget *child = _children.at(k);
        bool select;
        if (k == 0 || k == (int)_children.count() - 1) {
            // The elements at the border is only selected if the parent is selected.
            select = _isSelected;
        } else {
            // Drag accepters in the middle is selected if the elements at both
            // sides are selected.
            select = newState[k - 1] && newState[k + 1];
        }

        bool isChildSel = child->isSelected();
        DragAccepter *accepter = dynamic_cast<DragAccepter *>(child);
        if (accepter) {
            accepter->_isSelected = select;
        }

        if (select != isChildSel) {
            child->repaint();
        }
    }

    changed = changed || isSel != _isSelected;
    if (changed) {
        repaint();
    }

    return changed;
}

QRect MultiContainerWidget::selectionRect() const
{
    if (_isSelected) {
        return QRect(mapToGlobal(QPoint(0, 0)), size());
    } else {
        QRect res;
        QList<RegExpWidget *>::const_iterator it = _children.constBegin();
        ++it; // Move past the first dragAccepter
        for (; it != _children.constEnd(); it += 2) {
            if ((*it)->hasSelection()) {
                QRect childSel = (*it)->selectionRect();
                if (res.isNull()) {
                    res = childSel;
                } else {
                    QRect newRes;
                    newRes.setLeft(qMin(res.left(), childSel.left()));
                    newRes.setTop(qMin(res.top(), childSel.top()));
                    newRes.setRight(qMax(res.right(), childSel.right()));
                    newRes.setBottom(qMax(res.bottom(), childSel.bottom()));
                    res = newRes;
                }
            }
        }
        return res;
    }
}

RegExpWidget *MultiContainerWidget::widgetUnderPoint(QPointF globalPos, bool justVisibleWidgets)
{
    int start, incr;
    if (justVisibleWidgets) {
        start = 1;
        incr = 2;
    } else {
        start = 0;
        incr = 1;
    }

    for (int i = start; i < _children.count(); i += incr) {
        RegExpWidget *wid = _children.at(i)->widgetUnderPoint(globalPos, justVisibleWidgets);
        if (wid) {
            return wid;
        }
    }

    if (justVisibleWidgets) {
        return nullptr;
    } else {
        return RegExpWidget::widgetUnderPoint(globalPos, justVisibleWidgets);
    }
}

RegExpWidget *MultiContainerWidget::findWidgetToEdit(QPointF globalPos)
{
    for (int i = 1; i < _children.count(); i += 2) {
        RegExpWidget *wid = _children.at(i)->findWidgetToEdit(globalPos);
        if (wid) {
            return wid;
        }
    }
    return nullptr;
}

void MultiContainerWidget::selectWidget(bool sel)
{
    RegExpWidget::selectWidget(sel);
    QList<RegExpWidget *>::const_iterator it = _children.constBegin();
    for (; it != _children.constEnd(); ++it) {
        (*it)->selectWidget(sel);
    }
    update();
}

void MultiContainerWidget::updateAll()
{
    QList<RegExpWidget *>::const_iterator it = _children.constBegin();
    for (; it != _children.constEnd(); ++it) {
        (*it)->updateAll();
    }
    RegExpWidget::updateAll();
}

void MultiContainerWidget::updateCursorRecursively()
{
    QList<RegExpWidget *>::const_iterator it = _children.constBegin();
    for (; it != _children.constEnd(); ++it) {
        (*it)->updateCursorRecursively();
    }
    updateCursorShape();
}
