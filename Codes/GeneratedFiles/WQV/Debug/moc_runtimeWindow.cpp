/****************************************************************************
** Meta object code from reading C++ file 'runtimeWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../GUI/runtimeWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'runtimeWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_runtimeWindow_t {
    QByteArrayData data[17];
    char stringdata0[201];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_runtimeWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_runtimeWindow_t qt_meta_stringdata_runtimeWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "runtimeWindow"
QT_MOC_LITERAL(1, 14, 18), // "on_btnStop_clicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 18), // "on_button1_clicked"
QT_MOC_LITERAL(4, 53, 16), // "realtimeDataSlot"
QT_MOC_LITERAL(5, 70, 1), // "x"
QT_MOC_LITERAL(6, 72, 1), // "y"
QT_MOC_LITERAL(7, 74, 10), // "secondPlot"
QT_MOC_LITERAL(8, 85, 9), // "chartName"
QT_MOC_LITERAL(9, 95, 15), // "bracketDataSlot"
QT_MOC_LITERAL(10, 111, 10), // "screenShot"
QT_MOC_LITERAL(11, 122, 14), // "allScreenShots"
QT_MOC_LITERAL(12, 137, 16), // "contextMenuEvent"
QT_MOC_LITERAL(13, 154, 18), // "QContextMenuEvent*"
QT_MOC_LITERAL(14, 173, 5), // "event"
QT_MOC_LITERAL(15, 179, 8), // "showMenu"
QT_MOC_LITERAL(16, 188, 12) // "QMouseEvent*"

    },
    "runtimeWindow\0on_btnStop_clicked\0\0"
    "on_button1_clicked\0realtimeDataSlot\0"
    "x\0y\0secondPlot\0chartName\0bracketDataSlot\0"
    "screenShot\0allScreenShots\0contextMenuEvent\0"
    "QContextMenuEvent*\0event\0showMenu\0"
    "QMouseEvent*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_runtimeWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    0,   65,    2, 0x08 /* Private */,
       4,    4,   66,    2, 0x08 /* Private */,
       4,    3,   75,    2, 0x28 /* Private | MethodCloned */,
       4,    2,   82,    2, 0x28 /* Private | MethodCloned */,
       9,    0,   87,    2, 0x08 /* Private */,
      10,    0,   88,    2, 0x08 /* Private */,
      11,    0,   89,    2, 0x08 /* Private */,
      12,    1,   90,    2, 0x08 /* Private */,
      15,    1,   93,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Bool, QMetaType::QString,    5,    6,    7,    8,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Bool,    5,    6,    7,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    5,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, 0x80000000 | 16,   14,

       0        // eod
};

void runtimeWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        runtimeWindow *_t = static_cast<runtimeWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_btnStop_clicked(); break;
        case 1: _t->on_button1_clicked(); break;
        case 2: _t->realtimeDataSlot((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 3: _t->realtimeDataSlot((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 4: _t->realtimeDataSlot((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 5: _t->bracketDataSlot(); break;
        case 6: _t->screenShot(); break;
        case 7: _t->allScreenShots(); break;
        case 8: _t->contextMenuEvent((*reinterpret_cast< QContextMenuEvent*(*)>(_a[1]))); break;
        case 9: _t->showMenu((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject runtimeWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_runtimeWindow.data,
      qt_meta_data_runtimeWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *runtimeWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *runtimeWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_runtimeWindow.stringdata0))
        return static_cast<void*>(const_cast< runtimeWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int runtimeWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
