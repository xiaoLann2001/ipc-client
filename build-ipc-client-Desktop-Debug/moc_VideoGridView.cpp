/****************************************************************************
** Meta object code from reading C++ file 'VideoGridView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../ipc-client/src/video/VideoView/VideoGridView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoGridView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VideoGridView_t {
    QByteArrayData data[13];
    char stringdata0[213];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoGridView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoGridView_t qt_meta_stringdata_VideoGridView = {
    {
QT_MOC_LITERAL(0, 0, 13), // "VideoGridView"
QT_MOC_LITERAL(1, 14, 23), // "videoDisplayUnitClicked"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 19), // "id_videodisplayunit"
QT_MOC_LITERAL(4, 59, 28), // "videoDisplayUnitRightClicked"
QT_MOC_LITERAL(5, 88, 3), // "pos"
QT_MOC_LITERAL(6, 92, 25), // "onVideoDisplayUnitClicked"
QT_MOC_LITERAL(7, 118, 42), // "onVideoDisplayUnitRequestMaxi..."
QT_MOC_LITERAL(8, 161, 11), // "onVideoPlay"
QT_MOC_LITERAL(9, 173, 5), // "index"
QT_MOC_LITERAL(10, 179, 11), // "onVideoStop"
QT_MOC_LITERAL(11, 191, 15), // "onVideoSetImage"
QT_MOC_LITERAL(12, 207, 5) // "image"

    },
    "VideoGridView\0videoDisplayUnitClicked\0"
    "\0id_videodisplayunit\0videoDisplayUnitRightClicked\0"
    "pos\0onVideoDisplayUnitClicked\0"
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
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    2,   52,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   57,    2, 0x0a /* Public */,
       7,    1,   60,    2, 0x0a /* Public */,
       8,    1,   63,    2, 0x0a /* Public */,
      10,    1,   66,    2, 0x0a /* Public */,
      11,    2,   69,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::QPoint,    3,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::QImage,    9,   12,

       0        // eod
};

void VideoGridView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoGridView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->videoDisplayUnitClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->videoDisplayUnitRightClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QPoint(*)>(_a[2]))); break;
        case 2: _t->onVideoDisplayUnitClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onVideoDisplayUnitRequestMaximizeOrRestore((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->onVideoPlay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onVideoStop((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->onVideoSetImage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QImage(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VideoGridView::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoGridView::videoDisplayUnitClicked)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VideoGridView::*)(int , const QPoint & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoGridView::videoDisplayUnitRightClicked)) {
                *result = 1;
                return;
            }
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

// SIGNAL 0
void VideoGridView::videoDisplayUnitClicked(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VideoGridView::videoDisplayUnitRightClicked(int _t1, const QPoint & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
