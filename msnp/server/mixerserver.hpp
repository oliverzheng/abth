#ifndef MIXER_SERVER_HPP
#define MIXER_SERVER_HPP

/* Project Includes */
#include "mixerconnection.hpp"

/* Standard Library Includes */
#include <string>

/* Additional Library Includes */
#include <boost/asio.hpp>

namespace mixer {

class MixerServer
{
public:
	MixerServer(boost::asio::io_service & io, std::string email, std::string name);

private:
	void startAccept();

	void handleAccept(MixerConnection::pointer newConnection, const boost::system::error_code& error);

	boost::asio::ip::tcp::acceptor acceptor;
	std::string email, name;

}; /* class MixerServer */

} /* namespace mixer */

#endif /* MIXER_SERVER_HPP */
