#pragma once
#include <QtCore/QObject>
#include <vector>

struct modem_signal_sender : public QObject {
	Q_OBJECT
		
public:
	std::string state;

signals:
	void packet_receiving(int received, int packet_length);
	void configuration_changed();
	void packet_lost();
	void debug_packet();
	void packet_received();
};