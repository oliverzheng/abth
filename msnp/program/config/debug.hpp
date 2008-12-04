#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include <cstdio>

#define debug(...) \
	std::cout << __FUNCTION__ << ": "; \
	printf(__VA_ARGS__); \
	std::cout << endl;

#endif
