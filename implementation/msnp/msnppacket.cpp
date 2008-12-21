/* Project Includes */
#include "msnppacket.hpp"

/* Standard Library Includes */
#include <string>
#include <cstring>
#include <sstream>

/* Namespace Declarations */
using namespace msnp;
using namespace boost;
using namespace std;

string MSNPPacket::transactionIDRegex("(\\d+)");

MSNPPacket::CommandStructure MSNPPacket::commandStructures[] =
{
	CommandStructure(CHALLENGE,			false,	"CHL ",	"0 29875966221179296547\r\n",	false),
	CommandStructure(CHALLENGE_RESPONSE,true,	"QRY ",	" \\S+ \\d+\r\n\\w+",	true),
	CommandStructure(CHALLENGE_RETURN,	true,	"QRY ",	"\r\n",					true),
	CommandStructure(PING,				false,	"PNG",	"\r\n",					true),
	CommandStructure(PING_RESPONSE,		true,	"QNG ",	"\r\n",					true),
	CommandStructure(INVITE,			false,	"RNG ",	"\r\n",					false),
	CommandStructure(INIT_CONTACT_LIST,	true,	"ILN ",	".*",					true),
	CommandStructure(STATUS_CHANGE,		false,	"NLN ",	"(\[A-Z]{3}) (\\S+) \\d+ \\S+ .*",	true),
	CommandStructure(PERSONAL_MSG,		false,	"UBX ",	"(\\S+) \\d+ \\d+\r\n.*",true),
	CommandStructure(CONTACT_SIGN_OUT,	false,	"FLN ",	"(\\S+) \\d+ \\d+ \r\n",	true),
	CommandStructure(SIGN_OUT,			false,	"OUT ",	"\r\n",					true),
	CommandStructure(UNSUPPORTED,		false,	NULL,	NULL,					false),
};

MSNPPacket::CommandStructure::CommandStructure(ECommandType command, bool transactionID, const char * prefix, const char * postfix, bool useRegex)
	: command(command), transactionID(transactionID), prefix(prefix), postfix(postfix), commandRegex(NULL)
{
	if (useRegex) {
		string commandRegexString = prefix;
		if (transactionID)
			commandRegexString += transactionIDRegex;
		commandRegexString += postfix;

		commandRegex = new regex(commandRegexString);
	}
}

MSNPPacket::CommandStructure::~CommandStructure()
{
	if (commandRegex != NULL)
		delete commandRegex;
}

MSNPPacket::MSNPPacket()
	: command(UNSUPPORTED), commandSet(false), transactionID(-1)
{
	email.clear();
	statusCode.clear();
}

MSNPPacket::MSNPPacket(TCPPacket & tcpPacket)
	: TCPPacket(tcpPacket), command(UNSUPPORTED), commandSet(false), transactionID(-1)
{
	email.clear();
	statusCode.clear();
	parse();
}

void MSNPPacket::setCommand(ECommandType command, int transactionID) throw(IllegalCommandException, TransactionIDNotSetException)
{
	if (command == UNSUPPORTED)
		throw IllegalCommandException();

	if (command != this->command)
		custom.erase();

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
	} else
		commandData += custom;

	commandData += commandStructure->postfix;

	dataLength = commandData.length();
	data = new unsigned char[dataLength];
	memcpy((unsigned char *) data, commandData.c_str(), dataLength);
}

void MSNPPacket::setCustom(string custom) throw(CommandNotSetException, IllegalCommandException)
{
	if (command == UNSUPPORTED)
		throw CommandNotSetException();

	if (findCommandStructure(command)->transactionID == true)
		throw IllegalCommandException();

	this->custom = custom;
	setCommand(command);
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

string MSNPPacket::getEmail() const throw(EmailNotSetException)
{
	if (email.empty())
		throw EmailNotSetException();

	return email;
}

string MSNPPacket::getStatusCode() const throw (StatusCodeNotSetException)
{
	if (statusCode.empty())
		throw StatusCodeNotSetException();

	return statusCode;
}

void MSNPPacket::parse()
{
	command = UNSUPPORTED;
	commandSet = true;

	transactionID = -1;

	CommandStructure * commandStructure = NULL;

	string dataString((const char *) data, dataLength);
	cmatch match;

	for (commandStructure = commandStructures; commandStructure->command != UNSUPPORTED; commandStructure++) {
		if (commandStructure->commandRegex == NULL)
			continue;

		if (regex_match(dataString.c_str(), match, *(commandStructure->commandRegex)))
			break;
	}

	if (commandStructure->command != UNSUPPORTED) {
			command = commandStructure->command;

			if (commandStructure->transactionID)
				transactionID = atoi(string(match[1].first, match[1].second).c_str());
			
			switch( commandStructure->command ) {
				case STATUS_CHANGE:
					statusCode.assign(string(match[1].first, match[1].second).c_str());
					email.assign(string(match[2].first, match[2].second).c_str());
					break;
				case PERSONAL_MSG:
				case CONTACT_SIGN_OUT:
					email.assign(string(match[1].first, match[1].second).c_str());
			}
	}
}

MSNPPacket::CommandStructure * MSNPPacket::findCommandStructure(ECommandType command) const
{
	CommandStructure * commandStructure = NULL;
	for (commandStructure = commandStructures; commandStructure->command != UNSUPPORTED; commandStructure++)
		if (command == commandStructure->command)
			break;

	return commandStructure;
}
