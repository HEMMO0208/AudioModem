#include "modem_device.h"
#include "fsk.h"
#include "qpsk.h"

modem_device* modem_device::new_device(modem_type type, int sample_rate, int baud_rate) {
	modem_device* ret;

	switch (type) {
	case modem_type::fsk:
		ret = new fsk(sample_rate, baud_rate); break;

	case modem_type::qpsk:
		ret = new qpsk(sample_rate, baud_rate); break;

	default:
		ret = NULL;
	}

	return ret;
}