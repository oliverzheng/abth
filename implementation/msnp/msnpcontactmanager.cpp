/* Project Includes */
#include "msnpcontactmanager.hpp"

/* Namespace Declarations */
using namespace msnp;
using namespace boost;
using namespace std;

MSNPContactManager::MSNPContactManager() 
{
	onlineContactList = new vector<string>();
}

void MSNPContactManager::ParsePacket( MSNPPacket msnpPacket )
{
	static int initialTrId = 0;

	switch( msnpPacket.getCommand() ) {
		case MSNPPacket::INIT_CONTACT_LIST:
			ilnRx = true;
			if (initialTrId == 0) initialTrId = msnpPacket.getTransactionID();
			break;
		case MSNPPacket::STATUS_CHANGE:
			{
				string email = msnpPacket.getEmail();
				int index = SearchOnlineContactList(email);
			
				if ( index == -1 )
					onlineContactList->push_back(email);
			}
			break;
		case MSNPPacket::CONTACT_SIGN_OUT:
			{
				int index = SearchOnlineContactList(msnpPacket.getEmail());

				if ( index != -1 )
					onlineContactList->erase(onlineContactList->begin()+index);
			}
			break;
		case MSNPPacket::SIGN_OUT:
			onlineContactList->clear();
			break;
	}

	if ( initialTrId != 0 ) {
		int currentTrId = 0;
		try {
			currentTrId = msnpPacket.getTransactionID();
		} catch ( TransactionIDNotSetException e ) {
		}

		if ( currentTrId == initialTrId || currentTrId == 0 ) {
			InitialSignIn(msnpPacket);
		} else {
			initialTrId = 0;
		}
	}
}



void MSNPContactManager::InitialSignIn( const MSNPPacket msnpPacket ) {
	const regex expression("ILN \\d+ [A-Z]{3} (\\S+) \\d+");

	string dataString((const char *) msnpPacket.data, msnpPacket.dataLength);

	sregex_iterator m1(dataString.begin(), dataString.end(), expression);
	sregex_iterator m2;

	while( m1 != m2 ) {
		string contact = (*m1)[1].str();
		if ( SearchOnlineContactList( contact ) == -1 )
			onlineContactList->push_back(contact);

		m1++;
	}
}

int MSNPContactManager::SearchOnlineContactList( const std::string email ) {
	for( unsigned int i=0; i < onlineContactList->size(); i++)
	{
		if ( onlineContactList->at(i).compare(email) == 0 ) {
			return i;
		}
	}
	return -1;
}

int MSNPContactManager::GetNumOnline() 
{
	return onlineContactList->size();
}
