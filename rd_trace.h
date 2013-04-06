#pragma once
#include <stdio.h>

#define DEBUG_TRACE		DebugTrace
#define DEBUG_VS_TRACE	OutputDebugString

inline void DebugTrace(const char* str)
{
	printf(str);
}