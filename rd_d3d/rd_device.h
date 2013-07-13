#ifndef _RD_DEVICE_H
#define _RD_DEVICE_H

#include "../rd_render/interface_device.h"
#include "../rd_core/rd_core.h"
#include <d3d9.h>

struct VertexBase
{
	float x, y, z, w;
	DWORD color;
};
#define VertexBase_FVF	D3DFVF_XYZRHW | D3DFVF_DIFFUSE

struct VertexBitmap
{
	float x, y, z, w;
	float u, v;
	DWORD color;
};
#define VertexBitmap_FVF D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE



struct stDeviceInfo
{
	D3DDISPLAYMODE display_mode;
	D3DADAPTER_IDENTIFIER9 adapter_identifer;
	D3DCAPS9 caps;

	void PrintDisplayMode(D3DDISPLAYMODE& display_mode);
	void PrintCurDisplayMode() { PrintDisplayMode(display_mode); }
	void PrintAdapterIdentifier();
	void PrintCaps();
};

class CTextOut;
class CDevice : public IDevice
{
public:
	CDevice();
	~CDevice();
	
	void Init();
	void Release();
	void SetUp();
	
	void BeginScene(DWORD color);
	void EndScene();

	void DrawRect(const RectF& rect, DWORD color);
	void FillRect(const RectF& rect, DWORD color);
	void FillRectList(const RectF* rect, DWORD num, DWORD color);
	void DrawLine(const Vec2F& pt0, const Vec2F& pt1, DWORD color);
	void DrawLineStrip(const Vec2F* p, DWORD num, DWORD color);
	void DrawLineList(const Vec2F* p, DWORD num, DWORD color);

	void DrawString(const char* text, const Vec2F& pt, BYTE format, DWORD color);

private:
	IDirect3D9*				m_pD3D;
	IDirect3DDevice9*		m_pD3DDevice;
	IDirect3DVertexBuffer9* m_pVB;
	IDirect3DIndexBuffer9*	m_pIB;

	DWORD m_dwVBOffset;
	DWORD m_dwIBOffset;

	HWND	m_hWnd;
	RectF	m_CanvasRect;
	stDeviceInfo m_DeviceInfo;

	CTextOut*	m_pTextOut;
};

#endif