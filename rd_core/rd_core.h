#ifndef _RD_CORE_H
#define _RD_CORE_H

#pragma warning(disable:4996)

#include "rd_types.h"
#include "rd_trace.h"
#include "rd_math.h"
#include "rd_string.h"
#include "rd_heapalloc.h"

#include <vector>
#include <string>
#include <queue>
#include <stack>


#ifndef NAMESPACE_BEGINE
#define NAMESPACE_BEGINE(X) namespace X {
#define NAMESPACE_END };
#endif


#define SAFE_DELETE(p)		{ if ((p)) delete (p); (p) = NULL; }
#define SAFE_DELETEARRY(p)	{ if ((p)) delete [] (p); (p) = NULL; }
#define SAFE_FREE(p)		{ if ((p)) rd_free((p)); (p) = NULL; }
#define SAFE_RELEASE(p)		{ if ((p)) (p)->Release(); (p) = NULL; }

#define count_of(x)		(sizeof((x)[0]) != 0 ? sizeof((x)) / sizeof((x)[0]) : 0)

template<class T>
inline void rd_swap(T& a, T& b)
{
	T t;
	t = a;
	a = b;
	b = t;
}

template<class T>
inline bool contain_of(const std::vector<T>& vec, T t)
{
	for (int i = 0; i < vec.size(); ++i)
	{
		if (t == vec[i])
			return true;
	}
	return false;
}

template<class T>
inline T* contain_ptr_of(const std::vector<T*>& vec, T* p)
{
	for (int i = 0; i < vec.size(); ++i) 
	{
		if (p == vec[i])
			return p;
	}
	return NULL;
}

template<class T>
inline void free_list(std::vector<T*>& vec)
{
	for (int i = 0; i < vec.size(); ++i)
	{
		rd_free(vec[i]);
	}
	vec.clear();
}

#endif