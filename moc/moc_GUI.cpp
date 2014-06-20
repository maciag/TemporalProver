/****************************************************************************
** Meta object code from reading C++ file 'GUI.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gui/GUI.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GUI.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QmlBridge_t {
    QByteArrayData data[22];
    char stringdata[291];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_QmlBridge_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_QmlBridge_t qt_meta_stringdata_QmlBridge = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 8),
QT_MOC_LITERAL(2, 19, 0),
QT_MOC_LITERAL(3, 20, 7),
QT_MOC_LITERAL(4, 28, 8),
QT_MOC_LITERAL(5, 37, 7),
QT_MOC_LITERAL(6, 45, 19),
QT_MOC_LITERAL(7, 65, 12),
QT_MOC_LITERAL(8, 78, 9),
QT_MOC_LITERAL(9, 88, 13),
QT_MOC_LITERAL(10, 102, 16),
QT_MOC_LITERAL(11, 119, 2),
QT_MOC_LITERAL(12, 122, 13),
QT_MOC_LITERAL(13, 136, 13),
QT_MOC_LITERAL(14, 150, 17),
QT_MOC_LITERAL(15, 168, 16),
QT_MOC_LITERAL(16, 185, 21),
QT_MOC_LITERAL(17, 207, 22),
QT_MOC_LITERAL(18, 230, 12),
QT_MOC_LITERAL(19, 243, 15),
QT_MOC_LITERAL(20, 259, 17),
QT_MOC_LITERAL(21, 277, 12)
    },
    "QmlBridge\0validate\0\0formula\0toPrefix\0"
    "toInfix\0getOperatorArgCount\0getErrorCode\0"
    "ErrorCode\0getErrorToken\0getErrorPosition\0"
    "OK\0UnexpectedEnd\0UnexpectedVar\0"
    "UnexpectedBinOper\0UnexpectedUnOper\0"
    "UnexpectedLeftBracket\0UnexpectedRightBracket\0"
    "InvalidToken\0BracketUnclosed\0"
    "SpareRightBracket\0UnknownError\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QmlBridge[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       1,   64, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x02,
       4,    1,   52,    2, 0x02,
       5,    1,   55,    2, 0x02,
       6,    1,   58,    2, 0x02,
       7,    0,   61,    2, 0x02,
       9,    0,   62,    2, 0x02,
      10,    0,   63,    2, 0x02,

 // methods: parameters
    QMetaType::Bool, QMetaType::QString,    3,
    QMetaType::QString, QMetaType::QString,    3,
    QMetaType::QString, QMetaType::QString,    3,
    QMetaType::Int, QMetaType::QString,    3,
    0x80000000 | 8,
    QMetaType::QString,
    QMetaType::Int,

 // enums: name, flags, count, data
       8, 0x0,   11,   68,

 // enum data: key, value
      11, uint(QmlBridge::OK),
      12, uint(QmlBridge::UnexpectedEnd),
      13, uint(QmlBridge::UnexpectedVar),
      14, uint(QmlBridge::UnexpectedBinOper),
      15, uint(QmlBridge::UnexpectedUnOper),
      16, uint(QmlBridge::UnexpectedLeftBracket),
      17, uint(QmlBridge::UnexpectedRightBracket),
      18, uint(QmlBridge::InvalidToken),
      19, uint(QmlBridge::BracketUnclosed),
      20, uint(QmlBridge::SpareRightBracket),
      21, uint(QmlBridge::UnknownError),

       0        // eod
};

void QmlBridge::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QmlBridge *_t = static_cast<QmlBridge *>(_o);
        switch (_id) {
        case 0: { bool _r = _t->validate((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: { QString _r = _t->toPrefix((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 2: { QString _r = _t->toInfix((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 3: { int _r = _t->getOperatorArgCount((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: { ErrorCode _r = _t->getErrorCode();
            if (_a[0]) *reinterpret_cast< ErrorCode*>(_a[0]) = _r; }  break;
        case 5: { QString _r = _t->getErrorToken();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 6: { int _r = _t->getErrorPosition();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject QmlBridge::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QmlBridge.data,
      qt_meta_data_QmlBridge,  qt_static_metacall, 0, 0}
};


const QMetaObject *QmlBridge::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QmlBridge::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QmlBridge.stringdata))
        return static_cast<void*>(const_cast< QmlBridge*>(this));
    return QObject::qt_metacast(_clname);
}

int QmlBridge::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
