#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>

namespace packetwrapper {

class PacketWrapperException : public std::exception
{
public:
	virtual const char * what() const throw() { return err.c_str(); };

protected:
	PacketWrapperException() {};
	PacketWrapperException(std::string err) : err(err) {};

	virtual ~PacketWrapperException() throw() {};

	std::string err;

}; /* class PacketWrapperException */

class ListInterfacesException : public PacketWrapperException
{
public:
	ListInterfacesException(std::string err) : PacketWrapperException(err) {};
	virtual ~ListInterfacesException() throw() {};

}; /* class ListInterfacesException */

class OpenInterfaceException : public PacketWrapperException
{
public:
	OpenInterfaceException(std::string err) : PacketWrapperException(err) {};
	virtual ~OpenInterfaceException() throw() {};

}; /* class OpenInterfaceException */

} /* namespace packetwrapper */

#endif /* EXCEPTIONS_HPP */
