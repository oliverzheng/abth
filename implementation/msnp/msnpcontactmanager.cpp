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
	
	switch( msnpPacket.getCommand() ) {
		case MSNPPacket::ILN:
		{
			// initial sign-in
			int trId = msnpPacket.getTransactionID();
			ilnRx = true;

			if (initialTrId == 0) initialTrId = trId;

			break;
		}
/*
		case MSNPPacket::NLN:
		case MSNPPacket::UBX:
			// contact list modify
			break;
		case MSNPPacket::OUT:
			// contact sign out
			break;
*/
		default:
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
		}

	}
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
