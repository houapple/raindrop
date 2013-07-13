#ifndef _RD_CORE_H
#define _RD_CORE_H

#pragma warning(disable:4996)

#include "rd_types.h"
#include "rd_trace.h"
#include "rd_math.h"
#include "rd_string.h"
#include "rd_heapalloc.h"
#include "rd_timer.h"

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

#define MAX_NAMESZIE	32

struct stResDesc
{
	char filename[MAX_NAMESZIE];
	stResDesc(const char* name)
	{
		rd_strncpy(filename, name, sizeof(filename));
	}
};


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
inline void free_list(std::vector<T*>& vec)
{
	for (int i = 0; i < vec.size(); ++i)
	{
		rd_free(vec[i]);
	}
	vec.clear();
}

class CFuncTime
{
	char sz[MAX_NAMESZIE];
	QWORD start_time;
	int limit;
public:
	CFuncTime(const char* desc, int limit = 0)
	{
		rd_strncpy(sz, desc,sizeof(sz));
		start_time = GetTimer()->GetTime();
		this->limit = limit;
	}
	~CFuncTime()
	{
		QWORD end_time = GetTimer()->GetTime();
		float f = GetTimer()->GetTimeMillisec(end_time - start_time);
		if (f > limit)
			DEBUG_TRACE("%s time: %f\n", sz, f);
	}
};

#endif