#ifndef MIXER_CONNECTION_HPP
#define MIXER_CONNECTION_HPP

/* Project Includes */
#include "config.hpp"

/* Additional Library Includes */
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/regex.hpp>

/* Standard Library Includes */
#include <string>

namespace mixer {

class MixerConnection : public boost::enable_shared_from_this<MixerConnection>
{
public:
	typedef boost::shared_ptr<MixerConnection> pointer;

	static pointer create(boost::asio::io_service & io);

	boost::asio::ip::tcp::socket & socket();

	void start(std::string email, std::string name);
	void sendMessage(std::string message);

private:
	MixerConnection(boost::asio::io_service & io);

	void answer(const boost::system::error_code&, size_t transferred);
	void handleWrite(const boost::system::error_code&, size_t transferred);
	void handleRead(const boost::system::error_code&, size_t transferred);

	static const int MAX_LENGTH = 1024;
	char data[MAX_LENGTH];
	boost::asio::ip::tcp::socket sock;
	std::string email, name;

}; /* class MixerConnection */

} /* namespace mixer */

#endif /* MIXER_CONNECTION_HPP */
