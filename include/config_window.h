#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QSlider>
#include <QSpinBox>
#include <QGridLayout>
#include <vector>
#include "audio_modem.h"
#include "Windows.h"

class config_window : public QWidget {
	Q_OBJECT

public:
	config_window(QWidget* parent, audio_modem& modem) : QWidget(parent), modem(modem) {
		setFixedSize(330, 350);
		setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
		setWindowTitle("Configuration");

		labelInput = new QLabel("Input Device", this);
		labelOutput = new QLabel("Output Device", this);
		labelSampleRate = new QLabel("Sample Rate", this);
		labelChunkSize = new QLabel("Audio Buffer Size", this);
		labelDevice = new QLabel("Modem Device", this);
		labelBaudRate = new QLabel("Baud Rate", this);
		comboInput = new QComboBox(this);
		comboOutput = new QComboBox(this);
		comboSampleRate = new QComboBox(this);
		comboChunkSize = new QComboBox(this);
		comboDevice = new QComboBox(this);
		spinBaudRate = new QSpinBox(this);
		sliderInput = new QSlider(Qt::Horizontal, this);
		sliderOutput = new QSlider(Qt::Horizontal, this);
		buttonOk = new QPushButton("Confirm", this);
		buttonCancel = new QPushButton("Cancel", this);
		
		layoutWidget = new QWidget(this);
		layout = new QGridLayout(layoutWidget);

		layout->addWidget(labelInput, 0, 0);
		layout->addWidget(labelOutput, 2, 0);
		layout->addWidget(labelSampleRate, 4, 0);
		layout->addWidget(labelChunkSize, 5, 0);
		layout->addWidget(labelDevice, 6, 0);
		layout->addWidget(labelBaudRate, 7, 0);
		layout->addWidget(comboInput, 0, 1);
		layout->addWidget(sliderInput, 1, 1);
		layout->addWidget(comboOutput, 2, 1);
		layout->addWidget(sliderOutput, 3, 1);
		layout->addWidget(comboSampleRate, 4, 1);
		layout->addWidget(comboChunkSize, 5, 1);
		layout->addWidget(comboDevice, 6, 1);
		layout->addWidget(spinBaudRate, 7, 1);

		labelInput->setAlignment(Qt::AlignCenter);
		labelOutput->setAlignment(Qt::AlignCenter);
		labelSampleRate->setAlignment(Qt::AlignCenter);
		labelChunkSize->setAlignment(Qt::AlignCenter);
		labelDevice->setAlignment(Qt::AlignCenter);
		labelBaudRate->setAlignment(Qt::AlignCenter);

		layoutWidget->setGeometry(10, 0, 310, 280);
		buttonOk->setGeometry(70, 285, 80, 40);
		buttonCancel->setGeometry(180, 285, 80, 40);

		spinBaudRate->setRange(400, 3000);
		sliderInput->setRange(0, 1000);
		sliderOutput->setRange(0, 1000);

		connect(buttonCancel, &QPushButton::clicked, this, &QWidget::close);
		connect(buttonOk, &QPushButton::clicked, this, &config_window::change_config);
	}

	void setup() {
		comboInput->clear();
		comboOutput->clear();
		comboSampleRate->clear();
		comboChunkSize->clear();
		comboDevice->clear();
		spinBaudRate->clear();

		modem_config config = modem.config();
		std::vector<const PaDeviceInfo*> devices;
		modem.devices(devices);

		for (int i = 0; i < devices.size(); ++i) {
			QString name = QString::fromUtf8(devices[i]->name);
			comboInput->addItem(name);
			comboOutput->addItem(name);
		}

		for (int i = 0; i < modem_type_count; ++i) {
			comboDevice->addItem(modem_types[i]);
		}

		comboChunkSize->addItems({ "1024", "2048", "4096", "8192" });
		comboSampleRate->addItems({ "22050", "32000", "44100", "48000" });
		spinBaudRate->setValue(config.baud_rate);

		if (config.input_volume < 0) {
			sliderInput->setEnabled(false);
			sliderOutput->setEnabled(false);
		}

		sliderInput->setEnabled(true);
		sliderOutput->setEnabled(true);

		sliderInput->setValue(config.input_volume * 1000);
		sliderOutput->setValue(config.output_volume * 1000);

		int idxChunkSize = comboChunkSize->findText(QString::number(config.chunk_size));
		int idxSampleRate = comboSampleRate->findText(QString::number(config.sample_rate));

		if (config.input_device >= 0)
			comboInput->setCurrentIndex(config.input_device);

		if (config.output_device >= 0)
			comboOutput->setCurrentIndex(config.output_device);

		if (idxChunkSize >= 0)
			comboChunkSize->setCurrentIndex(idxChunkSize);

		if (idxSampleRate >= 0)
			comboSampleRate->setCurrentIndex(idxSampleRate);

		comboDevice->setCurrentIndex((int)config.device_type);
	}

	~config_window() {}

private:
	QLabel* labelInput, * labelOutput, * labelSampleRate, * labelChunkSize, * labelDevice, * labelBaudRate;
	QComboBox* comboInput, * comboOutput, * comboSampleRate, * comboChunkSize, * comboDevice;
	QSlider* sliderInput, * sliderOutput;
	QSpinBox *spinBaudRate;
	QPushButton* buttonOk, * buttonCancel;
	QGridLayout* layout;
	QWidget* layoutWidget;
	audio_modem& modem;

public slots:
	void change_config() {
		modem_config config;
		config.input_device = comboInput->currentIndex();
		config.output_device = comboOutput->currentIndex();
		config.chunk_size = comboChunkSize->currentText().toInt();
		config.sample_rate = comboSampleRate->currentText().toInt();
		config.device_type = (modem_type)comboDevice->currentIndex();
		config.baud_rate = spinBaudRate->value();
		config.input_volume = (double)sliderInput->value() / 1000;
		config.output_volume = (double)sliderOutput->value() / 1000;

		modem.set_config(config);
		this->close();
	}
};