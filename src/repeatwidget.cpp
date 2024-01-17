/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "repeatwidget.h"

#include <QButtonGroup>
#include <QGroupBox>
#include <QLabel>
#include <QPainter>
#include <QRadioButton>
#include <QSpinBox>
#include <QVBoxLayout>

#include <KLocalizedString>
#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>

#include "concwidget.h"
#include "kwidgetstreamer.h"
#include "repeatregexp.h"

RepeatWidget::RepeatWidget(RegExpEditorWindow *editorWindow, QWidget *parent)
    : SingleContainerWidget(editorWindow, parent)
{
    _child = new ConcWidget(editorWindow, this);
    init();
}

RepeatWidget::RepeatWidget(RepeatRegExp *regexp, RegExpEditorWindow *editorWindow, QWidget *parent)
    : SingleContainerWidget(editorWindow, parent)
{
    init();
    RegExpWidget *child = WidgetFactory::createWidget(regexp->child(), editorWindow, this);
    if (!(_child = dynamic_cast<ConcWidget *>(child))) {
        _child = new ConcWidget(editorWindow, child, this);
    }

    if (regexp->max() == -1) {
        if (regexp->min() == 0) {
            _content->set(RepeatRangeWindow::ANY, regexp->min(), regexp->max());
        } else {
            _content->set(RepeatRangeWindow::ATLEAST, regexp->min(), regexp->max()); // krazy:exclude=spelling
        }
    } else {
        if (regexp->min() == 0) {
            _content->set(RepeatRangeWindow::ATMOST, regexp->min(), regexp->max());
        } else if (regexp->min() == regexp->max()) {
            _content->set(RepeatRangeWindow::EXACTLY, regexp->min(), regexp->max());
        } else {
            _content->set(RepeatRangeWindow::MINMAX, regexp->min(), regexp->max());
        }
    }
}

void RepeatWidget::init()
{
    _configWindow = new QDialog(this);
    _configWindow->setWindowTitle(i18n("Number of Times to Repeat Content"));
    QVBoxLayout *mainLayout = new QVBoxLayout;
    _configWindow->setLayout(mainLayout);

    _content = new RepeatRangeWindow(_configWindow);
    connect(_configWindow, &QDialog::rejected, this, &RepeatWidget::slotConfigCanceled);
    connect(_configWindow, &QDialog::finished, this, &RepeatWidget::slotConfigWindowClosed);
    mainLayout->addWidget(_content);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    mainLayout->addWidget(buttonBox);
    _configWindow->connect(buttonBox, &QDialogButtonBox::accepted, _configWindow, &QDialog::accept);
    _configWindow->connect(buttonBox, &QDialogButtonBox::rejected, _configWindow, &QDialog::reject);
}

QSize RepeatWidget::sizeHint() const
{
    // TODO: Merge with LookAheadWidget::sizeHint
    QFontMetrics metrics = fontMetrics();
    _textSize = metrics.size(0, _content->text());

    _childSize = _child->sizeHint();

    int height = _textSize.height() + bdSize + _childSize.height() + bdSize + 2 * pw;
    int width = 2 * pw + qMax(_childSize.width(), 4 * bdSize + _textSize.width());
    return QSize(width, height);
}

void RepeatWidget::paintEvent(QPaintEvent *e)
{
    // TODO: Merge with LookAheadWidget::paintEvent
    QSize mySize = sizeHint();
    QPainter painter(this);

    drawPossibleSelection(painter, mySize);

    // move the child to its position and resize it.
    _child->move(pw, _textSize.height() + bdSize);
    QSize curChildSize = _child->size();
    QSize newChildSize = QSize(mySize.width() - 2 * pw, _childSize.height());
    if (curChildSize != newChildSize) {
        _child->resize(newChildSize);
        // I resized the child, so give it a chance to relect thus.
        _child->update();
    }

    // Draw the border and the text.
    int startY = _textSize.height() / 2;

    // Top lines and text
    painter.drawLine(pw, startY, bdSize, startY);
    painter.drawText(pw + 2 * bdSize, 0, _textSize.width(), _textSize.height(), 0, _content->text());
    int offset = pw + 3 * bdSize + _textSize.width();
    painter.drawLine(offset, startY, mySize.width() - pw, startY);

    // horizontal lines
    painter.drawLine(0, startY, 0, mySize.height() - pw);
    painter.drawLine(mySize.width() - pw, startY, mySize.width() - pw, mySize.height() - pw);

    // buttom line
    painter.drawLine(0, mySize.height() - pw, mySize.width() - pw, mySize.height() - pw);

    SingleContainerWidget::paintEvent(e);
}

