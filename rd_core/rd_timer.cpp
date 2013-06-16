#include "rd_timer.h"
#include <windows.h>


static QWORD GetMMTime()
{
	QWORD time = timeGetTime();
	return time;
}

static QWORD GetQueryPerformanceTime()
{
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	QWORD time = t.QuadPart;
	return time;
}

CTimer::CTimer()
: m_lCpuFreq(0)
, m_lCpuTicksPerSec(0)
, m_pfunc(NULL)
{
	Init();
}

void CTimer::Init()
{
	LARGE_INTEGER  freq;
	if (QueryPerformanceFrequency(&freq))
	{
		LARGE_INTEGER t;
		QueryPerformanceCounter(&t);
		m_lCpuTicksPerSec = freq.QuadPart;
		m_pfunc = GetQueryPerformanceTime;
	}
	else
	{
		m_pfunc = GetMMTime;
		m_lCpuTicksPerSec = 1000;
	}
}

QWORD CTimer::GetTime()
{
	return (*m_pfunc)();
}

float CTimer::GetTimeMillisec(QWORD time)
{
	return (float) (time / (m_lCpuTicksPerSec * 0.001f));
}