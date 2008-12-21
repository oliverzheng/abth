/* Project Includes */
#include "msnpcontactmanager.hpp"

/* Standard Library Includes */
#include <cstring>
#include <sstream>

/* Namespace Declarations */
using namespace msnp;
using namespace boost;
using namespace std;

MSNPContactManager::MSNPContactManager() 
	: numOnline(0)
{
}

void MSNPContactManager::ParsePacket( MSNPPacket msnpPacket )
{
	static int initialTrId = 0;
	static bool ilnRx = false;
	static MSNPPacket::ECommandType prevPacket = MSNPPacket::UNSUPPORTED;
	static std::string statusChangeEmail;
	static std::string prevContactEmail;

	switch( msnpPacket.getCommand() ) {
		case MSNPPacket::INIT_CONTACT_LIST:
			ilnRx = true;
			if (initialTrId == 0) initialTrId = msnpPacket.getTransactionID();
			break;
		case MSNPPacket::STATUS_CHANGE:
			if ( msnpPacket.getStatusCode().compare("IDL") == 0 ) {
				statusChangeEmail.clear();
			} else {
				statusChangeEmail = msnpPacket.getEmail();
			}
			break;
		case MSNPPacket::PERSONAL_MSG:
			{
				std::string curEmail = msnpPacket.getEmail();

				if ( prevPacket == MSNPPacket::STATUS_CHANGE && 
					statusChangeEmail.compare(curEmail) == 0) {
						if ( prevContactEmail.compare(curEmail) != 0 ) {
							prevContactEmail = curEmail;
							numOnline++;
						}
				}
			}
			break;
		case MSNPPacket::CONTACT_SIGN_OUT:
			if ( prevContactEmail.compare(msnpPacket.getEmail()) == 0 )
				prevContactEmail.clear();

			numOnline--;
			break;
		case MSNPPacket::SIGN_OUT:
			numOnline = 0;
			break;
		default:
			statusChangeEmail.clear();
			break;
	}

	if ( ilnRx ) {
		int currentTrId = 0;
		try {
			currentTrId = msnpPacket.getTransactionID();
		} catch ( TransactionIDNotSetException e ) {
		}

		if ( currentTrId == initialTrId || currentTrId == 0 ) {
			InitialSignIn(msnpPacket);
		} else {
			initialTrId = 0;
			ilnRx = false;
		}
	}

	prevPacket = msnpPacket.getCommand();
}

void MSNPContactManager::InitialSignIn( MSNPPacket msnpPacket ) {
	const regex re("(ILN )+");

	std::string dataString((const char *) msnpPacket.data, msnpPacket.dataLength);

	sregex_token_iterator cur( dataString.begin(), dataString.end(), re );
	sregex_token_iterator end;

	for( ; cur != end; ++cur )
		numOnline++;
}

int MSNPContactManager::GetNumOnline() 
{
	return numOnline;
}
