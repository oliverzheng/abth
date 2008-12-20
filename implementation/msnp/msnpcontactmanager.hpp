#ifndef MSNP_CONTACTS_HPP
#define MSNP_CONTACTS_HPP

/* Project Includes */
#include <packetwrapper/tcppacket.hpp>
#include "msnppacket.hpp"
#include "msnpexceptions.hpp"

/* Additional Library Includes */
#include <boost/regex.hpp>

/* Standard Library Includes */
#include <string>

namespace msnp {

class MSNPContactManager
{
public:
	MSNPContactManager();
	
	void ParsePacket( MSNPPacket msnpPacket );

	int GetNumOnline();

private:
	void InitialSignIn( MSNPPacket msnpPacket );

	int numOnline;
}; /* class MSNPContacts */

} /* namespace msnp */

#endif /* MSNP_CONTACTS_HPP */
