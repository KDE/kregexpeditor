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

#include "characterswidget.h"

#include <QApplication>
#include <QPainter>
#include <QLabel>
#include <QCheckBox>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDialogButtonBox>

#include "textrangeregexp.h"
#include "charselector.h"
#include "myfontmetrics.h"
#include "regexpconverter.h"

CharacterEdits *CharactersWidget::_configWindow = nullptr;

CharactersWidget::CharactersWidget(RegExpEditorWindow *editorWindow, QWidget *parent)
    : RegExpWidget(editorWindow, parent)
{
    _regexp = new TextRangeRegExp(false /* not used */);
}

CharactersWidget::CharactersWidget(TextRangeRegExp *regexp, RegExpEditorWindow *editorWindow, QWidget *parent)
    : RegExpWidget(editorWindow, parent)
{
    _regexp = dynamic_cast<TextRangeRegExp *>(regexp->clone());
    Q_ASSERT(_regexp);
}

CharactersWidget::~CharactersWidget()
{
    delete _regexp;
}

QSize CharactersWidget::sizeHint() const
{
    QFontMetrics metrics = fontMetrics();
    _textSize = HackCalculateFontSize(metrics, title());
    //  _textSize = metrics.size(0, title());

    QSize headerSize = QSize(_textSize.width() + 4 * bdSize,
                             _textSize.height());

    _contentSize = HackCalculateFontSize(metrics, text());
    //  _contentSize = metrics.size(0, text());

    return QSize(qMax(headerSize.width(), bdSize + _contentSize.width() + bdSize + 2 * pw),
                 headerSize.height() + bdSize + _contentSize.height() + bdSize + 2 * pw);
}

void CharactersWidget::paintEvent(QPaintEvent *e)
{
    QSize mySize = sizeHint();

    QPainter painter(this);
    drawPossibleSelection(painter, mySize);

    int center = _textSize.height() / 2;
    int offset = 0;

    // draw the horizontal line and the label
    painter.drawLine(pw, center, bdSize, center);
    offset += pw + 2 * bdSize;

    painter.drawText(offset, 0, _textSize.width(), _textSize.height(), 0, title());
    offset += _textSize.width() + bdSize;

    painter.drawLine(offset, center, mySize.width(), center);

    // Draw the rest of the lines
    int y = mySize.width() - pw;
    int x = mySize.height() - pw;
    painter.drawLine(0, center, 0, mySize.height());
    painter.drawLine(y, center, y, mySize.height());
    painter.drawLine(0, x, y, x);

    // Draw the text
    painter.drawText(bdSize, bdSize + _textSize.height(), _contentSize.width(),
                     _contentSize.height(), 0, text());

    RegExpWidget::paintEvent(e);
}

RegExp *CharactersWidget::regExp() const
{
    RegExp *r = _regexp->clone();
    r->setSelected(isSelected());
    return r;
}

QString CharactersWidget::text() const
{
    QString res = QString();

    if (_regexp->wordChar()) {
        res += i18n("- A word character\n");
    }

    if (_regexp->nonWordChar()) {
        res += i18n("- A non-word character\n");
    }

    if (_regexp->digit()) {
        res += i18n("- A digit character\n");
    }

    if (_regexp->nonDigit()) {
        res += i18n("- A non-digit character\n");
    }

    if (_regexp->space()) {
        res += i18n("- A space character\n");
    }

    if (_regexp->nonSpace()) {
        res += i18n("- A non-space character\n");
    }

    // Single characters
    QStringList chars = _regexp->chars();
    if (!chars.isEmpty()) {
        QString str = chars.join(QStringLiteral(", "));
        res += QStringLiteral("- ") + str + QStringLiteral("\n");
    }

    // Ranges characters
    foreach (const StringPair &elm, _regexp->range()) {
        QString fromText = elm.first;
        QString toText = elm.second;

        res += i18nc("a range of characters", "- from %1 to %2\n", fromText, toText);
    }
    return res.left(res.length() - 1);
}

QString CharactersWidget::title() const
{
    if (_regexp->negate()) {
        return i18n("Any Character Except");
    } else {
        return i18n("One of Following Characters");
    }
}

RegExpWidget *CharactersWidget::findWidgetToEdit(QPoint globalPos)
{
    if (QRect(mapToGlobal(QPoint(0, 0)), size()).contains(globalPos)) {
        return this;
    } else {
        return nullptr;
    }
}

