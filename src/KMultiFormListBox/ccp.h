/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/
#ifndef __ccp
#define __ccp

#include <QObject>

class KMultiFormListBoxMultiVisible;
class KMultiFormListBoxEntry;

/**
   Helper class for @ref KMultiFormListBoxMultiVisible which is used to install EventFilters.

   When the user presses CTRL + right mouse button then a menu should
   appear which offers him cut and paste capabilities for the entries in
   the KMultiFormListBoxMultiVisible.

  To obtain this an event filter must be install for each subwidget of the
  KMultiFormListBoxMultiVisible. This event filter must catch the right mouse press event and
  post the menu. This requires a widget which has the method @ref
  eventFilter defined. We have this helper class exactly for this purpose.

  For each @ref KMultiFormListBoxEntry in the @ref KMultiFormListBoxMultiVisible widget an instance of
  this class is associated.

  CCP stand for Cut Copy and Paste

  @internal
**/
class CCP : public QObject
{
    Q_OBJECT
private:
    friend class KMultiFormListBoxMultiVisible;
    /**
     * Constructor is private so only the class @ref KMultiFormListBoxMultiVisible may create an
     * instance of this widget.
     **/
    CCP(KMultiFormListBoxMultiVisible *, KMultiFormListBoxEntry *);

    void install(QObject *);
    bool eventFilter(QObject *, QEvent *) override;

    // Instance variables.

    KMultiFormListBoxMultiVisible *ee = nullptr;
    KMultiFormListBoxEntry *eee = nullptr;
};

#endif /* ccp */
