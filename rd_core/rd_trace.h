#ifndef _RD_TRACE_H
#define _RD_TRACE_H

#include <stdio.h>
#include <stdarg.h>

#define DEBUG_TRACE		DebugTrace
#define DEBUG_VS_TRACE	OutputDebugString

inline void DebugTrace(const char* format, ...)
{
	static char buffer[1024];
	va_list ap;
	va_start(ap, format);
	vsprintf(buffer, format, ap);
	va_end(ap);
	buffer[1023] = 0;
	printf(buffer);
}

#endif