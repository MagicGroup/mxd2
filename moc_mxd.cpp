/****************************************************************************
** Meta object code from reading C++ file 'mxd.h'
**
** Created: Wed Oct 6 10:59:02 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mxd.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mxd.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_myclass[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x08,
      22,    8,    8,    8, 0x08,
      34,    8,    8,    8, 0x08,
      43,    8,    8,    8, 0x08,
      55,    8,    8,    8, 0x08,
      63,    8,    8,    8, 0x08,
      81,   74,    8,    8, 0x08,
     130,    8,    8,    8, 0x08,
     143,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_myclass[] = {
    "myclass\0\0pppoeStart()\0pppoeStop()\0"
    "Netset()\0linkStart()\0showM()\0saveconf()\0"
    "reason\0iconActivated(QSystemTrayIcon::ActivationReason)\0"
    "updateIcon()\0hideme()\0"
};

const QMetaObject myclass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_myclass,
      qt_meta_data_myclass, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &myclass::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *myclass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *myclass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_myclass))
        return static_cast<void*>(const_cast< myclass*>(this));
    return QWidget::qt_metacast(_clname);
}

int myclass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: pppoeStart(); break;
        case 1: pppoeStop(); break;
        case 2: Netset(); break;
        case 3: linkStart(); break;
        case 4: showM(); break;
        case 5: saveconf(); break;
        case 6: iconActivated((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 7: updateIcon(); break;
        case 8: hideme(); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
