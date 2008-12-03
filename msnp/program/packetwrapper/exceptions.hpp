#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>

namespace packetwrapper {

class ListInterfacesException : public std::exception
{
public:
	ListInterfacesException() {};
	ListInterfacesException(std::string err) : err(err) {};
	virtual const char * what() const throw() { return err.c_str(); };
	~ListInterfacesException() throw() {};

private:
	std::string err;

}; /* class ListInterfacesException */

} /* namespace packetwrapper */

#endif /* EXCEPTIONS_HPP */
