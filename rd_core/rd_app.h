#ifndef _RD_APP_H
#define _RD_APP_H
	
#include "../rd_render/rd_render.h"
#include "../rd_core/rd_core.h"
#include "rd_cursor.h"
#include "rd_timer.h"
#include "rd_fps.h"
#include <windows.h>

inline HWND GetWnd()
{
	extern HWND g_hWnd;
	return g_hWnd;
}

struct stSysInfo
{
	RectI	m_windowRect;
	RectI	m_deviceRect;
	Vec2I	m_ClientOffset;

	void SysInfo();
};

class CApp
{
public:
	CApp() {}
	virtual ~CApp() {}

	void Create(int width, int height, const char* window_name);
	bool Loop();

	virtual bool MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Run(float fElapsedTime);
	virtual void Draw(float fElapsedTime) {}
protected:
	stSysInfo	m_stSys;
	CFps	m_fps;
};

#endif