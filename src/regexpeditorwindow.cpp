/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "regexpeditorwindow.h"

#include <QApplication>
#include <QClipboard>
#include <QDrag>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMenu>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QShortcut>
#include <QTextStream>

#include <KLocalizedString>
#include <KMessageBox>
#include <QIcon>

#include "concwidget.h"
#include "regexp.h"
#include "regexpconverter.h"
#include "userdefinedregexps.h"

RegExpEditorWindow::RegExpEditorWindow(QWidget *parent)
    : QWidget(parent /*, Qt::WPaintUnclipped*/)
{
    _top = new ConcWidget(this, this);
    _layout = new QHBoxLayout(this);
    _layout->addWidget(_top);
    _top->setToplevel();
    _menu = nullptr;
    _insertInAction = false;
    _pasteInAction = false;
    _pasteData = nullptr;

    _PosEdit = QPoint(0, 0);

    (void)new QShortcut(Qt::CTRL | Qt::Key_C, this, SLOT(slotCopy()));
    (void)new QShortcut(Qt::CTRL | Qt::Key_X, this, SLOT(slotCut()));
    (void)new QShortcut(Qt::Key_Delete, this, SLOT(slotCut()));
    (void)new QShortcut(Qt::Key_Backspace, this, SLOT(slotCut()));
    (void)new QShortcut(Qt::CTRL | Qt::Key_V, this, SLOT(slotStartPasteAction()));
    (void)new QShortcut(Qt::Key_Escape, this, SLOT(slotEndActions()));
    (void)new QShortcut(Qt::CTRL | Qt::Key_S, this, SLOT(slotSave()));

    connect(this, &RegExpEditorWindow::change, this, &RegExpEditorWindow::emitVerifyRegExp);
}

RegExpEditorWindow::~RegExpEditorWindow()
{
    delete _pasteData;
}

RegExp *RegExpEditorWindow::regExp() const
{
    return _top->regExp();
}

void RegExpEditorWindow::mousePressEvent(QMouseEvent *event)
{
    setFocus();
    updateContent(nullptr);

    _start = event->pos();
    _lastPoint = QPoint(0, 0);

    if (pointSelected(event->globalPosition())) {
        _isDndOperation = true;
    } else {
        _isDndOperation = false;
        _selection = QRect();
        _top->updateSelection(false);

        QWidget::mousePressEvent(event);
    }
    grabMouse();
}

bool RegExpEditorWindow::pointSelected(const QPointF &p) const
{
    QRectF rect = _top->selectionRect();
    return rect.contains(p);
}

void RegExpEditorWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (_isDndOperation) {
        if ((_start - event->pos()).manhattanLength() > QApplication::startDragDistance()) {
            RegExp *regexp = _top->selection();
            if (!regexp) {
                return;
            }

            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData;

            mimeData->setText(RegExpConverter::current()->toStr(regexp, false));
            mimeData->setData(QStringLiteral("KRegExpEditor/widgetdrag"), regexp->toXmlString().toLocal8Bit());
            delete regexp;

            drag->setMimeData(mimeData);

            Qt::DropAction dropAction = drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction);
            if (dropAction == Qt::MoveAction) {
                slotDeleteSelection();
            } else {
                clearSelection(true);
            }

            releaseMouse();
            Q_EMIT change();
            Q_EMIT canSave(_top->hasAnyChildren());
        }
    } else {
        _top->updateSelection(false);

        Q_EMIT scrolling(event->pos());

        _lastPoint = event->pos();

        update();

        _selection = QRectF(mapToGlobal(_start), mapToGlobal(_lastPoint)).normalized();
    }
}

void RegExpEditorWindow::mouseReleaseEvent(QMouseEvent *event)
{
    releaseMouse();
    QWidget::mouseReleaseEvent(event);

    _lastPoint = QPoint(0, 0);

    _top->validateSelection();
    _top->updateAll();
    Q_EMIT anythingSelected(hasSelection());
    if (hasSelection()) {
        Q_EMIT verifyRegExp();
    }
}

bool RegExpEditorWindow::selectionOverlap(const QPointF &pos, QSize size) const
{
    QRectF child(pos, size);

    return _selection.intersects(child) && !child.contains(_selection);
}

bool RegExpEditorWindow::hasSelection() const
{
    return _top->hasSelection();
}

void RegExpEditorWindow::clearSelection(bool update)
{
    _top->clearSelection();
    if (update) {
        _top->updateAll();
    }
    Q_EMIT anythingSelected(false);
}

