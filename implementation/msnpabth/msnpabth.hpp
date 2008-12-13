#ifndef MSNP_ABTH_HPP
#define MSNP_ABTH_HPP

/* Project Includes */
#include <config/config.hpp>
#include "msnppacket.hpp"
#include <packetwrapper/packetobservable.hpp>
#include <packetwrapper/packetinjector.hpp>

namespace msnpabth {

class MSNPABTH : public packetwrapper::PacketObservable
{
public:
	MSNPABTH(packetwrapper::PacketInjector * packetInjector);

protected:
	virtual void packetReceived(packetwrapper::TCPPacket & tcpPacket);

	static const char * INVITE_MESSAGE;

	static const unsigned int PING_NUM = 24;
	static const unsigned int CHALLENGE_NUM = 2;

	static const unsigned int PING_LENGTH = 5;
	static const unsigned int PING_RESPONSE_LENGTH = 8;
	static const unsigned int CHALLENGE_LENGTH = 28;
	static const unsigned int CHALLENGE_RESPONSE_LENGTH = 60;
	static const unsigned int CHALLENGE_RETURN_LENGTH = 8;
	static const unsigned int INVITE_LENGTH = 120;

	/* Create packets beforehand */
	void createPackets(MSNPPacket & pingResponse);

	/* Returns true if the last ping has been sent */
	bool sendPing();

	/* Returns true if the last challenge return has been sent */
	bool sendChallenge(MSNPPacket & challengeResponse);

	/* Sends final invite */
	void sendInvite();

	MSNPPacket pings[PING_NUM];
	packetwrapper::TCPPacket pingResponseAcks[PING_NUM];
	MSNPPacket challenges[CHALLENGE_NUM];
	MSNPPacket challengeReturns[CHALLENGE_NUM];
	MSNPPacket invite;

	bool started;
	packetwrapper::PacketInjector * packetInjector;

	unsigned int nthPing;
	unsigned int nthChallenge;

}; /* class MSNPABTH */

} /* namespace msnpabth */

#endif /* MSNP_ABTH_HPP */
