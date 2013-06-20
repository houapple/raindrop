#include "rd_app.h"
#include <time.h>

HWND g_hWnd = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

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

	GetRender()->InitDevice();
	srand(time(NULL));
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
			float elpsed_time = GetTimer()->GetTimeMillisec(curr_time - last_time) * 0.001f;

			m_fps.Run(elpsed_time);
			Run(elpsed_time);
			GetRender()->BeginScene();
			Draw(elpsed_time);
			GetRender()->EndScene();

			last_time = curr_time;
			DEBUG_TRACE("elpsed time:%f\tfps:%f\n", elpsed_time, m_fps.GetFps());
		}
	}

	GetRender()->ReleaseDevice();

	return false;
}
