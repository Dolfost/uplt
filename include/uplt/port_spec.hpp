#ifndef _UPLT_PORT_SPEC_HPP_
#define _UPLT_PORT_SPEC_HPP_

#include <string>

#include <exprtk.hpp>

namespace uplt { 

struct port_spec { 
	std::string name;
	std::string alias;
	std::string fx = "x";
	uint32_t baud = 9600;
	uint8_t data_bits = 8;
	uint8_t stop_bits = 1;
	exprtk::expression<double> ex;
};

}

#endif // !_UPLT_PORT_SPEC_HPP_
