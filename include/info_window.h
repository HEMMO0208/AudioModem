#pragma once
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QBoxLayout>

class info_window : public QWidget {
	Q_OBJECT

public:
	info_window(QWidget* parent) : QWidget(parent) {
		setFixedSize(230, 180);
		setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
		setWindowTitle("Information");
		
		QString str = "AudioModem V.1.0";

		logo = new QLabel(this);
		label_1 = new QLabel(this);
		label_2 = new QLabel(this);
		button = new QPushButton("OK", this);

		QPixmap logoPic;
		logoPic.load("resource\\logo.jpg");
		logoPic = logoPic.scaledToWidth(190);

		label_1->setText(str);
		label_1->setAlignment(Qt::AlignCenter);
		label_2->setText("Kang Hyeonmo, POSTECH\ngusah7787@postech.ac.kr");
		label_2->setAlignment(Qt::AlignCenter);
		button->setFixedSize(70, 30);
		logo->setPixmap(logoPic);
		logo->setAlignment(Qt::AlignCenter);

		
		layout = new QVBoxLayout(this);
		layout->addWidget(label_1);
		layout->addWidget(logo);
		layout->addWidget(label_2);
		layout->addWidget(button);
		layout->setAlignment(button, Qt::AlignHCenter);


		connect(button, &QPushButton::clicked, this, &info_window::close);
	}
	~info_window() {}

private:
	QVBoxLayout* layout;
	QLabel* label_1, *label_2;
	QLabel* logo;
	QPushButton* button;
};