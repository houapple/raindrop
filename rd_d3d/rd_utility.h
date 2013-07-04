#ifndef _RD_UTILITY_H
#define _RD_UTILITY_H

#include <dxerr.h>

#define DEBUG_DXTRACE(hr)	DebugDXTrace(hr, __FILE__, __LINE__)

inline void DebugDXTrace(HRESULT hr, const char* file, int line)
{
	const char* error = DXGetErrorDescription(hr);
	DEBUG_TRACE("Dx Error: %s:%d %s\n", file, line, error);
}

#endif