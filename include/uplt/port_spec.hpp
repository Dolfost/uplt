#ifndef _PORT_SPEC_HPP_
#define _PORT_SPEC_HPP_

#include <QString>

namespace uplt { 

struct port_spec { 
	QString name;
	uint32_t baud = 9600;
	uint8_t data_bits = 8;
	uint8_t stop_bits = 1;
};

}

#endif // !_PORT_SPEC_HPP_
