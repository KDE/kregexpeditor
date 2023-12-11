/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "concwidget.h"

#include <QPainter>
#include <QMouseEvent>

#include "concregexp.h"
#include "dragaccepter.h"

ConcWidget::ConcWidget(RegExpEditorWindow *editorWindow, QWidget *parent)
    : MultiContainerWidget(editorWindow, parent)
{
    init();
    DragAccepter *accepter = new DragAccepter(editorWindow, this);
    accepter->show();
    _children.append(accepter);
}

ConcWidget::ConcWidget(RegExpEditorWindow *editorWindow, RegExpWidget *child, QWidget *parent)
    : MultiContainerWidget(editorWindow, parent)
{
    init();
    DragAccepter *accepter = new DragAccepter(editorWindow, this);
    _children.append(accepter);
    child->setParent(this);
    addNewChild(accepter, child);
}

ConcWidget::ConcWidget(RegExpEditorWindow *editorWindow, ConcWidget *origConc, unsigned int start, unsigned int end)
    : MultiContainerWidget(editorWindow, nullptr)
{
    Q_UNUSED(start);
    Q_UNUSED(end);
    init();
    _children.prepend(new DragAccepter(editorWindow, this));
    QMutableListIterator<RegExpWidget *> i(origConc->_children);
    while (i.hasNext()) {
        RegExpWidget *child = i.next();
        i.remove();
        _children.prepend(child);
        child->setParent(this);
    }

    _children.prepend(new DragAccepter(editorWindow, this));
}

ConcWidget::ConcWidget(ConcRegExp *regexp, RegExpEditorWindow *editorWindow, QWidget *parent)
    : MultiContainerWidget(editorWindow, parent)
{
    init();
    DragAccepter *accepter = new DragAccepter(editorWindow, this);
    _children.append(accepter);

    const RegExpList list = regexp->children();
    for (RegExp *r : list) {
        RegExpWidget *child = WidgetFactory::createWidget(r, editorWindow, this);
        append(child);
    }
}

void ConcWidget::init()
{
    _maxSelectedHeight = 0;
}

QSize ConcWidget::sizeHint() const
{
    int childrenWidth = 0;
    int childrenHeight = 0;

    for (const RegExpWidget *regExpWidget : std::as_const(_children)) {
        QSize thisChildSize = regExpWidget->sizeHint();
        childrenWidth += thisChildSize.width();
        childrenHeight = qMax(childrenHeight, thisChildSize.height());
    }

    return QSize(childrenWidth, childrenHeight);
}

