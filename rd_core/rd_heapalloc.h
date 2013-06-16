#pragma once
#include <stdlib.h>

#define rd_malloc(s) _rd_malloc(s, __FILE__, __LINE__)
#define rd_free(p) _rd_free(p)

inline void* _rd_malloc(size_t size, const char* file, int line)
{
	return malloc(size);
}

inline void _rd_free(void* p)
{
	free(p);
}