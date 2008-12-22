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
	static bool ilnRx = false;

	switch( msnpPacket.getCommand() ) {
		case MSNPPacket::INIT_CONTACT_LIST:
			ilnRx = true;
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

	if ( ilnRx ) ParseForILN(msnpPacket);
}

void MSNPContactManager::ParseForILN( const MSNPPacket msnpPacket ) {
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

int MSNPContactManager::SearchOnlineContactList( const string email ) {
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
