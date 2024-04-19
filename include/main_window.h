#pragma once
#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QStatusBar>
#include <QTextEdit>
#include <QBoxLayout>
#include <QKeyEvent>
#include <QtCore/QTimer>
#include <QScrollBar>
#include <random>
#include "audio_modem.h"
#include "info_window.h"
#include "config_window.h"

class MyTextEdit : public QTextEdit {
    Q_OBJECT

public:
    MyTextEdit(QWidget* obj) : QTextEdit(obj) {}

    void keyPressEvent(QKeyEvent* event) {
        if (event->key() == Qt::Key_Return)
        {
            emit enter_signal();
        }
        else
            QTextEdit::keyPressEvent(event);
    }

signals:
    void enter_signal();
};

class main_window : public QWidget
{
    Q_OBJECT

public:
    main_window(QWidget *parent = nullptr);
    ~main_window();

    struct outstream { QTextEdit* q; } cout;
    struct _time_ {} time;

private:
    audio_modem modem;
    std::mt19937 engine;

    QBoxLayout* hLayout, *vLayout;
    QMenuBar* menuBar;
    QMenu* menuFile, * menuModem, * menuHelp, * menuSettings;
    QStatusBar* statusBar;
    QPushButton* buttonSend, *buttonFile;
    QTextEdit* textLog;
    MyTextEdit* textInput;
    QAction* actionFile, * actionExport;
    QAction* actionModemDevice, * actionConfig;
    QVector<QAction*> actionDevices;
    QAction* actionInfo;
    QAction* actionStartStream, * actionStopStream, * actionStartDemod, * actionStopDemod;
    info_window* windowInfo;
    config_window* windowConfig;

public slots:
    void start_audio_stream();
    void start_demodulation_service();
    void stop_audio_stream();
    void stop_demodulation_service();
    void send_msg();
    void send_file();
    void receiving_packet(int received, int packet_length);
    void receive_packet();
    void translate_packet(packet* p);
    void export_log();
    void config_changed();
    void show_info();
    void show_config();

    void debug();
};

template <typename T>
main_window::outstream& operator<<(main_window::outstream& os, const T& data) {
    QTextCursor cursor(os.q->textCursor());
    cursor.movePosition(QTextCursor::End);
    os.q->setTextCursor(cursor);

    if constexpr (std::is_same<T, main_window::_time_>::value == true) {
        time_t timer = std::time(NULL);
        tm t;
        localtime_s(&t, &timer);

        os.q->insertPlainText(QString::number(t.tm_hour) + "h/" + QString::number(t.tm_min) + "m/" + QString::number(t.tm_sec) + "s");
    }

    else if constexpr(std::is_arithmetic<T>::value == true)
        os.q->insertPlainText(QString::number(data));

    else if constexpr (std::is_same<T, std::string>::value == true) {
        os.q->insertPlainText(data.c_str());
    }

    else
        os.q->insertPlainText(data);

    auto scroll = os.q->verticalScrollBar();
    scroll->setValue(scroll->maximum());

    return os;
}