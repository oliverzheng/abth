/* Project Includes */
#include "msnppacket.hpp"

/* Standard Library Includes */
#include <string>
#include <cstring>
#include <sstream>

/* Namespace Declarations */
using namespace msnpabth;
using namespace boost;
using namespace std;

MSNPPacket::CommandStructure MSNPPacket::commandStructures[] =
{
	{CHALLENGE,		false,	"CHL ",	"0 12345678901234567890\r\n",	NULL},
	{CHALLENGE_RESPONSE,	true,	"QRY ",	" \\s+ \\d+\r\n\\s+",		NULL},
	{CHALLENGE_RETURN,	true,	"QRY ",	"\r\n",				NULL},
	{PING,			false,	"PNG",	"\r\n",				NULL},
	{PING_RESPONSE,		true,	"QNG ",	"\r\n",				NULL},
	{UNSUPPORTED,		false,	NULL,	NULL,				NULL},
};

string MSNPPacket::transactionIDRegex("(\\d+)");

MSNPPacket::MSNPPacket()
	: command(UNSUPPORTED), commandSet(false), transactionID(-1)
{
	setupRegexes();
}

MSNPPacket::MSNPPacket(TCPPacket & tcpPacket)
	: TCPPacket(tcpPacket), command(UNSUPPORTED), commandSet(false), transactionID(-1)
{
	setupRegexes();
}

void MSNPPacket::setCommand(ECommandType command, int transactionID) throw(CommandNotSetException, TransactionIDNotSetException)
{
	if (command == UNSUPPORTED)
		throw CommandNotSetException();

	CommandStructure * commandStructure = findCommandStructure(command);

	if (commandStructure->transactionID)
		if (transactionID < 0)
			throw TransactionIDNotSetException();
		else
			this->transactionID = transactionID;

	this->command = command;
	commandSet = true;

	/* Construct TCP data */
	if (data != NULL)
		delete data;

	string commandData = commandStructure->prefix;
	if (commandStructure->transactionID) {
		ostringstream transactionIDString;
		transactionIDString << transactionID;
		commandData += transactionIDString.str();
	}
	commandData += commandStructure->postfix;

	dataLength = commandData.length();
	data = new unsigned char[dataLength];
	memcpy((unsigned char *) data, commandData.c_str(), dataLength);
}

MSNPPacket::ECommandType MSNPPacket::getCommand() const throw(CommandNotSetException)
{
	if (!commandSet)
		throw CommandNotSetException();

	return command;
}

int MSNPPacket::getTransactionID() const throw(TransactionIDNotSetException)
{
	if (transactionID < 0)
		throw TransactionIDNotSetException();

	return transactionID;
}

void MSNPPacket::parse()
{
	commandSet = true;

	for (CommandStructure * commandStructure = commandStructures; commandStructure->command != UNSUPPORTED; commandStructure++) {
		string dataString((const char *) data, dataLength);
		cmatch match;

		if (regex_match(dataString.c_str(), match, *(commandStructure->commandRegex))) {
			command = commandStructure->command;

			if (commandStructure->transactionID)
				transactionID = atoi(string(match[1].first, match[1].second).c_str());
			return;
		}
	}

	command = UNSUPPORTED;

	transactionID = -1;
}

MSNPPacket::CommandStructure * MSNPPacket::findCommandStructure(ECommandType command) const
{
	CommandStructure * commandStructure = NULL;
	for (commandStructure = commandStructures; commandStructure->command != UNSUPPORTED; commandStructure++)
		if (command == commandStructure->command)
			break;

	return commandStructure;
}

void MSNPPacket::setupRegexes()
{
	static bool setup = false;
	if (!setup) {
		for (CommandStructure * commandStructure = commandStructures; commandStructure->command != UNSUPPORTED; commandStructure++) {
			string commandRegexString = commandStructure->prefix;
			if (commandStructure->transactionID)
				commandRegexString += transactionIDRegex;
			commandRegexString += commandStructure->postfix;

			commandStructure->commandRegex = new regex(commandRegexString);
		}

		setup = true;
	}
}
