/****************************************************************************
** Meta object code from reading C++ file 'main_window.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/main_window.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMyTextEditENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSMyTextEditENDCLASS = QtMocHelpers::stringData(
    "MyTextEdit",
    "enter_signal",
    ""
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMyTextEditENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   20,    2, 0x06,    1 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MyTextEdit::staticMetaObject = { {
    QMetaObject::SuperData::link<QTextEdit::staticMetaObject>(),
    qt_meta_stringdata_CLASSMyTextEditENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMyTextEditENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMyTextEditENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MyTextEdit, std::true_type>,
        // method 'enter_signal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MyTextEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MyTextEdit *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->enter_signal(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MyTextEdit::*)();
            if (_t _q_method = &MyTextEdit::enter_signal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject *MyTextEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyTextEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMyTextEditENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QTextEdit::qt_metacast(_clname);
}

int MyTextEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void MyTextEdit::enter_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSmain_windowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSmain_windowENDCLASS = QtMocHelpers::stringData(
    "main_window",
    "start_audio_stream",
    "",
    "start_demodulation_service",
    "stop_audio_stream",
    "stop_demodulation_service",
    "send_msg",
    "send_file",
    "receiving_packet",
    "received",
    "packet_length",
    "receive_packet",
    "translate_packet",
    "packet*",
    "p",
    "export_log",
    "config_changed",
    "show_info",
    "show_config",
    "debug"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSmain_windowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   98,    2, 0x0a,    1 /* Public */,
       3,    0,   99,    2, 0x0a,    2 /* Public */,
       4,    0,  100,    2, 0x0a,    3 /* Public */,
       5,    0,  101,    2, 0x0a,    4 /* Public */,
       6,    0,  102,    2, 0x0a,    5 /* Public */,
       7,    0,  103,    2, 0x0a,    6 /* Public */,
       8,    2,  104,    2, 0x0a,    7 /* Public */,
      11,    0,  109,    2, 0x0a,   10 /* Public */,
      12,    1,  110,    2, 0x0a,   11 /* Public */,
      15,    0,  113,    2, 0x0a,   13 /* Public */,
      16,    0,  114,    2, 0x0a,   14 /* Public */,
      17,    0,  115,    2, 0x0a,   15 /* Public */,
      18,    0,  116,    2, 0x0a,   16 /* Public */,
      19,    0,  117,    2, 0x0a,   17 /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    9,   10,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject main_window::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSmain_windowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSmain_windowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSmain_windowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<main_window, std::true_type>,
        // method 'start_audio_stream'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'start_demodulation_service'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stop_audio_stream'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stop_demodulation_service'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'send_msg'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'send_file'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'receiving_packet'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'receive_packet'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'translate_packet'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<packet *, std::false_type>,
        // method 'export_log'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'config_changed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'show_info'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'show_config'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'debug'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void main_window::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<main_window *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->start_audio_stream(); break;
        case 1: _t->start_demodulation_service(); break;
        case 2: _t->stop_audio_stream(); break;
        case 3: _t->stop_demodulation_service(); break;
        case 4: _t->send_msg(); break;
        case 5: _t->send_file(); break;
        case 6: _t->receiving_packet((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 7: _t->receive_packet(); break;
        case 8: _t->translate_packet((*reinterpret_cast< std::add_pointer_t<packet*>>(_a[1]))); break;
        case 9: _t->export_log(); break;
        case 10: _t->config_changed(); break;
        case 11: _t->show_info(); break;
        case 12: _t->show_config(); break;
        case 13: _t->debug(); break;
        default: ;
        }
    }
}

const QMetaObject *main_window::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *main_window::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSmain_windowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int main_window::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
