/****************************************************************************
** Meta object code from reading C++ file 'VideoStreamController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../ipc-client/src/Video/VideoStreamController/VideoStreamController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoStreamController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VideoStreamController_t {
    QByteArrayData data[16];
    char stringdata0[227];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoStreamController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoStreamController_t qt_meta_stringdata_VideoStreamController = {
    {
QT_MOC_LITERAL(0, 0, 21), // "VideoStreamController"
QT_MOC_LITERAL(1, 22, 16), // "onAddVideoStream"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 18), // "videoDisplayUnitId"
QT_MOC_LITERAL(4, 59, 3), // "url"
QT_MOC_LITERAL(5, 63, 18), // "onStopVideoDisplay"
QT_MOC_LITERAL(6, 82, 11), // "onOutOfGrid"
QT_MOC_LITERAL(7, 94, 18), // "onVideoGridChanged"
QT_MOC_LITERAL(8, 113, 4), // "page"
QT_MOC_LITERAL(9, 118, 18), // "onVideoPageChanged"
QT_MOC_LITERAL(10, 137, 15), // "onAddIPCClicked"
QT_MOC_LITERAL(11, 153, 18), // "onVideoViewClicked"
QT_MOC_LITERAL(12, 172, 23), // "onVideoViewRightClicked"
QT_MOC_LITERAL(13, 196, 3), // "pos"
QT_MOC_LITERAL(14, 200, 19), // "onNewFrameAvailable"
QT_MOC_LITERAL(15, 220, 6) // "handle"

    },
    "VideoStreamController\0onAddVideoStream\0"
    "\0videoDisplayUnitId\0url\0onStopVideoDisplay\0"
    "onOutOfGrid\0onVideoGridChanged\0page\0"
    "onVideoPageChanged\0onAddIPCClicked\0"
    "onVideoViewClicked\0onVideoViewRightClicked\0"
    "pos\0onNewFrameAvailable\0handle"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoStreamController[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   59,    2, 0x0a /* Public */,
       5,    1,   64,    2, 0x0a /* Public */,
       6,    1,   67,    2, 0x0a /* Public */,
       7,    1,   70,    2, 0x0a /* Public */,
       9,    1,   73,    2, 0x0a /* Public */,
      10,    0,   76,    2, 0x0a /* Public */,
      11,    1,   77,    2, 0x0a /* Public */,
      12,    2,   80,    2, 0x0a /* Public */,
      14,    1,   85,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::QPoint,    3,   13,
    QMetaType::Void, QMetaType::Int,   15,

       0        // eod
};

void VideoStreamController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoStreamController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onAddVideoStream((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->onStopVideoDisplay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onOutOfGrid((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onVideoGridChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->onVideoPageChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onAddIPCClicked(); break;
        case 6: _t->onVideoViewClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->onVideoViewRightClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QPoint(*)>(_a[2]))); break;
        case 8: _t->onNewFrameAvailable((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VideoStreamController::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_VideoStreamController.data,
    qt_meta_data_VideoStreamController,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VideoStreamController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoStreamController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoStreamController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VideoStreamController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
