/*
 *  SPDX-FileCopyrightText: 2014 Pino Toscano <pino@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 */

#pragma once

#include "regexp.h"

#include <QList>

struct parse_context {
    RegExp *result = nullptr;
    int nerrs;
    QList<QString> backrefs;
};
