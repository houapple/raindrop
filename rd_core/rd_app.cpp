#include "rd_app.h"
#include <time.h>

HWND g_hWnd = NULL;

HWND GetWnd()
{
	return g_hWnd;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CApp* app = (CApp*)GetWindowLong(g_hWnd, GWL_USERDATA);
	if (app && g_hWnd == hWnd)
		app->MsgProc(hWnd, uMsg, wParam, lParam);
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void stSysInfo::SysInfo()
{
	int cx_screen = GetSystemMetrics(SM_CXSCREEN);
	int cy_screen = GetSystemMetrics(SM_CYSCREEN);
	int cx_full_screen = GetSystemMetrics(SM_CXFULLSCREEN);
	int cy_full_screen = GetSystemMetrics(SM_CYFULLSCREEN);

	RECT rect;
	GetWindowRect(g_hWnd, &rect);
	m_windowRect.SetRect(rect.left, rect.top, rect.right, rect.bottom);
	MoveWindow(g_hWnd, (cx_screen - m_windowRect.Width()) / 2,  (cy_screen - m_windowRect.Height()) / 2, m_windowRect.Width(), m_windowRect.Height(), false);
//	MoveWindow(g_hWnd, 0,  0, m_windowRect.Width(), m_windowRect.Height(), false);
	GetWindowRect(g_hWnd, &rect);

	POINT pt;
	pt.x = 0;
	pt.y = 0;
	ClientToScreen(g_hWnd, &pt);
	pt.x -= rect.left;
	pt.y -= rect.top;
	m_ClientOffset.Set(pt.x, pt.y);
}

//////////////////////////////////////////////////////////////////////////
void CApp::Create(int width, int height, const char* window_name)
{
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = "Raindrop class";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (! RegisterClass(&wc))
	{
		DEBUG_TRACE("RegisterClass failed!\n");
		return;
	}

	g_hWnd = CreateWindow(wc.lpszClassName, window_name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, wc.hInstance, NULL);
	if (! g_hWnd)
	{
		DEBUG_TRACE("CreateWindow failed!\n");
		return;
	}

	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);
	SetWindowLong(g_hWnd, GWL_USERDATA, (LONG)this);

	m_stSys.SysInfo();
	GetDevice()->Init();
	srand((unsigned int)time(NULL));
}

bool CApp::Loop()
{
	QWORD last_time = GetTimer()->GetTime();
	while (true)
	{
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			QWORD curr_time = GetTimer()->GetTime();
			float elapsed_time = GetTimer()->GetTimeMillisec(curr_time - last_time) * 0.001f;

			m_fps.Run(elapsed_time);
			Run(elapsed_time);
			GetDevice()->BeginScene(0xffff0000);
			Draw(elapsed_time);
#ifdef _DEBUG
			DrawDebug(elapsed_time);
#endif
			GetDevice()->EndScene();

			last_time = curr_time;
		}
	}

	GetDevice()->Release();

	return false;
}

void CApp::Run(float fElapsedTime)
{
	GetAppCursor()->Update();
}

bool CApp::MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return true;
}

void CApp::DrawDebug(float elapsed_time)
{
	GetDevice()->DrawString(avar("fps:%f  elapsed time:%f", m_fps.GetFps(), elapsed_time), Vec2F(0.0f,0.0f), eDT_Left | eDT_Top, 0xffffffff);
	Vec2I mouse_pt = GetAppCursor()->GetPosition();
	GetDevice()->DrawString(avar("mouse:%d %d", mouse_pt.x, mouse_pt.y),  Vec2F(0.0f,12.0f), eDT_Left | eDT_Top, 0xff00ff00);
}