void ConcWidget::paintEvent(QPaintEvent *e)
{
    Q_ASSERT(dynamic_cast<DragAccepter *>(_children.at(0)));
    // if this fails, then I should check the location of the show()
    Q_ASSERT(_children.count() == 1
             || (_children.count() >= 3
                 && dynamic_cast<DragAccepter *>(_children.at(_children.count() - 1))));

    if (_children.count() == 1) {
        // There is only an accepter, lets give it all the space.
        _children.at(0)->setGeometry(0, 0, size().width(), size().height());
    } else {
        QSize myReqSize = sizeHint();
        QSize mySize(qMax(myReqSize.width(), size().width()),
                     qMax(myReqSize.height(), size().height()));

        // If the widget needs less space than it can get then this space should
        // be given to the leftmost and rightmost accepter. So lets calculate
        // this extra space.
        int extra = 0;
        if (size().width() > myReqSize.width()) {
            extra = (size().width() - myReqSize.width()) / 2;
        }

        QPainter painter(this);

        drawPossibleSelection(painter, mySize);

        int lastHeight = 0;
        int offset = 0;

        for (int i = 1; i < _children.count(); i += 2) {
            DragAccepter *accepter = dynamic_cast<DragAccepter *>(_children.at(i - 1));
            if (!accepter) {
                continue;
            }

            RegExpWidget *child = _children.at(i);

            QSize childSize = child->sizeHint();
            QSize curChildSize = child->size();

            //----------------------------- first place the accepter
            int x = offset;
            int w = accepter->sizeHint().width();
            if (i == 1) {
                w += extra;
            }

            int h = qMax(lastHeight, childSize.height());
            int y = (mySize.height() - h) / 2;
            accepter->setGeometry(x, y, w, h);

            offset += w;
            lastHeight = childSize.height();

            //------------------------------ Draw Accepter selection
            if (accepter->isSelected()) {
                y = (mySize.height() - _maxSelectedHeight) / 2;
                h = _maxSelectedHeight;
                painter.fillRect(x, y, w, h, QBrush(Qt::gray));
            }

            //-------------------------------------- place the child
            x = offset;
            h = childSize.height();
            w = childSize.width();
            y = (mySize.height() - h) / 2;
            child->setGeometry(x, y, w, h);
            if (childSize != curChildSize) {
                // I resized the child, so give it a chance to relect thus.
                child->update();
            }

            offset += w;

            //------------------------------ Draw Accepter selection
            if (child->isSelected()) {
                y = (mySize.height() - _maxSelectedHeight) / 2;
                h = _maxSelectedHeight;
                painter.fillRect(x, y, w, h, QBrush(Qt::gray));
            }
        }

        //---------------------- Finally place the last accepter.
        DragAccepter *accepter
            = static_cast<DragAccepter *>(_children.at(_children.count() - 1));
        // dynamic_cast is ASSERTed at top
        int x = offset;
        int h = lastHeight;
        int w = accepter->sizeHint().width() + extra;
        int y = (mySize.height() - h) / 2;
        accepter->setGeometry(x, y, w, h);
    }
    MultiContainerWidget::paintEvent(e);
}

void ConcWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        _editorWindow->showRMBMenu(_editorWindow->hasSelection());
    } else {
        RegExpWidget::mousePressEvent(event);
    }
}

void ConcWidget::sizeAccepter(DragAccepter *accepter, int height, int totHeight)
{
    if (accepter->height() != height) {
        accepter->resize(accepter->width(), height);
    }

    if (accepter->y() != (totHeight - height) / 2) {
        accepter->move(accepter->x(), (totHeight - height) / 2);
    }
}

RegExp *ConcWidget::regExp() const
{
    QList<RegExpWidget *>::const_iterator it = _children.constBegin();
    ++it; // start with the second element.

    if (_children.count() == 3) {
        // Exactly one child (and two drag accepters)
        return (*it)->regExp();
    } else {
        ConcRegExp *regexp = new ConcRegExp(isSelected());

        for (; it != _children.constEnd(); it += 2) {
            regexp->addRegExp((*it)->regExp());
        }
        return regexp;
    }
}

bool ConcWidget::updateSelection(bool parentSelected)
{
    bool isSel = _isSelected;
    bool changed = MultiContainerWidget::updateSelection(parentSelected);

    _maxSelectedHeight = 0;

    QList<RegExpWidget *>::const_iterator it = _children.constBegin();
    ++it; // Skip past the first DragAccepter
    for (; it != _children.constEnd(); it += 2) {
        if ((*it)->isSelected()) {
            _maxSelectedHeight = qMax(_maxSelectedHeight, (*it)->sizeHint().height());
        }
    }

    changed = changed || isSel != _isSelected;
    if (changed) {
        repaint();
    }

    return changed;
}

void ConcWidget::getSelectionIndexes(int *start, int *end)
{
    *start = -1;
    *end = -1;

    // Start with element at index 1, and skip every second element, as we
    // know they are dragAccepters.
    for (int index = 1; index < _children.count(); index += 2) {
        RegExpWidget *child = _children.at(index);

        if (child->isSelected()) {
            // The child is selected at topmost level.
            if (*start == -1) {
                *start = index;
            }
        } else if (*start != -1) {
            // Found the end of the selection.
            *end = index - 2;
            break;
        }
    }

    if (*start != -1 && *end == -1) {
        *end = _children.count() - 2;
    }
}

