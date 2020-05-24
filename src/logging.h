#pragma once

#include <iostream>

#include "libretro.h"

// Logging function.
static void zlog(int lineno, const char* filename, const char* msg, ...) {

	char buffer[256] = { 0 };
	sprintf(buffer, "%s(%d): ", filename, lineno);
	std::cout << buffer;

	va_list args;
	va_start(args, msg);
	vsprintf(buffer, msg, args);
	std::cout << buffer << std::endl;
	va_end(args);
}

// Logging macro.
#define ZLOG(msg, ...) \
	zlog(__LINE__, __FILE__, msg, __VA_ARGS__)
