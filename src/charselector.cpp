/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "charselector.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QStackedWidget>

#include <KLocalizedString>

#include "limitedcharlineedit.h"
#include "regexpconverter.h"

/**
   In the class CharSelector, three LimitedCharLineEdit are used.
   These widgets are all used in a QWidgetStack. The LimitedCharLineEdit
   class is basically a QLineEdit, which is limited to a certain
   number of characters. This conflicts with the QWidgetStack, as this
   class expects the widgets on the stack to take up all space.
   StackContainer fills in this gab.
*/
class StackContainer : public QWidget
{
    Q_OBJECT
public:
    explicit StackContainer(QWidget *child, QWidget *parent)
        : QWidget(parent)
    {
        QHBoxLayout *layout = new QHBoxLayout(this);
        child->setParent(this);
        layout->addWidget(child);
        layout->addStretch(1);
    }
};

CharSelector::CharSelector(QWidget *parent)
    : QWidget(parent)
{
    QStringList items;
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    _type = new QComboBox(this);
    _type->setObjectName(QStringLiteral("_type"));
    items << i18n("Normal Character") << i18n("Unicode Char in Hex.") << i18n("Unicode Char in Oct.") << QStringLiteral("----")
          << i18n("The Bell Character (\\a)") << i18n("The Form Feed Character (\\f)") << i18n("The Line Feed Character (\\n)")
          << i18n("The Carriage Return Character (\\r)") << i18n("The Horizontal Tab Character (\\t)") << i18n("The Vertical Tab Character (\\v)");
    _type->addItems(items);
    layout->addWidget(_type);

    _stack = new QStackedWidget(this /*, "_stack"*/);
    layout->addWidget(_stack);

    _normal = new LimitedCharLineEdit(LimitedCharLineEdit::NORMAL, nullptr, QStringLiteral("_normal"));
    _stack->insertWidget(0, new StackContainer(_normal, _stack));

    _hex = new LimitedCharLineEdit(LimitedCharLineEdit::HEX, _stack, QStringLiteral("_hex"));
    _stack->insertWidget(1, new StackContainer(_hex, _stack));

    _oct = new LimitedCharLineEdit(LimitedCharLineEdit::OCT, _stack, QStringLiteral("_oct"));
    _stack->insertWidget(2, new StackContainer(_oct, _stack));

    _stack->setCurrentIndex(0);

    connect(_type, &QComboBox::activated, this, &CharSelector::slotNewItem);
}

void CharSelector::slotNewItem(int which)
{
    _type->setCurrentIndex(which);
    if (which <= 2) {
        _stack->setCurrentIndex(which);
        _normal->setEnabled(true);
        _hex->setEnabled(true);
        _oct->setEnabled(true);
    } else if (which == 3) {
        _type->setCurrentIndex(_oldIndex);
        slotNewItem(_oldIndex);
        return;
    } else {
        _normal->setEnabled(false);
        _hex->setEnabled(false);
        _oct->setEnabled(false);
    }

    _oldIndex = which;
}

void CharSelector::setText(const QString &text)
{
    // This is the best I can do about missing character range features, unless all of
    // textrangeregexp is to be reworked. The problem is that textrangeregexp only allows to
    // get the characters, which would be something like \a, but \a does not work with say Emacs
    // style regexps -- ko28 Sep. 2003 10:55 -- Jesper K. Pedersen
    bool enabled = (RegExpConverter::current()->features() & RegExpConverter::ExtRange);
    _type->setEnabled(enabled);

    if (text.mid(0, 1) == QLatin1Char('\\')) {
        if (text.mid(1, 1) == QLatin1Char('x')) {
            _hex->setText(text.mid(2, 1));
            slotNewItem(1);
        } else if (text.mid(1, 1) == QLatin1Char('0')) {
            _oct->setText(text.mid(2, 1));
            slotNewItem(2);
        } else if (text.mid(1, 1) == QLatin1Char('a')) {
            slotNewItem(4);
        } else if (text.mid(1, 1) == QLatin1Char('f')) {
            slotNewItem(5);
        } else if (text.mid(1, 1) == QLatin1Char('n')) {
            slotNewItem(6);
        } else if (text.mid(1, 1) == QLatin1Char('r')) {
            slotNewItem(7);
        } else if (text.mid(1, 1) == QLatin1Char('t')) {
            slotNewItem(8);
        } else if (text.mid(1, 1) == QLatin1Char('v')) {
            slotNewItem(9);
        } else {
            qWarning("Warning %s:%d Unknown escape %s", __FILE__, __LINE__, qPrintable(text));
        }
    } else {
        slotNewItem(0);
        _normal->setText(text);
    }
}

bool CharSelector::isEmpty() const
{
    return (_type->currentIndex() == 0 && _normal->text().isEmpty()) || (_type->currentIndex() == 1 && _hex->text().isEmpty())
        || (_type->currentIndex() == 2 && _oct->text().isEmpty());
}

QString CharSelector::text() const
{
    switch (_type->currentIndex()) {
    case 0: // Normal Character
        return _normal->text();
    case 1: // Hex
        return QStringLiteral("\\x") + _hex->text();
    case 2: // Oct
        return QStringLiteral("\\0") + _oct->text();
    case 3: // The separator
        break;
    case 4:
        return QStringLiteral("\\a");
    case 5:
        return QStringLiteral("\\f");
    case 6:
        return QStringLiteral("\\n");
    case 7:
        return QStringLiteral("\\r");
    case 8:
        return QStringLiteral("\\t");
    case 9:
        return QStringLiteral("\\v");
    }
    return QString();
}

#include "charselector.moc"

#include "moc_charselector.cpp"
