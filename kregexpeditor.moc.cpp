/****************************************************************************
** KRegExpEditor meta object code from reading C++ file 'kregexpeditor.h'
**
** Created: Tue Aug 21 22:02:21 2001
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 9
#elif Q_MOC_OUTPUT_REVISION != 9
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "kregexpeditor.h"
#include <qmetaobject.h>
#include <qapplication.h>



const char *KRegExpEditor::className() const
{
    return "KRegExpEditor";
}

QMetaObject *KRegExpEditor::metaObj = 0;

void KRegExpEditor::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("KRegExpEditor","QWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString KRegExpEditor::tr(const char* s)
{
    return qApp->translate( "KRegExpEditor", s, 0 );
}

QString KRegExpEditor::tr(const char* s, const char * c)
{
    return qApp->translate( "KRegExpEditor", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* KRegExpEditor::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void (KRegExpEditor::*m1_t0)();
    typedef void (QObject::*om1_t0)();
    typedef void (KRegExpEditor::*m1_t1)();
    typedef void (QObject::*om1_t1)();
    m1_t0 v1_0 = &KRegExpEditor::slotRedo;
    om1_t0 ov1_0 = (om1_t0)v1_0;
    m1_t1 v1_1 = &KRegExpEditor::slotUndo;
    om1_t1 ov1_1 = (om1_t1)v1_1;
    QMetaData *slot_tbl = QMetaObject::new_metadata(2);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(2);
    slot_tbl[0].name = "slotRedo()";
    slot_tbl[0].ptr = (QMember)ov1_0;
    slot_tbl_access[0] = QMetaData::Public;
    slot_tbl[1].name = "slotUndo()";
    slot_tbl[1].ptr = (QMember)ov1_1;
    slot_tbl_access[1] = QMetaData::Public;
    metaObj = QMetaObject::new_metaobject(
	"KRegExpEditor", "QWidget",
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    metaObj->set_slot_access( slot_tbl_access );
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    return metaObj;
}
