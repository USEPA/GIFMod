/****************************************************************************
** Meta object code from reading C++ file 'reactiontablemodel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../GUI/reactiontablemodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'reactiontablemodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ReactionTableModel_t {
    QByteArrayData data[6];
    char stringdata0[74];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ReactionTableModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ReactionTableModel_t qt_meta_stringdata_ReactionTableModel = {
    {
QT_MOC_LITERAL(0, 0, 18), // "ReactionTableModel"
QT_MOC_LITERAL(1, 19, 25), // "invalidExpressoinDetected"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 3), // "msg"
QT_MOC_LITERAL(4, 50, 19), // "wrongSymbolDetected"
QT_MOC_LITERAL(5, 70, 3) // "sym"

    },
    "ReactionTableModel\0invalidExpressoinDetected\0"
    "\0msg\0wrongSymbolDetected\0sym"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ReactionTableModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       4,    2,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QModelIndex, QMetaType::QString,    2,    5,

       0        // eod
};

void ReactionTableModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ReactionTableModel *_t = static_cast<ReactionTableModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->invalidExpressoinDetected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->wrongSymbolDetected((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ReactionTableModel::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ReactionTableModel::invalidExpressoinDetected)) {
                *result = 0;
            }
        }
        {
            typedef void (ReactionTableModel::*_t)(const QModelIndex & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ReactionTableModel::wrongSymbolDetected)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject ReactionTableModel::staticMetaObject = {
    { &QAbstractTableModel::staticMetaObject, qt_meta_stringdata_ReactionTableModel.data,
      qt_meta_data_ReactionTableModel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ReactionTableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ReactionTableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ReactionTableModel.stringdata0))
        return static_cast<void*>(const_cast< ReactionTableModel*>(this));
    return QAbstractTableModel::qt_metacast(_clname);
}

int ReactionTableModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ReactionTableModel::invalidExpressoinDetected(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ReactionTableModel::wrongSymbolDetected(const QModelIndex & _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
