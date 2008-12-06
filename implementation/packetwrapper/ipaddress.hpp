#ifndef IP_ADDRESS_HPP
#define IP_ADDRESS_HPP

#include <string>
#include <ostream>
#include <boost/regex.hpp>

namespace packetwrapper {

class IPAddress
{
public:
	IPAddress();

	std::string get() const;
	bool set(std::string ipAddress);

private:
	static const boost::regex ipRegex;

	std::string ipAddress;

}; /* class IPAddress */

} /* namespace packetwrapper */

std::ostream & operator << (std::ostream & os, const packetwrapper::IPAddress & ipAddress);

#endif
