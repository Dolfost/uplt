#ifndef _PORT_SPEC_HPP_
#define _PORT_SPEC_HPP_

#include <string>

namespace uplt { 

struct port_spec { 
	std::string name;
	std::string alias;
	uint32_t baud = 9600;
	uint8_t data_bits = 8;
	uint8_t stop_bits = 1;

};

}

#endif // !_PORT_SPEC_HPP_
