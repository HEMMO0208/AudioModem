#include "main_window.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <bitset>
#include "info_window.h"
#include "utils.h"

main_window::main_window(QWidget *parent)
    : QWidget(parent), modem(2048, 48000), engine(std::random_device{}())
{
    vLayout = new QVBoxLayout(this);
    hLayout = new QHBoxLayout(this);
    actionFile = new QAction("Open File", this);
    actionExport = new QAction("Export Log", this);
    actionStartStream = new QAction("Start Audio Stream", this);
    actionStopStream = new QAction("Stop Audio Stream", this);
    actionStartDemod = new QAction("Start Demodulation", this);
    actionStopDemod = new QAction("Stop Demodulation", this);
    actionInfo = new QAction("Information", this);
    actionModemDevice = new QAction("Modem Device", this);
    actionConfig = new QAction("Settings", this);
    menuBar = new QMenuBar(this);
    statusBar = new QStatusBar(this);
    buttonSend = new QPushButton(this);
    buttonFile = new QPushButton(this);
    textLog = new QTextEdit(this);
    textInput = new MyTextEdit(this);
    windowInfo = new info_window(this);
    windowConfig = new config_window(this, modem);

    resize(500, 400);
    setStyleSheet(QString("background-color: rgb(255, 255, 255);"));

    menuFile = menuBar->addMenu("File");
    menuFile->addAction(actionFile);
    menuFile->addAction(actionExport);
    menuModem = menuBar->addMenu("Modem");
    menuModem->addAction(actionStartStream);
    menuModem->addAction(actionStopStream);
    menuModem->addAction(actionStartDemod);
    menuModem->addAction(actionStopDemod);
    menuSettings = menuBar->addMenu("Settings");
    menuSettings->addAction(actionModemDevice);
    menuSettings->addAction(actionConfig);
    menuHelp = menuBar->addMenu("Help");
    menuHelp->addAction(actionInfo);

    buttonSend->setText("Send");
    buttonSend->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    buttonFile->setText("File");
    buttonFile->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    textLog->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    textLog->setReadOnly(true);
    cout.q = textLog;

    hLayout->addWidget(textInput, 5);
    hLayout->addWidget(buttonSend, 1);
    hLayout->addWidget(buttonFile, 1);
    vLayout->addWidget(menuBar, 1);
    vLayout->addWidget(textLog, 10);
    vLayout->addLayout(hLayout, 1);
    vLayout->addWidget(statusBar, 1);

    foreach(QWidget * widget, QApplication::allWidgets()) {
        widget->setFont(QFont("Arial", 10));
    }

    connect(buttonSend, &QPushButton::clicked, this, &main_window::send_msg);
    connect(buttonFile, &QPushButton::clicked, this, &main_window::send_file);
    connect(textInput, &MyTextEdit::enter_signal, this, &main_window::send_msg);
    connect(actionFile, &QAction::triggered, this, &main_window::send_file);
    connect(actionExport, &QAction::triggered, this, &main_window::export_log);
    connect(actionConfig, &QAction::triggered, this, &main_window::show_config);
    connect(actionStartStream, &QAction::triggered, this, &main_window::start_audio_stream);
    connect(actionStartDemod, &QAction::triggered, this, &main_window::start_demodulation_service);
    connect(actionStopStream, &QAction::triggered, this, &main_window::stop_audio_stream);
    connect(actionStopDemod, &QAction::triggered, this, &main_window::stop_demodulation_service);
    connect(actionInfo, &QAction::triggered, this, &main_window::show_info);

    connect(modem.get_signal(), &modem_signal_sender::packet_received, this, &main_window::receive_packet);
    connect(modem.get_signal(), &modem_signal_sender::configuration_changed, this, &main_window::config_changed);
    connect(modem.get_signal(), &modem_signal_sender::packet_receiving, this, &main_window::receiving_packet);
    connect(modem.get_signal(), &modem_signal_sender::packet_lost, this, [=]() { statusBar->showMessage("Packet lost.", 1000); });

    start_audio_stream();
    start_demodulation_service();
}

main_window::~main_window()
{
}

void main_window::start_audio_stream() {
    cout << "<System> Starting audio stream.\n";

    bool result = modem.start_stream();
    if (!result)
        cout << "<System> An error occured while staring audio stream.\n";
    else
        cout << "<System> Task complete.\n";
}

void main_window::start_demodulation_service() {
    cout << "<System> Starting demodulation service.\n";
    bool result = modem.start_demodulate();
    if (!result)
        cout << "<System> An error occured while staring demodulation service.\n";
    else
        cout << "<System> Task complete.\n";
}

void main_window::stop_audio_stream() {
    cout << "<System> Audio stream terminated.\n";

    modem.stop_stream();
}

