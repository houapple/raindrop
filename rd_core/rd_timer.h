#ifndef	_RD_TIMER_H
#define	_RD_TIMER_H

#include "rd_types.h"

class CTimer
{
public:
	CTimer();
	
	void Init();
	QWORD GetTime();
	float GetTimeMillisec(QWORD time);

	typedef QWORD (*funcGetTime)();

private:
	QWORD	m_lCpuFreq;
	QWORD	m_lCpuTicksPerSec;
	funcGetTime	m_pfunc;
};

#endif