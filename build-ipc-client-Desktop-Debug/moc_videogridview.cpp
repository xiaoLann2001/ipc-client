/****************************************************************************
** Meta object code from reading C++ file 'videogridview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../ipc-client/src/video/videoview/videogridview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videogridview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VideoGridView_t {
    QByteArrayData data[12];
    char stringdata0[191];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoGridView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoGridView_t qt_meta_stringdata_VideoGridView = {
    {
QT_MOC_LITERAL(0, 0, 13), // "VideoGridView"
QT_MOC_LITERAL(1, 14, 25), // "onVideoDisplayUnitClicked"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 19), // "id_videodisplayunit"
QT_MOC_LITERAL(4, 61, 30), // "onVideoDisplayUnitRightClicked"
QT_MOC_LITERAL(5, 92, 3), // "pos"
QT_MOC_LITERAL(6, 96, 42), // "onVideoDisplayUnitRequestMaxi..."
QT_MOC_LITERAL(7, 139, 11), // "onVideoPlay"
QT_MOC_LITERAL(8, 151, 5), // "index"
QT_MOC_LITERAL(9, 157, 11), // "onVideoStop"
QT_MOC_LITERAL(10, 169, 15), // "onVideoSetImage"
QT_MOC_LITERAL(11, 185, 5) // "image"

    },
    "VideoGridView\0onVideoDisplayUnitClicked\0"
    "\0id_videodisplayunit\0"
    "onVideoDisplayUnitRightClicked\0pos\0"
    "onVideoDisplayUnitRequestMaximizeOrRestore\0"
    "onVideoPlay\0index\0onVideoStop\0"
    "onVideoSetImage\0image"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoGridView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x0a /* Public */,
       4,    2,   47,    2, 0x0a /* Public */,
       6,    1,   52,    2, 0x0a /* Public */,
       7,    1,   55,    2, 0x0a /* Public */,
       9,    1,   58,    2, 0x0a /* Public */,
      10,    2,   61,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::QPoint,    3,    5,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::QImage,    8,   11,

       0        // eod
};

void VideoGridView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoGridView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onVideoDisplayUnitClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->onVideoDisplayUnitRightClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QPoint(*)>(_a[2]))); break;
        case 2: _t->onVideoDisplayUnitRequestMaximizeOrRestore((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onVideoPlay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->onVideoStop((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onVideoSetImage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QImage(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VideoGridView::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_VideoGridView.data,
    qt_meta_data_VideoGridView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VideoGridView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoGridView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoGridView.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int VideoGridView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
