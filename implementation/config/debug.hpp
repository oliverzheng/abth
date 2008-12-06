#ifndef DEBUG_HPP
#define DEBUG_HPP

#ifdef _DEBUG_NO_VARIADIC
	#pragma GCC system_header
#endif

#ifdef _DEBUG

#include <iostream>
#include <cstdio>

#define debug(...) \
	std::cout << __FUNCTION__ << ": "; \
	printf(__VA_ARGS__); \
	std::cout << std::endl;

#else /* _DEBUG */

	#define debug(...)

#endif /* _DEBUG */

#endif