int CharactersWidget::edit()
{
    if (_configWindow == 0) {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        // No parent here, as this window should continue to exists.
        _configWindow = new CharacterEdits(0);
        _configWindow->setObjectName(QStringLiteral("CharactersWidget::_configWindow"));
        QApplication::restoreOverrideCursor();
    }

    _configWindow->move(QCursor::pos() - QPoint(_configWindow->sizeHint().width() / 2,
                                                _configWindow->sizeHint().height() / 2));
    _configWindow->setRegexp(_regexp);
    int ret = _configWindow->exec();
    if (ret == QDialog::Accepted) {
        _editorWindow->updateContent(0);
        update();
    }
    return ret;
}

void CharacterEdits::addCharacter(const QString &txt)
{
    KMultiFormListBoxEntryList list = _single->elements();
    foreach (KMultiFormListBoxEntry *e, list) {
        SingleEntry *entry = dynamic_cast<SingleEntry *>(e);
        if (entry && entry->isEmpty()) {
            entry->setText(txt);
            return;
        }
    }

    SingleEntry *entry = new SingleEntry(_single);
    entry->setText(txt);
    _single->append(entry);
}

void CharacterEdits::addRange(const QString &from, const QString &to)
{
    KMultiFormListBoxEntryList list = _range->elements();
    foreach (KMultiFormListBoxEntry *e, list) {
        RangeEntry *entry = dynamic_cast<RangeEntry *>(e);
        if (entry && entry->isEmpty()) {
            entry->setFrom(from);
            entry->setTo(to);
            return;
        }
    }

    RangeEntry *entry = new RangeEntry(_range);
    entry->setFrom(from);
    entry->setTo(to);
    _range->append(entry);
}

void CharacterEdits::setRegexp(TextRangeRegExp *regexp)
{
    _regexp = regexp;
    negate->setChecked(regexp->negate());
    digit->setChecked(regexp->digit());
    _nonDigit->setChecked(regexp->nonDigit());
    space->setChecked(regexp->space());
    _nonSpace->setChecked(regexp->nonSpace());
    wordChar->setChecked(regexp->wordChar());
    _nonWordChar->setChecked(regexp->nonWordChar());

    bool enabled = (RegExpConverter::current()->features() & RegExpConverter::CharacterRangeNonItems);
    _nonWordChar->setEnabled(enabled);
    _nonDigit->setEnabled(enabled);
    _nonSpace->setEnabled(enabled);

    // Characters

    KMultiFormListBoxEntryList list1 = _single->elements();
    foreach (KMultiFormListBoxEntry *e, list1) {
        SingleEntry *entry = dynamic_cast<SingleEntry *>(e);
        if (entry) {
            entry->setText(QString());
        }
    }
    QStringList list2 = regexp->chars();
    for (QStringList::Iterator it2(list2.begin()); it2 != list2.end(); ++it2) {
        addCharacter(*it2);
    }

    // Ranges
    KMultiFormListBoxEntryList list3 = _range->elements();
    foreach (KMultiFormListBoxEntry *e, list3) {
        RangeEntry *entry = dynamic_cast<RangeEntry *>(e);
        if (entry) {
            entry->setFrom(QString());
            entry->setTo(QString());
        }
    }

    foreach (const StringPair &elm, _regexp->range()) {
        QString from = elm.first;
        QString to = elm.second;
        addRange(from, to);
    }
}

