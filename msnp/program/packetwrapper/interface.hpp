#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <string>
#include <list>

#include "exceptions.hpp"
#include "address.hpp"

#include "pcap.h"

namespace packetwrapper {

class Interface
{
public:
	static std::list<Interface> listInterfaces() throw(ListInterfacesException);

	Interface(const Interface & i);

	void open(bool promiscuous) throw(OpenInterfaceException);
	bool isOpen();

	const std::string name;
	const std::string description;
	const Address addresses;

private:
	Interface(std::string name,
	          std::string description,
	          Address addresses);

	pcap_t * pcapHandle;

	friend class PacketObserver;

}; /* class Interface */

} /* namespace packewrapper */

#endif /* INTERFACE_HPP */
