#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <string>
#include <list>

#include "address.hpp"

namespace packetwrapper {

class Interface
{
public:
	Interface() {};
	Interface(std::string name,
	          std::string description,
	          std::list<Address> addresses);

	std::string name;
	std::string description;
	std::list<Address> addresses;

}; /* class Interface */

} /* namespace packewrapper */

#endif /* INTERFACE_HPP */
