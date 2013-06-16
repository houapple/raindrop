#ifndef _RD_STRING_H
#define _RD_STRING_H

#include <string.h>

inline const char* avar(const char* format, ...)
{
	static char buffer[1024];
	va_list ap;
	va_start(ap, format);
	vsprintf(buffer, format, ap);
	va_end(ap);
	buffer[1023] = 0;
	return buffer;
};

// string copy
inline void rd_strncpy(char* pDest, const char* pSrc, int destSize)
{
	if (! pDest || ! pSrc)
		return;
	int len = strlen(pSrc) + 1;
	if (len > destSize)
		len = destSize;
	memcpy(pDest, pSrc, len);
	*(pDest + len - 1) = 0;
}

#endif