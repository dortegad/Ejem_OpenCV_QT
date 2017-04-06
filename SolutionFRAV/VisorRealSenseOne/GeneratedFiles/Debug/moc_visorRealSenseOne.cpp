/****************************************************************************
** Meta object code from reading C++ file 'visorRealSenseOne.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../visorRealSenseOne.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'visorRealSenseOne.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VisorRealSenseOne_t {
    QByteArrayData data[7];
    char stringdata0[52];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VisorRealSenseOne_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VisorRealSenseOne_t qt_meta_stringdata_VisorRealSenseOne = {
    {
QT_MOC_LITERAL(0, 0, 17), // "VisorRealSenseOne"
QT_MOC_LITERAL(1, 18, 4), // "view"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 6), // "record"
QT_MOC_LITERAL(4, 31, 4), // "play"
QT_MOC_LITERAL(5, 36, 4), // "stop"
QT_MOC_LITERAL(6, 41, 10) // "selectFile"

    },
    "VisorRealSenseOne\0view\0\0record\0play\0"
    "stop\0selectFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VisorRealSenseOne[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x0a /* Public */,
       3,    0,   40,    2, 0x0a /* Public */,
       4,    0,   41,    2, 0x0a /* Public */,
       5,    0,   42,    2, 0x0a /* Public */,
       6,    0,   43,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void VisorRealSenseOne::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VisorRealSenseOne *_t = static_cast<VisorRealSenseOne *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { int _r = _t->view();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: { int _r = _t->record();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: { int _r = _t->play();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: _t->stop(); break;
        case 4: _t->selectFile(); break;
        default: ;
        }
    }
}

const QMetaObject VisorRealSenseOne::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_VisorRealSenseOne.data,
      qt_meta_data_VisorRealSenseOne,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VisorRealSenseOne::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VisorRealSenseOne::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VisorRealSenseOne.stringdata0))
        return static_cast<void*>(const_cast< VisorRealSenseOne*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int VisorRealSenseOne::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
