#ifndef DEBUG_HPP
#define DEBUG_HPP

/* Silence Compiler Warning */
#ifdef _DEBUG_NO_VARIADIC
	#pragma GCC system_header
#endif

#ifdef _DEBUG

/* Standard Library Includes */
#include <iostream>
#include <cstdio>

#define debug(...) \
	do { \
		std::cout << __FUNCTION__ << ": "; \
		printf(__VA_ARGS__); \
		std::cout << std::endl; \
	} while (0);

#else /* _DEBUG */

	#define debug(...)

#endif /* _DEBUG */

#endif /* DEBUG_HPP */
