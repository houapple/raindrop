#include "rd_cursor.h"
#include <windows.h>

void CCursor::Update()
{
	POINT pt;
	GetCursorPos(&pt);
	extern HWND GetWnd();
	ScreenToClient(GetWnd(), &pt);
	m_Position.Set(pt.x, pt.y);
}