RegExp *RepeatWidget::regExp() const
{
    return new RepeatRegExp(isSelected(), _content->min(), _content->max(), _child->regExp());
}

void RepeatWidget::slotConfigWindowClosed()
{
    _editorWindow->updateContent(nullptr);
    update();
}

void RepeatWidget::slotConfigCanceled()
{
    QDataStream stream(&_backup, QIODevice::ReadOnly);

    stream.setVersion(QDataStream::Qt_3_1);
    KWidgetStreamer streamer;
    streamer.fromStream(stream, _content);
    repaint();
}

int RepeatWidget::edit()
{
    _configWindow->move(QCursor::pos() - QPoint(_configWindow->sizeHint().width() / 2, _configWindow->sizeHint().height() / 2));
    QDataStream stream(&_backup, QIODevice::WriteOnly);

    stream.setVersion(QDataStream::Qt_3_1);
    KWidgetStreamer streamer;
    streamer.toStream(_content, stream);

    return _configWindow->exec();
}

//--------------------------------------------------------------------------------
RepeatRangeWindow::RepeatRangeWindow(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    _groupWidget = new QGroupBox(i18n("Times to Match"));
    mainLayout->addWidget(_groupWidget);
    _group = new QButtonGroup(this);

    QGridLayout *groupLayout = new QGridLayout(_groupWidget);

    // Any number of times
    QRadioButton *radioBut = new QRadioButton(i18n("Any number of times (including zero times)"));
    radioBut->setObjectName(QStringLiteral("RepeatRangeWindow::choice any times"));

    groupLayout->addWidget(radioBut, 0, 0, 1, 3);
    _group->addButton(radioBut, ANY);
    radioBut->click();

    createLine(groupLayout, i18n("At least"), &_leastTimes, ATLEAST); // krazy:exclude=spelling
    createLine(groupLayout, i18n("At most"), &_mostTimes, ATMOST);
    createLine(groupLayout, i18n("Exactly"), &_exactlyTimes, EXACTLY);

    // from ___ to ___ times
    radioBut = new QRadioButton(i18n("From"));
    radioBut->setObjectName(QStringLiteral("RepeatRangeWindow::from"));
    groupLayout->addWidget(radioBut, 4, 0);
    _group->addButton(radioBut, MINMAX);

    _rangeFrom = new QSpinBox();
    _rangeFrom->setRange(1, 999);
    _rangeFrom->setSingleStep(1);
    groupLayout->addWidget(_rangeFrom, 4, 1);

    QHBoxLayout *layout = new QHBoxLayout();

    QLabel *label = new QLabel(i18n("to"));
    layout->addWidget(label);

    _rangeTo = new QSpinBox();
    _rangeTo->setRange(1, 999);
    _rangeTo->setSingleStep(1);
    layout->addWidget(_rangeTo);

    label = new QLabel(i18n("time(s)"));
    layout->addWidget(label);

    groupLayout->addLayout(layout, 4, 2);

    connect(_rangeFrom, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateMaxVal(int)));
    connect(_rangeTo, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateMinVal(int)));

    connect(_group, SIGNAL(buttonClicked(int)), this, SLOT(slotItemChange(int)));

    _group->button(ANY)->click();
}

