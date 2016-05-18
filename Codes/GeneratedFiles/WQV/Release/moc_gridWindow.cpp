/****************************************************************************
** Meta object code from reading C++ file 'gridWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../GUI/gridWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gridWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_gridWindow_t {
    QByteArrayData data[19];
    char stringdata0[271];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_gridWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_gridWindow_t qt_meta_stringdata_gridWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "gridWindow"
QT_MOC_LITERAL(1, 11, 6), // "accept"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 6), // "reject"
QT_MOC_LITERAL(4, 26, 27), // "on_columnsSpin_valueChanged"
QT_MOC_LITERAL(5, 54, 7), // "columns"
QT_MOC_LITERAL(6, 62, 24), // "on_rowsSpin_valueChanged"
QT_MOC_LITERAL(7, 87, 4), // "rows"
QT_MOC_LITERAL(8, 92, 20), // "on_buttonBox_clicked"
QT_MOC_LITERAL(9, 113, 16), // "QAbstractButton*"
QT_MOC_LITERAL(10, 130, 6), // "button"
QT_MOC_LITERAL(11, 137, 23), // "on_radioButtonH_clicked"
QT_MOC_LITERAL(12, 161, 7), // "checked"
QT_MOC_LITERAL(13, 169, 23), // "on_radioButtonV_clicked"
QT_MOC_LITERAL(14, 193, 23), // "on_radioButtonH_toggled"
QT_MOC_LITERAL(15, 217, 23), // "on_radioButtonV_toggled"
QT_MOC_LITERAL(16, 241, 10), // "closeEvent"
QT_MOC_LITERAL(17, 252, 12), // "QCloseEvent*"
QT_MOC_LITERAL(18, 265, 5) // "event"

    },
    "gridWindow\0accept\0\0reject\0"
    "on_columnsSpin_valueChanged\0columns\0"
    "on_rowsSpin_valueChanged\0rows\0"
    "on_buttonBox_clicked\0QAbstractButton*\0"
    "button\0on_radioButtonH_clicked\0checked\0"
    "on_radioButtonV_clicked\0on_radioButtonH_toggled\0"
    "on_radioButtonV_toggled\0closeEvent\0"
    "QCloseEvent*\0event"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_gridWindow[] = {

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
       4,    1,   66,    2, 0x08 /* Private */,
       6,    1,   69,    2, 0x08 /* Private */,
       8,    1,   72,    2, 0x08 /* Private */,
      11,    1,   75,    2, 0x08 /* Private */,
      13,    1,   78,    2, 0x08 /* Private */,
      14,    1,   81,    2, 0x08 /* Private */,
      15,    1,   84,    2, 0x08 /* Private */,
      16,    1,   87,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, 0x80000000 | 17,   18,

       0        // eod
};

void gridWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        gridWindow *_t = static_cast<gridWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->accept(); break;
        case 1: _t->reject(); break;
        case 2: _t->on_columnsSpin_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_rowsSpin_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_buttonBox_clicked((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 5: _t->on_radioButtonH_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->on_radioButtonV_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->on_radioButtonH_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->on_radioButtonV_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractButton* >(); break;
            }
            break;
        }
    }
}

const QMetaObject gridWindow::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_gridWindow.data,
      qt_meta_data_gridWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *gridWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *gridWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_gridWindow.stringdata0))
        return static_cast<void*>(const_cast< gridWindow*>(this));
    return QDialog::qt_metacast(_clname);
}

int gridWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