CharacterEdits::CharacterEdits(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18n("Specify Characters"));
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    setObjectName(QStringLiteral("CharacterEdits"));
    QWidget *top = new QWidget(this);
    QVBoxLayout *topLayout = new QVBoxLayout(top);
    topLayout->setMargin(0);
    mainLayout->addWidget(top);

    negate = new QCheckBox(i18n("Do not match the characters specified here"));
    topLayout->addWidget(negate);

    // The predefined box
    QGroupBox *predefined = new QGroupBox(i18n("Predefined Character Ranges"));
    topLayout->addWidget(predefined);
    QGridLayout *predefinedLayout = new QGridLayout(predefined);

    wordChar = new QCheckBox(i18n("A word character"));
    predefinedLayout->addWidget(wordChar, 0, 0);
    digit = new QCheckBox(i18n("A digit character"));
    predefinedLayout->addWidget(digit, 0, 1);
    space = new QCheckBox(i18n("A space character"));
    predefinedLayout->addWidget(space, 0, 2);

    _nonWordChar = new QCheckBox(i18n("A non-word character"));
    predefinedLayout->addWidget(_nonWordChar, 1, 0);
    _nonDigit = new QCheckBox(i18n("A non-digit character"));
    predefinedLayout->addWidget(_nonDigit, 1, 1);
    _nonSpace = new QCheckBox(i18n("A non-space character"));
    predefinedLayout->addWidget(_nonSpace, 1, 2);

    // Single characters
    QGroupBox *singleBox = new QGroupBox(i18n("Single Characters"));
    QVBoxLayout *groupLayout = new QVBoxLayout(singleBox);
    topLayout->addWidget(singleBox);
    _single = new KMultiFormListBox(new SingleFactory(), KMultiFormListBox::MultiVisible);
    groupLayout->addWidget(_single);
    _single->addElement();
    _single->addElement();
    _single->addElement();

    QWidget *moreW = new QWidget();
    groupLayout->addWidget(moreW);
    QHBoxLayout *moreLay = new QHBoxLayout(moreW);
    QPushButton *more = new QPushButton(i18n("More Entries"));
    moreLay->addWidget(more);
    moreLay->addStretch(1);
    moreLay->setMargin(0);

    connect(more, &QAbstractButton::clicked, _single, &KMultiFormListBox::addElement);

    // Ranges
    QGroupBox *rangeBox = new QGroupBox(i18n("Character Ranges"));
    groupLayout = new QVBoxLayout(rangeBox);
    topLayout->addWidget(rangeBox);

    _range = new KMultiFormListBox(new RangeFactory(), KMultiFormListBox::MultiVisible);
    groupLayout->addWidget(_range);
    _range->addElement();
    _range->addElement();
    _range->addElement();

    moreW = new QWidget();
    groupLayout->addWidget(moreW);
    moreLay = new QHBoxLayout(moreW);
    more = new QPushButton(i18n("More Entries"));
    moreLay->addWidget(more);
    moreLay->addStretch(1);
    moreLay->setMargin(0);
    connect(more, &QAbstractButton::clicked, _range, &KMultiFormListBox::addElement);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(okButton, &QAbstractButton::clicked, this, &CharacterEdits::slotOK);
    mainLayout->addWidget(buttonBox);
}

void CharacterEdits::slotOK()
{
    _regexp->setNegate(negate->isChecked());

    _regexp->setWordChar(wordChar->isChecked());
    _regexp->setNonWordChar(_nonWordChar->isChecked());

    _regexp->setDigit(digit->isChecked());
    _regexp->setNonDigit(_nonDigit->isChecked());

    _regexp->setSpace(space->isChecked());
    _regexp->setNonSpace(_nonSpace->isChecked());

    // single characters
    _regexp->clearChars();
    KMultiFormListBoxEntryList list = _single->elements();
    foreach (KMultiFormListBoxEntry *e, list) {
        SingleEntry *entry = dynamic_cast<SingleEntry *>(e);
        if (entry && !entry->isEmpty()) {
            _regexp->addCharacter(entry->text());
        }
    }

    // Ranges
    _regexp->clearRange();
    list = _range->elements();
    foreach (KMultiFormListBoxEntry *e, list) {
        RangeEntry *entry = dynamic_cast<RangeEntry *>(e);
        if (entry && !entry->isEmpty()) {
            _regexp->addRange(entry->fromText(), entry->toText());
        }
    }
    accept();
}

SingleEntry::SingleEntry(QWidget *parent)
    : KMultiFormListBoxEntry(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    _selector = new CharSelector(this);
    layout->addWidget(_selector);
    layout->addStretch(1);
}

QString SingleEntry::text() const
{
    return _selector->text();
}

void SingleEntry::setText(const QString &text)
{
    _selector->setText(text);
}

bool SingleEntry::isEmpty() const
{
    return _selector->isEmpty();
}

RangeEntry::RangeEntry(QWidget *parent)
    : KMultiFormListBoxEntry(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);

    QLabel *label = new QLabel(i18n("From:"), this);
    _from = new CharSelector(this);
    layout->addWidget(label);
    layout->addWidget(_from);

    layout->addStretch(1);

    label = new QLabel(i18nc("end of range", "To:"), this);
    _to = new CharSelector(this);
    layout->addWidget(label);
    layout->addWidget(_to);
}

QString RangeEntry::fromText() const
{
    return _from->text();
}

QString RangeEntry::toText() const
{
    return _to->text();
}

void RangeEntry::setFrom(const QString &text)
{
    _from->setText(text);
}

void RangeEntry::setTo(const QString &text)
{
    _to->setText(text);
}

bool RangeEntry::isEmpty() const
{
    return _from->isEmpty() || _to->isEmpty();
}
