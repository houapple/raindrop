#include <Windows.h>
#include "rd_core.h"

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

int main(int argc, char* argv[])
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
		DEBUG_VS_TRACE("RegisterClass failed!\n");
		return 0;
	}

	g_hWnd = CreateWindow(wc.lpszClassName, "Raindrop Window", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, wc.hInstance, NULL);
	if (! g_hWnd)
	{
		DEBUG_VS_TRACE("CreateWindow failed!\n");
		return 0;
	}

	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);

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
			Sleep(0);
	}

	return 0;
}
