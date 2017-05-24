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

#include <KPluginFactory>
#include <KPluginLoader>

#include "kregexpeditorgui.h"
#include "kregexpeditorguidialog.h"

K_PLUGIN_FACTORY(KRegExpEditorPluginFactory,
                 registerPlugin<KRegExpEditorGUI>("widget");
                 registerPlugin<KRegExpEditorGUIDialog>("dialog");
                 registerPlugin<KRegExpEditorGUIDialog>();
                 )
K_EXPORT_PLUGIN(KRegExpEditorPluginFactory("kregexpeditorgui"))

#include "kregexpeditorfactory.moc"