void RegExpEditorWindow::slotInsertRegExp(RegExpType type)
{
    _insertInAction = true;
    _insertTp = type;

    updateCursorUnderPoint();
    setFocus();
}

void RegExpEditorWindow::slotInsertRegExp(RegExp *regexp)
{
    delete _pasteData;

    _pasteData = regexp->clone();
    _pasteInAction = true;
    updateCursorUnderPoint();
    setFocus();
}

void RegExpEditorWindow::slotDoSelect()
{
    _pasteInAction = false;
    _insertInAction = false;

    // I need to update the cursor recursively, as a repaint may not have been issued yet
    // when this method is invoked. This means that when the repaint comes, the cursor may
    // move to an other widget.
    _top->updateCursorRecursively();
}

void RegExpEditorWindow::slotDeleteSelection()
{
    if (!hasSelection()) {
        KMessageBox::information(this, i18n("There is no selection."), i18n("Missing Selection"));
    } else {
        _top->deleteSelection();
    }
    updateContent(nullptr);
}

void RegExpEditorWindow::updateContent(QWidget *focusChild)
{
    QPoint p(0, 0);
    if (focusChild) {
        p = focusChild->mapTo(this, QPoint(0, 0));
    }

    _top->update();
    Q_EMIT contentChanged(p);
}

QSize RegExpEditorWindow::sizeHint() const
{
    return _top->sizeHint();
}

void RegExpEditorWindow::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    p.setPen(Qt::DotLine);

    if (!_lastPoint.isNull()) {
        p.drawRect(QRectF(_start, _lastPoint));
    }

    QWidget::paintEvent(event);
}

void RegExpEditorWindow::slotCut()
{
    cut(QCursor::pos());
    Q_EMIT change();
    Q_EMIT canSave(_top->hasAnyChildren());
}

void RegExpEditorWindow::cut(QPointF pos)
{
    cutCopyAux(pos);
    slotDeleteSelection();
}

void RegExpEditorWindow::slotCopy()
{
    copy(QCursor::pos());
}

void RegExpEditorWindow::copy(QPointF pos)
{
    cutCopyAux(pos);
    clearSelection(true);
}

void RegExpEditorWindow::cutCopyAux(QPointF pos)
{
    if (!hasSelection()) {
        RegExpWidget *widget = _top->widgetUnderPoint(pos, true);
        if (!widget) {
            KMessageBox::information(this, i18n("There is no widget under cursor."), i18n("Invalid Operation"));
            return;
        } else {
            widget->updateSelection(true); // HACK!
        }
    }

    RegExp *regexp = _top->selection();

    QMimeData *mimeData = new QMimeData;
    mimeData->setText(RegExpConverter::current()->toStr(regexp, false));
    mimeData->setData(QStringLiteral("KRegExpEditor/widgetdrag"), regexp->toXmlString().toLocal8Bit());

    delete regexp;

    QClipboard *clipboard = qApp->clipboard();
    clipboard->setMimeData(mimeData);
    Q_EMIT anythingOnClipboard(true);
    Q_EMIT canSave(_top->hasAnyChildren());
}

void RegExpEditorWindow::slotStartPasteAction()
{
    QString str = QString::fromLatin1(qApp->clipboard()->mimeData()->data(QStringLiteral("KRegExpEditor/widgetdrag")));
    if (str.isEmpty()) {
        return;
    }

    RegExp *regexp = WidgetFactory::createRegExp(str);
    if (regexp) {
        slotInsertRegExp(regexp);
    }
}

void RegExpEditorWindow::slotEndActions()
{
    Q_EMIT doneEditing();
    Q_EMIT change();
    Q_EMIT canSave(_top->hasAnyChildren());
}

