#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <string>
#include <list>

#include "exceptions.hpp"
#include "address.hpp"

namespace packetwrapper {

#include "pcap.h"

class Interface
{
public:
	static std::list<Interface> listInterfaces() throw(ListInterfacesException);

	Interface(const Interface & i);

	void open(bool promiscuous) throw(OpenInterfaceException);

	const std::string name;
	const std::string description;
	const std::list<Address> addresses;

private:
	Interface(std::string name,
	          std::string description,
	          std::list<Address> addresses);

	pcap_t * pcapHandle;

}; /* class Interface */

} /* namespace packewrapper */

#endif /* INTERFACE_HPP */
