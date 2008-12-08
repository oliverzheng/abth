#ifndef INTERFACE_APP_HPP
#define INTERFACE_APP_HPP

/* Project Includes */
#include "interface.hpp"

/* Additional Library Includes */
#include <pcap.h>

namespace packetwrapper {

/* An interface API that subclasses can inherit from to use PCAP functionality. */
class InterfaceApp
{
public:
	InterfaceApp();
	virtual ~InterfaceApp() = 0;

	/* Use an opened interface.
	 * Returns true if interface is open and will be used, otherwise false.
	 */
	virtual bool setInterface(Interface & i);

	virtual bool isInterfaceSet();

protected:
	pcap_t * pcapHandle;

	/* IP subnet mask for which the capture filter is set.
	 * Should be retrieved from the interface being used.
	 */
	unsigned int netmask;

}; /* class InterfaceApp */

} /* namespace packetwrapper */

#endif /* INTERFACE_APP_HPP */
