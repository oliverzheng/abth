/* Project Includes */
#include "msnpabth.hpp"
#include "msnppacket.hpp"

/* Namespace Declarations */
using namespace msnpabth;
using namespace packetwrapper;

void MSNPABTH::packetReceived(TCPPacket * tcpPacket)
{
	MSNPPacket msnpPacket(*tcpPacket);

	switch (msnpPacket.getCommand()) {
		case MSNPPacket::UNSUPPORTED:
			//debug("unsupported");
			break;
		case MSNPPacket::PING:
			debug("ping");
			break;
		case MSNPPacket::PING_RESPONSE:
			debug("ping response %d", msnpPacket.getTransactionID());
			break;
		case MSNPPacket::CHALLENGE:
			debug("challenge");
			break;
		case MSNPPacket::CHALLENGE_RESPONSE:
			debug("challenge response %d", msnpPacket.getTransactionID());
			break;
		case MSNPPacket::CHALLENGE_RETURN:
			debug("challenge result %d", msnpPacket.getTransactionID());
			break;
		default:
			break;
	}

	delete tcpPacket;
}
