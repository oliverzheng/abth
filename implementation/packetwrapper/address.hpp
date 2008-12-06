#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#include <string>

namespace packetwrapper {

class Address
{
public:
	Address() {};
	Address(std::string addr,
	        std::string netmask,
	        std::string broadaddr,
	        std::string dstaddr);

	std::string addr;
	std::string netmask;
	std::string broadaddr;
	std::string dstaddr;

}; /* class Address */

} /* namespace packewrapper */

#endif /* ADDRESS_HPP */
