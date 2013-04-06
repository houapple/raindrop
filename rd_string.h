#pragma once
#include <string.h>

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