#ifndef MSNP_PACKET_HPP
#define MSNP_PACKET_HPP

/* Project Includes */
#include <packetwrapper/tcppacket.hpp>
#include "msnpexceptions.hpp"

/* Additional Library Includes */
#include <boost/regex.hpp>

/* Standard Library Includes */
#include <string>

namespace msnpabth {

class MSNPPacket : public packetwrapper::TCPPacket
{
public:
	/* A transaction ID is used by command of type:
	 * - CHALLENGE_RESPONSE
	 * - CHALLENGE_RETURN
	 * - PING_RESPONSE
	 */
	typedef enum ECommandType {
		CHALLENGE,		/* NS -> Client */
		CHALLENGE_RESPONSE,	/* Client -> NS */
		CHALLENGE_RETURN,	/* NS -> Client */
		PING,			/* Client -> NS */
		PING_RESPONSE,		/* NS -> Client */
		UNSUPPORTED
	} ECommandType;

	MSNPPacket();
	MSNPPacket(TCPPacket & tcpPacket);

	/* command must not be UNSUPPORTED.
	 * transactionID may be skipped if the command does not use it.
	 * transactionID must be positive if used.
	 */
	void setCommand(ECommandType command, int transactionID = -1) throw(CommandNotSetException, TransactionIDNotSetException);

	ECommandType getCommand() const throw(CommandNotSetException);
	int getTransactionID() const throw(TransactionIDNotSetException);

	/* Parse raw TCP data into an MSNP packet */
	void parse();

private:
	ECommandType command;
	bool commandSet;

	int transactionID;

	/* Each command can be constructed into <prefix> <transaction id> <postfix>.
	 * E.g. ping's prefix is "PNG " and postfix is "\r\n" to form "PNG <transaction ID>\r\n".
	 * transactionID indicates its requirement for the command.
	 */
	struct CommandStructure {
		ECommandType command;
		bool transactionID;
		const char * prefix;
		const char * postfix;
		boost::regex * commandRegex;
	};

	static CommandStructure commandStructures[];
	static std::string transactionIDRegex;

	CommandStructure * findCommandStructure(ECommandType command) const;
	static void setupRegexes();

}; /* class MSNPPacket */

} /* namespace msnpabth */

#endif /* MSNP_PACKET_HPP */
