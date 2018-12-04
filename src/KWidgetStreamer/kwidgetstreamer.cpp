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

#include "kwidgetstreamer.h"

#include <QObject>
#include <QVariant>

#include "kmultiformlistbox.h"

void KWidgetStreamer::toStream(const QObject *from, QDataStream &stream)
{
    const KMultiFormListBox *listBox = qobject_cast<const KMultiFormListBox*>(from);
    if (listBox) {
        listBox->toStream(stream);
        return;
    }

    propertyToStream(from, stream);
}

void KWidgetStreamer::fromStream(QDataStream &stream, QObject *to)
{
    KMultiFormListBox *listBox = qobject_cast<KMultiFormListBox*>(to);
    if (listBox) {
        listBox->toStream(stream);
        return;
    }

    propertyFromStream(stream, to);
}

void KWidgetStreamer::propertyToStream(const QObject *from, QDataStream &stream)
{
    // Only handle widgets. Alternatives to widgets are layouts, validators, timers, etc.
    if (!qobject_cast<const QWidget*>(from)) {
        return;
    }

    // Stream in all the children (if any)
    const QList<QObject *> children = from->children();

    if (children.count() > 0) {
        stream <<  children.count();
        for (int i = 0; i < children.size(); ++i) {
            toStream(children.at(i), stream);
        }
    } else {
        stream << (unsigned int)0;
    }

    // Now stream out properties
    for (PropertyMapIt mapIt = _map.constBegin(); mapIt != _map.constEnd(); ++mapIt) {
        QString tp = mapIt.key();
        PropertyList list = mapIt.value();
        if (from->inherits(tp.toLocal8Bit())) {
            for (PropertyListIt it = list.begin(); it != list.end(); ++it) {
                QVariant prop = from->property((*it).toLocal8Bit());
                if (!prop.isValid()) {
                    qWarning("Invalid property: %s:%s", qPrintable(tp), qPrintable(*it));
                }

                stream <<  prop;
            }
        }
    }
}

void KWidgetStreamer::propertyFromStream(QDataStream &stream, QObject *to)
{
    // Only handle widgets. Alternatives to widgets are layouts, validators, timers, etc.
    if (!qobject_cast<QWidget*>(to)) {
        return;
    }

    // Stream in all the children (if any)
    const QList<QObject *> children = to->children();

    for (int i = 0; i < children.size(); ++i) {
        fromStream(stream, children.at(i));
    }

    // Now stream in properties
    for (PropertyMapIt mapIt = _map.constBegin(); mapIt != _map.constEnd(); ++mapIt) {
        QString tp = mapIt.key();
        PropertyList list = mapIt.value();
        if (to->inherits(tp.toLocal8Bit())) {
            for (PropertyListIt it = list.begin(); it != list.end(); ++it) {
                QVariant value;
                stream >> value;
                to->setProperty((*it).toLocal8Bit(), value);
            }
        }
    }
}

KWidgetStreamer::KWidgetStreamer()
{
    // QCheckBox
    _map.insert(QStringLiteral("QCheckBox"), {
        QStringLiteral("enabled"), QStringLiteral("checked"),
        QStringLiteral("tristate"),
    });

    // QComboBox
    _map.insert(QStringLiteral("QComboBox"), {
        QStringLiteral("enabled"), QStringLiteral("editable"),
        QStringLiteral("currentItem"), QStringLiteral("maxCount"),
        QStringLiteral("insertionPolicy"), QStringLiteral("autoCompletion"),
    });

    // QDial
    _map.insert(QStringLiteral("QDial"), {
        QStringLiteral("enabled"), QStringLiteral("tracking"),
        QStringLiteral("wrapping"), QStringLiteral("value"),
    });

    // QLCDNumber
    _map.insert(QStringLiteral("QLCDNumber"), {
        QStringLiteral("enabled"), QStringLiteral("digitCount"),
        QStringLiteral("mode"), QStringLiteral("segmentStyle"),
        QStringLiteral("value"),
    });

    // QLineEdit
    _map.insert(QStringLiteral("QLineEdit"), {
        QStringLiteral("enabled"), QStringLiteral("text"),
        QStringLiteral("maxLength"), QStringLiteral("echoMode"),
        QStringLiteral("alignment"),
    });

    // QMultiLineEdit
    _map.insert(QStringLiteral("QTextEdit"), {
        QStringLiteral("enabled"), QStringLiteral("plainText")
    });

    // QRadioButton
    _map.insert(QStringLiteral("QRadioButton"), {
        QStringLiteral("enabled"), QStringLiteral("checked")
    });

    // QSlider
    _map.insert(QStringLiteral("QSlider"), {
        QStringLiteral("enabled"), QStringLiteral("value"),
    });

    // QSpinBox
    _map.insert(QStringLiteral("QSpinBox"), {
        QStringLiteral("enabled"), QStringLiteral("value"),
    });
}