void RegExpEditorWindow::showRMBMenu(bool enableCutCopy)
{
    enum CHOICES {
        CUT,
        COPY,
        PASTE,
        SAVE,
        EDIT
    };

    if (!_menu) {
        _menu = new QMenu(nullptr);

        _cutAction = _menu->addAction(getIcon(QStringLiteral("edit-cut")), i18n("C&ut"));
        connect(_cutAction, &QAction::triggered, this, &RegExpEditorWindow::slotCut);

        _copyAction = _menu->addAction(getIcon(QStringLiteral("edit-copy")), i18n("&Copy"));
        connect(_copyAction, &QAction::triggered, this, &RegExpEditorWindow::slotCopy);

        _pasteAction = _menu->addAction(getIcon(QStringLiteral("edit-paste")), i18n("&Paste"));
        connect(_pasteAction, &QAction::triggered, this, &RegExpEditorWindow::slotStartPasteAction);

        _menu->addSeparator();

        _editAction = _menu->addAction(getIcon(QStringLiteral("document-properties")), i18n("&Edit"));
        connect(_editAction, &QAction::triggered, this, &RegExpEditorWindow::editWidget);

        _saveAction = _menu->addAction(getIcon(QStringLiteral("document-save")), i18n("&Save Regular Expression..."));
        connect(_saveAction, &QAction::triggered, this, &RegExpEditorWindow::slotSave);
    }

    _cutAction->setEnabled(enableCutCopy);
    _copyAction->setEnabled(enableCutCopy);

    if (!qApp->clipboard()->mimeData()->hasFormat(QStringLiteral("KRegExpEditor/widgetdrag"))) {
        _pasteAction->setEnabled(false);
    } else {
        _pasteAction->setEnabled(true);
    }

    _saveAction->setEnabled(_top->hasAnyChildren());

    _PosEdit = QCursor::pos();

    RegExpWidget *editWidget = _top->findWidgetToEdit(_PosEdit);

    _editAction->setEnabled(editWidget);

    _menu->exec(_PosEdit.toPoint());

    _PosEdit = QPoint(0, 0);

    Q_EMIT change();
    Q_EMIT canSave(_top->hasAnyChildren());
}

void RegExpEditorWindow::applyRegExpToSelection(RegExpType tp)
{
    _top->applyRegExpToSelection(tp);
}

void RegExpEditorWindow::slotSave()
{
    QString dir = WidgetWinItem::path();
    QString txt;

    bool ok = false;
    const QString tmp = QInputDialog::getText(this, i18n("Name for Regular Expression"), i18n("Enter name:"), QLineEdit::Normal, QString(), &ok);
    if (!ok) {
        return;
    }
    if (tmp.trimmed().isEmpty()) {
        KMessageBox::error(this, i18n("Empty name is not supported"), i18n("Save Regular Expression"));
        return;
    }
    txt = tmp;

    QString fileName = dir + QLatin1Char('/') + txt + QStringLiteral(".regexp");
    QFileInfo finfo(fileName);
    if (finfo.exists()) {
        int answer = KMessageBox::warningContinueCancel(this,
                                                        i18n("<p>Overwrite named regular expression <b>%1</b></p>", txt),
                                                        QString(),
                                                        KStandardGuiItem::overwrite());
        if (answer != KMessageBox::Continue) {
            return;
        }
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        KMessageBox::error(this, i18n("Could not open file for writing: %1", fileName));
        return;
    }

    // Convert to XML.
    RegExp *regexp = _top->regExp();
    QString xml = regexp->toXmlString();
    delete regexp;

    QTextStream stream(&file);
    stream << xml;

    file.close();
    Q_EMIT savedRegexp();
}

void RegExpEditorWindow::slotSetRegExp(RegExp *regexp)
{
    // I have no clue why the following line is necesarry, but if it is not here
    // then the editor area is messed up when calling slotSetRegExp before starting the eventloop.
    qApp->processEvents();

    delete _top;
    RegExpWidget *widget = WidgetFactory::createWidget(regexp, this, this);
    if (!(_top = dynamic_cast<ConcWidget *>(widget))) {
        // It was not a ConcWidget
        _top = new ConcWidget(this, widget, this);
    }
    _top->setToplevel();

    _top->show();
    _layout->addWidget(_top);
    clearSelection(true); // HACK?
    Q_EMIT canSave(_top->hasAnyChildren());
}

void RegExpEditorWindow::updateCursorUnderPoint()
{
    RegExpWidget *widget = _top->widgetUnderPoint(QCursor::pos(), false);
    if (widget) {
        widget->updateCursorShape();
    }
}

void RegExpEditorWindow::emitVerifyRegExp()
{
    Q_EMIT verifyRegExp();
}

void RegExpEditorWindow::editWidget()
{
    auto EditPos = _PosEdit.isNull() ? QCursor::pos() : _PosEdit;
    RegExpWidget *editWidget = _top->findWidgetToEdit(EditPos);
    if (editWidget) {
        editWidget->edit();
    }
}

QIcon RegExpEditorWindow::getIcon(const QString &name)
{
    return QIcon::fromTheme(name);
}

#include "moc_regexpeditorwindow.cpp"
