/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#ifndef __kwidgetstreamer
#define __kwidgetstreamer

#include <QMap>
#include <QStringList>

class QObject;

/**
   This class defines methods for streaming widget data.

   For each widget type rules are defined telling which attributes should be
   streamed. If you need to stream other attributes or to avoid streaming
   certain attributes, then this may be obtained by editing the property
   map, which is obtained by calling @ref propertyMap. For further control
   inherit from this class and override @ref toStream and @ref fromStream.

   The following example shows how you can avoid streaming
   <tt>numDigits</tt> for a QLCDNumber. The same approach applies if you
   want to add extra properties to be streamed.
   <pre>
   KWidgetStreamer streamer;
   KWidgetStreamer::PropertyMap& map = streamer.propertyMap();
   KWidgetStreamer::PropertyList& list = *map.find("QLCDNumber");
   list.remove("numDigits");
   </pre>
**/
class KWidgetStreamer
{
public:
    typedef QStringList PropertyList;
    typedef QMap< QString, PropertyList > PropertyMap;
    typedef QMap< QString, PropertyList >::ConstIterator PropertyMapIt;
    typedef QStringList::Iterator PropertyListIt;

    KWidgetStreamer();
    virtual ~KWidgetStreamer()
    {
    }

    virtual void toStream(const QObject *from, QDataStream &stream);
    virtual void fromStream(QDataStream &stream, QObject *to);

    PropertyMap &propertyMap()
    {
        return _map;
    }

protected:
    void propertyToStream(const QObject *from, QDataStream &stream);
    void propertyFromStream(QDataStream &stream, QObject *to);

private:
    PropertyMap _map;
};

#endif /* __kwidgetstreamer */