void main_window::stop_demodulation_service() {
    cout << "<System> Demodulation service terminated.\n";

    modem.stop_demodulate();
}

void main_window::send_msg() {
    std::string str = textInput->toPlainText().toStdString();
    if (str == "")
        return;

    if (!modem.audio_stream_operating()) {
        textInput->setText("");
        return;
    }

    std::vector<char> data(str.begin(), str.end());
    data.push_back(NULL);

    packet_header header;
    packet p(header, std::move(data));

    std::uniform_int_distribution<int> uni_int(0, UINT8_MAX);
    p.header()->id = uni_int(engine);
    p.header()->control = packet_control::text;

    modem.modulate(p.packet_data());

    cout << time << " <TX> : " << str.c_str() << "\n";
    textInput->setText("");
}

void main_window::send_file() {
    QStringList dirs = QFileDialog::getOpenFileNames(this, "Select Files", QDir::currentPath());

    for (auto& dir : dirs) {
        QFile fp(dir);

        if (!fp.open(QIODeviceBase::ReadOnly)) {
            cout << "<System> An error occured while opening file.\n";
            fp.close();
            break;
        }

        packet_header header;
        file_header f_header(QFileInfo(fp).fileName().toStdString(), fp.size());
        size_t file_size = fp.size();
        size_t max_file_size = max_data_size - file_header_size;

        if (file_size > max_file_size) {
            cout << "<System> Cannnot exceed " << max_file_size << " Bytes.\n";
            fp.close();
            break;
        }

        std::uniform_int_distribution<int> uni_int(0, UINT8_MAX);
        header.id = uni_int(engine);

        packet p;
        QByteArray&& data = fp.read(file_size);

        p.push(&header, sizeof(header));
        p.push(&f_header, sizeof(f_header));
        p.push(data.data(), file_size);
        p.header()->control = packet_control::file;
        p.header()->len = p.packet_data().size();

        fp.close();

        cout << time << " <FILE TX> : Sended " << QFileInfo(fp).fileName() << "\n";

        modem.modulate(p.packet_data());
    }
}

void main_window::debug() {
    packet* p;

    if (!modem.get_packet_queue().empty()) {
        p = modem.get_packet_queue().pop();
    }

    if (!p)
        return;

    auto& data = p->packet_data();

    for (int i = 0; i < data.size(); ++i)
        cout << "<Debug> : " << (unsigned char)data[i] << " " << std::bitset<8>(data[i]).to_string() <<  "\n";

    cout << "\n";
}

void main_window::receiving_packet(int received, int packet_length) {
    QString msg = QString::number(received) + " / " + QString::number(packet_length) + " Bytes received.";

    statusBar->showMessage(msg);
}

void main_window::receive_packet() {
    packet* p;

    if (!modem.get_packet_queue().empty()) {
        p = modem.get_packet_queue().pop();
        translate_packet(p);
    }

    statusBar->showMessage("Transmission completed.", 1000);
}

void main_window::config_changed() {
    cout << "<System> Configuration changed.\n";
    start_audio_stream();
    start_demodulation_service();
}

void main_window::translate_packet(packet* p) {
    if (!p)
        return;

    switch (p->header()->control) {

    case packet_control::text:
        cout << time << " <RX> : " << p->data() << "\n"; break;

    case packet_control::file: 
        file_header* head = (file_header*)p->data();
        
        if (head->data_length + file_header_size + header_size != p->size()) {
            cout << time << " <FILE RX> : The file is corrupted.\n";
            break;
        }

        QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/Audio Modem Received Files/";
        QFile fp(utils::new_dir(path, head->file_name));

        if (!fp.open(QFile::WriteOnly)) {
            cout << time << " <FILE RX> : Failed to write " << head->file_name << "\n";
            fp.close();
            break;
        }

        fp.write(p->data() + sizeof(file_header), head->data_length);
        fp.close();

        cout << time << " <FILE RX> : Successfully wrote " << head->file_name << "\n";
        
        break;
        
    }
    
    delete p;
}

void main_window::export_log() {
    QString dir = QFileDialog::getSaveFileName(this, "Select File", QDir::currentPath());
    QFile fp(dir);

    if (!fp.open(QIODeviceBase::WriteOnly | QIODeviceBase::Text)) {
        cout << "<System> An error occured while opening file.\n";
        fp.close();
        return;
    }

    fp.write(textLog->toPlainText().toLocal8Bit());
    cout << "<System> Successfully created file log.\n";

    fp.close();
}

void main_window::show_info() {
    windowInfo->show();
}

void main_window::show_config() {
    windowConfig->setup();
    windowConfig->show();
}