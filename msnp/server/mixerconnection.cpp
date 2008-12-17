/* Project Includes */
#include "mixerconnection.hpp"

/* Standard Library Includes */
#include <string>
#include <iostream>

/* Additional Library Includes */
#include <boost/bind.hpp>

using namespace mixer;
using namespace boost::asio::ip;
using namespace boost::asio;
using namespace boost;
using namespace std;

MixerConnection::pointer MixerConnection::create(io_service & io)
{
	return pointer(new MixerConnection(io));
}

tcp::socket & MixerConnection::socket()
{
	return sock;
}

void MixerConnection::start(string email, string name)
{
	this->email = email;
	this->name = name;

	sock.async_read_some(buffer(data, MAX_LENGTH), bind(&MixerConnection::answer, shared_from_this(), placeholders::error, placeholders::bytes_transferred));
}

MixerConnection::MixerConnection(io_service & io)
	: sock(io)
{
}

void MixerConnection::answer(const boost::system::error_code & error, size_t transferred)
{
	if (!error) {
		string ans(data, transferred);

		cmatch match;
		int transactionID;

		if (regex_match(ans.c_str(), match, regex("ANS (\\d+) .*\r\n")))
			transactionID = atoi(string(match[1].first, match[1].second).c_str());
		else {
			cerr << "fail" << endl;
		}

		stringstream iro;
		iro << "IRO " << transactionID << " 1 1 " << email << " " << name << " 512\r\n";
		write(sock, buffer(iro.str()));

		stringstream ansOk;
		ansOk << "ANS " << transactionID << " OK\r\n";
		write(sock, buffer(ansOk.str()));

		sendMessage("Hi");
	} else {
		delete this;
	}
}

void MixerConnection::sendMessage(string message)
{
	stringstream header;
	header << "MIME-Version: 1.0\r\n"
		<< "Content-Type: text/plain; charset=UTF-8\r\n"
		<< "X-MMS-IM-Format: FN=Arial; EF=; CO=0\r\n"
		<< "\r\n";

	stringstream msg;
	msg << "MSG " << email << " " << name << " " << header.str().length() + message.length() << "\r\n";
	msg << header.str();
	msg << message;

	async_write(sock, buffer(msg.str()), bind(&MixerConnection::handleWrite, shared_from_this(), placeholders::error, placeholders::bytes_transferred));

	sock.async_read_some(buffer(data, MAX_LENGTH), bind(&MixerConnection::handleRead, shared_from_this(), placeholders::error, placeholders::bytes_transferred));
}

void MixerConnection::handleRead(const boost::system::error_code & error, size_t transferred)
{
	string read(data, transferred);
	cout << read << endl;
}

void MixerConnection::handleWrite(const boost::system::error_code & error, size_t transferred)
{
	if (error)
		delete this;
}
