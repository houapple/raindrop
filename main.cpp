#include <Windows.h>
#include <time.h>
#include "rd_core.h"
#include "rd_app.h"

HWND g_hWnd = NULL;
const int width = 800;
const int height = 600;

void rd_init()
{
	IRender* render = GetRender();
	if (! render)
		return;
	render->InitDevice();

	srand(time(NULL));
}

void rd_release()
{
	IRender* render = GetRender();
	if (! render)
		return;
	render->ReleaseDevice();
}

void RandomTriange()
{
	IRender* render = GetRender();
	if (! render)
		return;

	const int max_size = 9000;
	Vertex* v = (Vertex* )rd_malloc(max_size * sizeof(Vertex));
	for (int i = 0; i < max_size; ++i)
	{
		v[i].x = rand() % width;
		v[i].y = rand() % height;
		v[i].z = 0.0f;
		v[i].w = 1.0f;
		v[i].color = D3DCOLOR_ARGB(255, 0, 255, 0);
	}

	WORD* index = (WORD*)rd_malloc(sizeof(WORD) * max_size / 3);
	for (int i = 0; i < max_size / 3; ++i)
	{
		index[i] = i;
	}

	render->DrawTriangeList(max_size / 3, v, max_size, index, max_size / 3);

	rd_free(v);
	rd_free(index);
}

void rd_render()
{
	IRender* render = GetRender();
	if (! render)
		return;
	
	QWORD start_time = GetTimer()->GetTime();
	
	RandomTriange();
	render->Render();

	QWORD end_time = GetTimer()->GetTime();
	float ms = GetTimer()->GetTimeMillisec(end_time - start_time);
	DEBUG_TRACE("frame time in ms:%f\tfps:%d\n", ms, (int)(1000 / ms));
}


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
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, wc.hInstance, NULL);
	if (! g_hWnd)
	{
		DEBUG_VS_TRACE("CreateWindow failed!\n");
		return 0;
	}

	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);

	rd_init();

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
			rd_render();
	}

	rd_release();

	return 0;
}
