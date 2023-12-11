/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "compoundwidget.h"

#include <QPainter>
#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QMouseEvent>

#include <KLocalizedString>
#include <QDialog>
#include <QLineEdit>
#include <KTextEdit>
#include <KIconLoader>
#include <QDialogButtonBox>
#include <QPushButton>

#include "concwidget.h"
#include "kwidgetstreamer.h"
#include "compoundregexp.h"

//================================================================================

CompoundDetailWindow::CompoundDetailWindow(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    QLabel *label = new QLabel(i18n("&Title:"), this);
    layout->addWidget(label);
    _title = new QLineEdit(this);
    layout->addWidget(_title);
    label->setBuddy(_title);

    label = new QLabel(i18n("&Description:"), this);
    layout->addWidget(label);
    _description = new KTextEdit(this);
    layout->addWidget(_description);
    label->setBuddy(_description);

    _allowReplace = new QCheckBox(i18n("&Automatically replace using this item"), this);
    layout->addWidget(_allowReplace);
    _allowReplace->setToolTip(i18n("When the content of this box is typed in to the ASCII line,<br />"
                                   "this box will automatically be added around it,<br />"
                                   "if this check box is selected."));
    _allowReplace->setChecked(true);

    _title->setFocus();
}

QString CompoundDetailWindow::title() const
{
    return _title->text();
}

QString CompoundDetailWindow::description() const
{
    return _description->toPlainText();
}

bool CompoundDetailWindow::allowReplace() const
{
    return _allowReplace->isChecked();
}

void CompoundDetailWindow::setTitle(const QString &txt)
{
    _title->setText(txt);
}

void CompoundDetailWindow::setDescription(const QString &txt)
{
    _description->setText(txt);
}

void CompoundDetailWindow::setAllowReplace(bool b)
{
    _allowReplace->setChecked(b);
}

//================================================================================

CompoundWidget::CompoundWidget(RegExpEditorWindow *editorWindow, QWidget *parent)
    : SingleContainerWidget(editorWindow, parent)
{
    _child = new ConcWidget(editorWindow, this);
    init();
}

CompoundWidget::CompoundWidget(CompoundRegExp *regexp, RegExpEditorWindow *editorWindow, QWidget *parent)
    : SingleContainerWidget(editorWindow, parent)
{
    init();
    _content->setTitle(regexp->title());
    _content->setDescription(regexp->description());
    _content->setAllowReplace(regexp->allowReplace());
    RegExpWidget *child = WidgetFactory::createWidget(regexp->child(), _editorWindow, this);
    if (!(_child = dynamic_cast<ConcWidget *>(child))) {
        _child = new ConcWidget(_editorWindow, child, this);
    }

    _hidden = regexp->hidden();
}

