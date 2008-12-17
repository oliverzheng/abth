#ifndef MSNP_EXCEPTIONS_HPP
#define MSNP_EXCEPTIONS_HPP

/* Project Includes */
#include <config/config.hpp>

/* Standard Library Exceptions */
#include <exception>
#include <string>

namespace msnp {

class MSNPABTHException : public std::exception
{
public:
	virtual const char * what() const throw() { return err.c_str(); };

protected:
	MSNPABTHException() {};
	MSNPABTHException(std::string err) : err(err) {};

	virtual ~MSNPABTHException() throw() {};

	std::string err;

}; /* class MSNPABTHException */


class CommandNotSetException : public MSNPABTHException
{
public:
	CommandNotSetException() : MSNPABTHException() {};
	virtual ~CommandNotSetException() throw() {};

}; /* class CommandNotSetException */


class TransactionIDNotSetException : public MSNPABTHException
{
public:
	TransactionIDNotSetException() : MSNPABTHException() {};
	virtual ~TransactionIDNotSetException() throw() {};

}; /* class TransactionIDNotSetException */


class IllegalCommandException : public MSNPABTHException
{
public:
	IllegalCommandException() : MSNPABTHException() {};
	virtual ~IllegalCommandException() throw() {};

}; /* class TransactionIDNotSetException */

} /* namespace packetwrapper */

#endif /* MSNP_EXCEPTIONS_HPP */
