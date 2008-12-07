#ifndef IP_ADDRESS_HPP
#define IP_ADDRESS_HPP

#include <config/config.hpp>

#include <string>
#include <ostream>
#include <boost/regex.hpp>

namespace packetwrapper {

class IPAddress
{
public:
	IPAddress();

	std::string get() const;
	const char * getCStr() const;
	unsigned int getRaw() const;

	bool set(std::string ipAddress);
	bool set(unsigned int ipAddress);

private:
	static const boost::regex ipRegex;
	static const boost::regex digitRegex;

	std::string ipAddress;
	unsigned int ipAddressRaw;

}; /* class IPAddress */

} /* namespace packetwrapper */

std::ostream & operator << (std::ostream & os, const packetwrapper::IPAddress & ipAddress);

#endif
