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
	
	/* Parse the packet to determine whether it affects the contact list */
	void ParsePacket( MSNPPacket msnpPacket );

	/* Retrieves the number of contacts currently online */
	int GetNumOnline();

private:
	void ParseForILN( const MSNPPacket msnpPacket );

	/* if the email is found within the contact list, the index is returned, else -1 is returned */
	int SearchOnlineContactList( const std::string email );

	std::vector<std::string> *onlineContactList;
}; /* class MSNPContacts */

} /* namespace msnp */

#endif /* MSNP_CONTACTS_HPP */
