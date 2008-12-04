#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include <cstdio>

#ifdef _DEBUG_NO_VARIADIC
	#pragma GCC system_header
#endif

#define debug(...) \
	std::cout << __FUNCTION__ << ": "; \
	printf(__VA_ARGS__); \
	std::cout << endl;

#endif
