/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#include "kwidgetstreamer.h"

#include <QObject>
#include <QVariant>

#include "kmultiformlistbox.h"

void KWidgetStreamer::toStream(const QObject *from, QDataStream &stream)
{
    if (from->inherits("KMultiFormListBox")) {
        // Hmm, we'll trust Qt that this dynamic_cast won't fail!
        dynamic_cast<const KMultiFormListBox *>(from)->toStream(stream);
    }

    propertyToStream(from, stream);
}

void KWidgetStreamer::fromStream(QDataStream &stream, QObject *to)
{
    if (to->inherits("KMultiFormListBox")) {
        // Hmm, we'll trust Qt that this dynamic_cast won't fail!
        dynamic_cast<KMultiFormListBox *>(to)->fromStream(stream);
    }

    propertyFromStream(stream, to);
}

void KWidgetStreamer::propertyToStream(const QObject *from, QDataStream &stream)
{
    // Only handle widgets. Alternatives to widgets are layouts, validators, timers, etc.
    if (!from->inherits("QWidget")) {
        return;
    }

    // Stream in all the children (if any)
    const QList<QObject *> children = from->children();
    // unsigned int count;

    // stream >> count;
    if (children.count() > 0) {
        stream << children.count();
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
        if (from->inherits(tp.toLocal8Bit().data())) {
            for (PropertyListIt it = list.begin(); it != list.end(); ++it) {
                QVariant prop = from->property(it->toLocal8Bit().data());
                if (!prop.isValid()) {
                    qWarning("Invalid property: %s:%s", qPrintable(tp), qPrintable(*it));
                }

                stream << prop;
            }
        }
    }
}

void KWidgetStreamer::propertyFromStream(QDataStream &stream, QObject *to)
{
    // Only handle widgets. Alternatives to widgets are layouts, validators, timers, etc.
    if (!to->inherits("QWidget")) {
        return;
    }

    // Stream in all the children (if any)
    const QList<QObject *> children = to->children();
    // const QObjectList* children = to->children();

    /*  unsigned int count;

      stream >> count;

      Q_ASSERT( count == 0 );
    */

    for (int i = 0; i < children.size(); ++i) {
        fromStream(stream, children.at(i));
    }

    // Now stream in properties
    for (PropertyMapIt mapIt = _map.constBegin(); mapIt != _map.constEnd(); ++mapIt) {
        QString tp = mapIt.key();
        PropertyList list = mapIt.value();
        if (to->inherits(tp.toLocal8Bit().data())) {
            for (PropertyListIt it = list.begin(); it != list.end(); ++it) {
                QVariant value;
                stream >> value;
                to->setProperty(it->toLocal8Bit().data(), value);
            }
        }
    }
}

KWidgetStreamer::KWidgetStreamer()
{
    QStringList l;

    // QCheckBox
    l.clear();
    l << QStringLiteral("enabled") << QStringLiteral("checked") << QStringLiteral("tristate");
    _map.insert(QStringLiteral("QCheckBox"), l);

    // QComboBox
    l.clear();
    l << QStringLiteral("enabled") << QStringLiteral("editable") << QStringLiteral("currentItem") << QStringLiteral("maxCount")
      << QStringLiteral("insertionPolicy") << QStringLiteral("autoCompletion");
    _map.insert(QStringLiteral("QComboBox"), l);

    // QDial
    l.clear();
    l << QStringLiteral("enabled") << QStringLiteral("tracking") << QStringLiteral("wrapping") << QStringLiteral("value");
    _map.insert(QStringLiteral("QDial"), l);

    // QLCDNumber
    l.clear();
    l << QStringLiteral("enabled") << QStringLiteral("digitCount") << QStringLiteral("mode") << QStringLiteral("segmentStyle") << QStringLiteral("value");
    _map.insert(QStringLiteral("QLCDNumber"), l);

    // QLineEdit
    l.clear();
    l << QStringLiteral("enabled") << QStringLiteral("text") << QStringLiteral("maxLength") << QStringLiteral("echoMode") << QStringLiteral("alignment");
    _map.insert(QStringLiteral("QLineEdit"), l);

    // QMultiLineEdit
    l.clear();
    l << QStringLiteral("enabled") << QStringLiteral("plainText");
    _map.insert(QStringLiteral("QTextEdit"), l);

    // QRadioButton
    l.clear();
    l << QStringLiteral("enabled") << QStringLiteral("checked");
    _map.insert(QStringLiteral("QRadioButton"), l);

    // QSlider
    l.clear();
    l << QStringLiteral("enabled") << QStringLiteral("value");
    _map.insert(QStringLiteral("QSlider"), l);

    // QSpinBox
    l.clear();
    l << QStringLiteral("enabled") << QStringLiteral("value");
    _map.insert(QStringLiteral("QSpinBox"), l);
}