void CompoundWidget::init()
{
    _configWindow = new QDialog(this);
    _configWindow->setWindowTitle(i18n("Configure Compound"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    _configWindow->setLayout(mainLayout);

    _content = new CompoundDetailWindow(_configWindow);
    mainLayout->addWidget(_content);

    connect(_configWindow, &QDialog::rejected, this, &CompoundWidget::slotConfigCanceled);
    connect(_configWindow, &QDialog::finished, this, &CompoundWidget::slotConfigWindowClosed);

    _down = getIcon(QStringLiteral("arrow-down"));
    _up = getIcon(QStringLiteral("arrow-up"));

    _hidden = false;
    _backRefId = -1;

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    _configWindow->connect(buttonBox, &QDialogButtonBox::accepted, _configWindow, &QDialog::accept);
    _configWindow->connect(buttonBox, &QDialogButtonBox::rejected, _configWindow, &QDialog::reject);
    mainLayout->addWidget(buttonBox);
}

QSize CompoundWidget::sizeHint() const
{
    QFontMetrics metrics = fontMetrics();
    _childSize = _child->sizeHint();
    _textSize = metrics.size(0, _content->title());

    int width, height;

    if (_hidden) {
        _pixmapSize = _up.size();
        width = 2 * pw + qMax(2 * bdSize + _textSize.width(), 2 * bdSize + _pixmapSize.width());
        height = _pixmapSize.height() + 2 * bdSize + _textSize.height() + pw;
    } else {
        _pixmapSize = _down.size();
        int headerLineWidth = 2 * pw + 2 * bdSize + _pixmapSize.width();
        if (_textSize.width() != 0) {
            headerLineWidth += 3 * bdSize + _textSize.width();
        }

        width = qMax(2 * pw + _childSize.width(), headerLineWidth);
        height = qMax(_textSize.height(), _pixmapSize.height())
                 +2 * bdSize + _childSize.height() + pw;
    }
    return QSize(width, height);
}

void CompoundWidget::paintEvent(QPaintEvent *e)
{
    QSize mySize = sizeHint();

    QPainter painter(this);
    drawPossibleSelection(painter, mySize);

    int horLineY, childY;

    // draw top line
    if (_hidden) {
        horLineY = _pixmapSize.height() / 2;
        childY = _pixmapSize.height() + bdSize;
        _pixmapPos = QPoint(mySize.width() - pw - bdSize - _pixmapSize.width(), 0);
        painter.drawLine(pw, horLineY, _pixmapPos.x(), horLineY);
        painter.drawLine(mySize.width() - bdSize - pw, horLineY,
                         mySize.width(), horLineY);
        painter.drawPixmap(_pixmapPos, _up);
    } else {
        int maxH = qMax(_textSize.height(), _pixmapSize.height());
        int offset = 0;
        horLineY = maxH / 2;
        childY = maxH + bdSize;

        painter.drawLine(pw, horLineY, bdSize, horLineY);
        if (_textSize.width() != 0) {
            offset += pw + 2 * bdSize;

            painter.drawText(offset, horLineY - _textSize.height() / 2,
                             bdSize + _textSize.width(), horLineY + _textSize.height() / 2,
                             0, _content->title());
            offset += _textSize.width() + bdSize;
        }

        _pixmapPos = QPoint(mySize.width() - pw - bdSize - _pixmapSize.width(),
                            horLineY - _pixmapSize.height() / 2);

        painter.drawLine(offset, horLineY, _pixmapPos.x(), horLineY);
        painter.drawPixmap(_pixmapPos, _down);

        painter.drawLine(mySize.width() - bdSize - pw, horLineY, mySize.width(), horLineY);
    }

    // draw rest frame
    painter.drawLine(0, horLineY, 0, mySize.height());
    painter.drawLine(mySize.width() - pw, horLineY, mySize.width() - pw, mySize.height());
    painter.drawLine(0, mySize.height() - pw, mySize.width(), mySize.height() - pw);

    // place/size child
    if (_hidden) {
        _child->hide();
        painter.drawText(pw + bdSize, childY,
                         pw + bdSize + _textSize.width(), childY + _textSize.height(), 0,
                         _content->title());
    } else {
        QSize curSize = _child->size();
        QSize newSize = QSize(qMax(_child->sizeHint().width(), mySize.width() - 2 * pw),
                              _child->sizeHint().height());

        _child->move(pw, childY);
        if (curSize != newSize) {
            _child->resize(newSize);
            // I resized the child, so give it a chance to relect thus.
            _child->update();
        }

        _child->show();
    }

    RegExpWidget::paintEvent(e);
}

void CompoundWidget::slotConfigWindowClosed()
{
    _editorWindow->updateContent(nullptr);
    update();
}

void CompoundWidget::slotConfigCanceled()
{
    QDataStream stream(&_backup, QIODevice::ReadOnly);

    stream.setVersion(QDataStream::Qt_3_1);
    KWidgetStreamer streamer;
    streamer.fromStream(stream, _content);
    repaint();
}

RegExp *CompoundWidget::regExp() const
{
    return new CompoundRegExp(isSelected(), _content->title(), _content->description(),
                              _hidden, _content->allowReplace(), _child->regExp());
}

void CompoundWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton
        && QRect(_pixmapPos, _pixmapSize).contains(event->pos())) {
        // Skip otherwise we will never see the mouse release
        // since it is eaten by Editor window.
    } else {
        SingleContainerWidget::mousePressEvent(event);
    }
}

void CompoundWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton
        && QRect(_pixmapPos, _pixmapSize).contains(event->pos())) {
        _hidden = !_hidden;
        _editorWindow->updateContent(nullptr);
        repaint(); // is this necesary?
        _editorWindow->emitChange();
    } else {
        SingleContainerWidget::mouseReleaseEvent(event);
    }
}

bool CompoundWidget::updateSelection(bool parentSelected)
{
    if (_hidden) {
        bool changed = RegExpWidget::updateSelection(parentSelected);
        _child->selectWidget(_isSelected);
        if (changed) {
            repaint();
        }
        return changed;
    } else {
        return SingleContainerWidget::updateSelection(parentSelected);
    }
}

int CompoundWidget::edit()
{
    _configWindow->move(QCursor::pos() - QPoint(_configWindow->sizeHint().width() / 2,
                                                _configWindow->sizeHint().height() / 2));
    QDataStream stream(&_backup, QIODevice::WriteOnly);

    stream.setVersion(QDataStream::Qt_3_1);
    KWidgetStreamer streamer;
    streamer.toStream(_content, stream);
    return _configWindow->exec();
}

int nextId()
{
    static int counter = 0;
    return ++counter;
}

QPixmap CompoundWidget::getIcon(const QString &name)
{
    return QIcon::fromTheme(name).pixmap(KIconLoader::SizeSmall);
}
