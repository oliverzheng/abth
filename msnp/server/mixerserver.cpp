/* Project Includes */
#include "mixerserver.hpp"

/* Standard Library Includes */
#include <iostream>

/* Additional Library Includes */
#include <boost/bind.hpp>

using namespace mixer;
using namespace boost::asio::ip;
using namespace boost::asio;
using namespace boost;
using namespace std;

MixerServer::MixerServer(io_service & io, string email, string name)
	: acceptor(io, tcp::endpoint(tcp::v4(), 1863)), email(email), name(name)
{
	cout << "Waiting to accept connection..." << endl;

	startAccept();
}

void MixerServer::startAccept()
{
	MixerConnection::pointer newConnection = MixerConnection::create(acceptor.io_service());

	acceptor.async_accept(newConnection->socket(), bind(&MixerServer::handleAccept, this, newConnection, placeholders::error));
}

void MixerServer::handleAccept(MixerConnection::pointer newConnection, const boost::system::error_code & error)
{
	if (!error) {
		cout << "Accepted connection..." << endl;

		newConnection->start(email, name);
		startAccept();
	}
}
