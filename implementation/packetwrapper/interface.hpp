#ifndef INTERFACE_HPP
#define INTERFACE_HPP

/* Project Includes */
#include <config/config.hpp>
#include "exceptions.hpp"
#include "interfaceaddress.hpp"

/* Additional Library Includes */
#include <pcap.h>

/* Standard Library Includes */
#include <string>
#include <list>

namespace packetwrapper {

/* Resolve circular dependency */
class InterfaceApp;

/* Abstraction of a network interface.
 * This is the base unit for which network traffic can be captured.
 * Contents of Interface should not be modified by the user.
 */
class Interface
{
public:
	Interface(const Interface & i);

	/* Open an interface for network capture or packet injection.
	 * promiscuous allows for capture of all packets received, regardless of its destination.
	 */
	void open(bool promiscuous) throw(OpenInterfaceException);

	bool isOpen();

	/* Generate a list of all available interfaces */
	static std::list<Interface> listInterfaces() throw(ListInterfacesException);

	/* Accompanying information about the interface */
	const std::string name;
	const std::string description;
	const InterfaceAddress addresses;

private:
	/* Prevent user from  creating interfaces */
	Interface(std::string name,
	          std::string description,
	          InterfaceAddress addresses);

	pcap_t * pcapHandle;

	/* Let PacketObserver access pcapHandle */
	friend class InterfaceApp;

}; /* class Interface */

} /* namespace packewrapper */

#endif /* INTERFACE_HPP */
