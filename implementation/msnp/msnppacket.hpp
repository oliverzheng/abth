#ifndef MSNP_PACKET_HPP
#define MSNP_PACKET_HPP

/* Project Includes */
#include <packetwrapper/tcppacket.hpp>
#include "msnpexceptions.hpp"

/* Additional Library Includes */
#include <boost/regex.hpp>

/* Standard Library Includes */
#include <string>

namespace msnp {

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
		INVITE,			/* NS -> Client */
		UNSUPPORTED
	} ECommandType;

	MSNPPacket();
	MSNPPacket(TCPPacket & tcpPacket);

	/* command must not be UNSUPPORTED.
	 * transactionID may be skipped if the command does not use it.
	 * transactionID must be positive if used.
	 */
	void setCommand(ECommandType command, int transactionID = -1) throw(IllegalCommandException, TransactionIDNotSetException);

	/* Use a custom string in place of the transaction ID.
	 * The command itself must not use transaction ID.*/
	void setCustom(std::string custom) throw(CommandNotSetException, IllegalCommandException);

	ECommandType getCommand() const throw(CommandNotSetException);
	int getTransactionID() const throw(TransactionIDNotSetException);

private:
	/* Parse raw TCP data into an MSNP packet */
	void parse();

	ECommandType command;
	bool commandSet;

	int transactionID;

	static std::string transactionIDRegex;

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
		CommandStructure(ECommandType command, bool transactionID, const char * prefix, const char * postfix, bool useRegex);
		~CommandStructure();
	};

	static CommandStructure commandStructures[];

	CommandStructure * findCommandStructure(ECommandType command) const;

	/* Custom string to insert in lieu of transaction ID */
	std::string custom;

}; /* class MSNPPacket */

} /* namespace msnpabth */

#endif /* MSNP_PACKET_HPP */