void RepeatRangeWindow::createLine(QGridLayout *layout, const QString &text, QSpinBox **spin, REPEATTYPE tp)
{
    int row = layout->rowCount();

    QRadioButton *radioBut = new QRadioButton(text);
    layout->addWidget(radioBut, row, 0);

    *spin = new QSpinBox();
    (*spin)->setRange(1, 999);
    (*spin)->setSingleStep(1);
    (*spin)->setValue(1);
    layout->addWidget(*spin, row, 1);

    QLabel *label = new QLabel(i18n("time(s)"));
    layout->addWidget(label, row, 2, 1, 2);
    _group->addButton(radioBut, tp);
}

void RepeatRangeWindow::slotItemChange(int which)
{
    _leastTimes->setEnabled(false);
    _mostTimes->setEnabled(false);
    _exactlyTimes->setEnabled(false);
    _rangeFrom->setEnabled(false);
    _rangeTo->setEnabled(false);

    switch (which) {
    case ANY:
        break;
    case ATLEAST:
        _leastTimes->setEnabled(true);
        break; // krazy:exclude=spelling
    case ATMOST:
        _mostTimes->setEnabled(true);
        break;
    case EXACTLY:
        _exactlyTimes->setEnabled(true);
        break;
    case MINMAX:
        _rangeFrom->setEnabled(true);
        _rangeTo->setEnabled(true);
        break;
    }
}

void RepeatRangeWindow::slotUpdateMinVal(int maxVal)
{
    if (_rangeFrom->value() > maxVal) {
        _rangeFrom->setValue(maxVal);
    }
}

void RepeatRangeWindow::slotUpdateMaxVal(int minVal)
{
    if (_rangeTo->value() < minVal) {
        _rangeTo->setValue(minVal);
    }
}

QString RepeatRangeWindow::text()
{
    switch (_group->checkedId()) {
    case ANY:
        return i18n("Repeated Any Number of Times");
    case ATLEAST:
        return i18np("Repeated at Least 1 Time", "Repeated at Least %1 Times", _leastTimes->value()); // krazy:exclude=spelling
    case ATMOST:
        return i18np("Repeated at Most 1 Time", "Repeated at Most %1 Times", _mostTimes->value());
    case EXACTLY:
        return i18np("Repeated Exactly 1 Time", "Repeated Exactly %1 Times", _exactlyTimes->value());
    case MINMAX:
        return i18n("Repeated From %1 to %2 Times", _rangeFrom->value(), _rangeTo->value());
    }

    qFatal("Fall through!");
    return QString();
}

int RepeatRangeWindow::min()
{
    switch (_group->checkedId()) {
    case ANY:
        return 0;
    case ATLEAST:
        return _leastTimes->value(); // krazy:exclude=spelling
    case ATMOST:
        return 0;
    case EXACTLY:
        return _exactlyTimes->value();
    case MINMAX:
        return _rangeFrom->value();
    }
    qFatal("Fall through!");
    return -1;
}

int RepeatRangeWindow::max()
{
    switch (_group->checkedId()) {
    case ANY:
        return -1;
    case ATLEAST:
        return -1; // krazy:exclude=spelling
    case ATMOST:
        return _mostTimes->value();
    case EXACTLY:
        return _exactlyTimes->value();
    case MINMAX:
        return _rangeTo->value();
    }
    qFatal("Fall through!");
    return -1;
}

void RepeatRangeWindow::set(REPEATTYPE tp, int min, int max)
{
    _group->button(tp)->click();
    switch (tp) {
    case ANY:
        break;
    case ATLEAST: // krazy:exclude=spelling
        _leastTimes->setValue(min);
        break;
    case ATMOST:
        _mostTimes->setValue(max);
        break;
    case EXACTLY:
        _exactlyTimes->setValue(min);
        break;
    case MINMAX:
        _rangeFrom->setValue(min);
        _rangeTo->setValue(max);
        break;
    }
}

#include "moc_repeatwidget.cpp"