void ConcWidget::applyRegExpToSelection(RegExpType type)
{
    int start, end;
    getSelectionIndexes(&start, &end);

    if (start == -1) {
        // No item selected at top level

        QList<RegExpWidget * >::const_iterator it = _children.constBegin();
        ++it; // Skip past the first DragAccepter
        for (; it != _children.constEnd(); it += 2) {
            if ((*it)->hasSelection()) {
                (*it)->applyRegExpToSelection(type);
                break;
            }
        }
    } else {
        // Apply RegExp to selection.
        RegExpWidget *newElm = WidgetFactory::createWidget(_editorWindow, this, type);

        if (newElm) {
            ConcWidget *subSequence = new ConcWidget(_editorWindow, this, start, end);
            newElm->setConcChild(subSequence);

            subSequence->resize(0, 0); // see note (1)
            subSequence->setParent(newElm);
            _children.insert(start, newElm);
            newElm->show();
        }
    }
}

bool ConcWidget::isSelected() const
{
    // A ConcWidget should be considered selected when all its elements has been selected
    // otherwise empty ConcWidgets may be left behind when for example selection is deleted.
    bool allSelected = true;
    QList<RegExpWidget *>::const_iterator it = _children.constBegin();
    ++it; // Skip past first DragAccepter.
    for (; (it != _children.constEnd()) && allSelected; it += 2) {
        allSelected = (*it)->isSelected();
    }

    return allSelected;
}

RegExp *ConcWidget::selection() const
{
    if (isSelected()) {
        return regExp();
    }

    bool foundAny = false;
    bool foundMoreThanOne = false;
    RegExp *regexp = nullptr;

    QList<RegExpWidget * >::const_iterator it = _children.constBegin();
    ++it; // Skip past the first DragAccepter
    for (; it != _children.constEnd(); it += 2) {
        if ((*it)->hasSelection()) {
            if (!foundAny) {
                regexp = (*it)->selection();
                foundAny = true;
            } else if (!foundMoreThanOne) {
                ConcRegExp *reg = new ConcRegExp(isSelected());
                reg->addRegExp(regexp);
                reg->addRegExp((*it)->selection());
                regexp = reg;
                foundMoreThanOne = true;
            } else {
                dynamic_cast<ConcRegExp *>(regexp)->addRegExp((*it)->selection());
            }
        }
    }

    Q_ASSERT(foundAny);
    return regexp;
}

void ConcWidget::addNewConcChild(DragAccepter *accepter, ConcWidget *other)
{
    for (int i = 0; i < _children.count(); i += 2) {
        RegExpWidget *ch = _children.at(i);
        if (ch == accepter) {
            // Move all the element from the `child' ConcWidget to this one.
            // Do not copy the first one as this is a dragAccepter, and we place the widgets
            // after this drag accepter.
            // We must take them in pairs to avoid breaking the invariant for paintEvent,
            // namely that every second element is a dragAccepter
            for (unsigned int j = other->_children.count() - 1; j > 0; j -= 2) {
                RegExpWidget *newChildA = other->_children.takeAt(j);
                newChildA->setParent(this);
                _children.insert(i + 1, newChildA);
                RegExpWidget *newChildB = other->_children.takeAt(j - 1);
                newChildB->setParent(this);
                _children.insert(i + 1, newChildB);
                newChildA->show();
                newChildB->show();
            }
            delete other;
            return;
        }
    }
    qFatal("accepter not found");
}

bool ConcWidget::validateSelection() const
{
    bool cont = true;
    QList<RegExpWidget *>::const_iterator it = _children.constBegin();
    ++it; // skip past the DragAccepter.
    for (; (it != _children.constEnd()) && cont; it += 2) {
        cont = (*it)->validateSelection();
    }
    return cont;
}
