#ifndef LOGGER_HPP
#define LOGGER_HPP

/* Project Includes */
#include <config/config.hpp>
#include <msnp/msnppacket.hpp>
#include <msnp/msnpcontactmanager.hpp>
#include <packetwrapper/packetobservable.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

/* An implementation of PacketObservable.
 * Logs an averaged frequency of packets received for a given client.
 */
namespace logger {

class Logger : virtual public packetwrapper::PacketObservable
{
public:
	Logger();

protected:
	virtual void packetReceived(packetwrapper::TCPPacket & tcpPacket);

private:
	int numContacts;
	msnp::MSNPContactManager * contactManager;

	void Log (std::string msg) ;

	class Timer 
	{
	public:
		static const unsigned int ABTH_DURATION_SEC = 3;

		Timer(boost::posix_time::time_duration duration);
		void resetTimer();
		bool isTimerExpired();
	private:
		boost::posix_time::ptime endTime;
		boost::posix_time::time_duration logDuration;
	}; /* class Timer */

	Timer * timer;
}; /* class Logger */

} /* namespace logger */

#endif /* LOGGER_HPP */
