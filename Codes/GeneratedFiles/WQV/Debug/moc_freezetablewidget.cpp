/****************************************************************************
** Meta object code from reading C++ file 'freezetablewidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../GUI/freezetablewidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'freezetablewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FreezeTableWidget_t {
    QByteArrayData data[13];
    char stringdata0[166];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FreezeTableWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FreezeTableWidget_t qt_meta_stringdata_FreezeTableWidget = {
    {
QT_MOC_LITERAL(0, 0, 17), // "FreezeTableWidget"
QT_MOC_LITERAL(1, 18, 18), // "updateSectionWidth"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 12), // "logicalIndex"
QT_MOC_LITERAL(4, 51, 7), // "oldSize"
QT_MOC_LITERAL(5, 59, 7), // "newSize"
QT_MOC_LITERAL(6, 67, 24), // "updateSectionWidthSecond"
QT_MOC_LITERAL(7, 92, 19), // "updateSectionHeight"
QT_MOC_LITERAL(8, 112, 13), // "onRowsRemoved"
QT_MOC_LITERAL(9, 126, 6), // "parent"
QT_MOC_LITERAL(10, 133, 5), // "first"
QT_MOC_LITERAL(11, 139, 4), // "last"
QT_MOC_LITERAL(12, 144, 21) // "onWrongSymbolDetected"

    },
    "FreezeTableWidget\0updateSectionWidth\0"
    "\0logicalIndex\0oldSize\0newSize\0"
    "updateSectionWidthSecond\0updateSectionHeight\0"
    "onRowsRemoved\0parent\0first\0last\0"
    "onWrongSymbolDetected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FreezeTableWidget[] = {

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
       1,    3,   39,    2, 0x08 /* Private */,
       6,    3,   46,    2, 0x08 /* Private */,
       7,    3,   53,    2, 0x08 /* Private */,
       8,    3,   60,    2, 0x08 /* Private */,
      12,    2,   67,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::QModelIndex, QMetaType::Int, QMetaType::Int,    9,   10,   11,
    QMetaType::Void, QMetaType::QModelIndex, QMetaType::QString,    2,    2,

       0        // eod
};

void FreezeTableWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FreezeTableWidget *_t = static_cast<FreezeTableWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateSectionWidth((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->updateSectionWidthSecond((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->updateSectionHeight((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: _t->onRowsRemoved((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 4: _t->onWrongSymbolDetected((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject FreezeTableWidget::staticMetaObject = {
    { &QTableView::staticMetaObject, qt_meta_stringdata_FreezeTableWidget.data,
      qt_meta_data_FreezeTableWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FreezeTableWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FreezeTableWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FreezeTableWidget.stringdata0))
        return static_cast<void*>(const_cast< FreezeTableWidget*>(this));
    return QTableView::qt_metacast(_clname);
}

int FreezeTableWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableView::qt_metacall(_c, _id, _a);
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
