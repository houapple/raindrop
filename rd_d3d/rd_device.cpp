#include "rd_device.h"
#include "../rd_core/rd_app.h"

IRender* GetRender()
{
	static d3d::CDevice render;
	return &render;
}

NAMESPACE_BEGINE(d3d)

void stDeviceInfo::PrintDisplayMode(D3DDISPLAYMODE& display_mode)
{
	std::string fmt;
	switch (display_mode.Format)
	{
	case D3DFMT_R8G8B8:			fmt = "D3DFMT_R8G8B8";			break;
	case D3DFMT_A8R8G8B8:		fmt = "D3DFMT_A8R8G8B8";		break;
	case D3DFMT_X8R8G8B8:		fmt = "D3DFMT_X8R8G8B8";		break;
	case D3DFMT_R5G6B5:			fmt = "D3DFMT_R5G6B5";			break;
	case D3DFMT_X1R5G5B5:		fmt = "D3DFMT_X1R5G5B5";		break;
	case D3DFMT_A1R5G5B5:		fmt = "D3DFMT_A1R5G5B5";		break;
	case D3DFMT_A4R4G4B4:		fmt = "D3DFMT_A4R4G4B4";		break;
	case D3DFMT_R3G3B2:			fmt = "D3DFMT_R3G3B2";			break;
	case D3DFMT_A8:				fmt = "D3DFMT_A8";				break;
	case D3DFMT_A8R3G3B2:		fmt = "D3DFMT_A8R3G3B2";		break;
	case D3DFMT_X4R4G4B4:		fmt = "D3DFMT_X4R4G4B4";		break;
	case D3DFMT_A2B10G10R10:	fmt = "D3DFMT_A2B10G10R10";		break;
	case D3DFMT_A8B8G8R8:		fmt = "D3DFMT_A8B8G8R8";		break;
	case D3DFMT_X8B8G8R8:		fmt = "D3DFMT_X8B8G8R8";		break;
	case D3DFMT_G16R16:			fmt = "D3DFMT_G16R16";			break;
	case D3DFMT_A2R10G10B10:	fmt = "D3DFMT_A2R10G10B10";		break;
	case D3DFMT_A16B16G16R16:	fmt = "D3DFMT_A16B16G16R16";	break;
	}
	DEBUG_TRACE("Width:%d\tHeight:%d\tRefreshRate:%d\tFormat:%s\n", display_mode.Width, display_mode.Height, display_mode.RefreshRate, fmt.c_str());
}

void stDeviceInfo::PrintAdapterIdentifier()
{
	DEBUG_TRACE("Adapter info:\nDirver:%s\nDescription:%s", adapter_identifer.Driver, adapter_identifer.Description);
	DWORD Product = HIWORD(adapter_identifer.DriverVersion.HighPart);
	DWORD Version = LOWORD(adapter_identifer.DriverVersion.HighPart);
	DWORD SubVersion = HIWORD(adapter_identifer.DriverVersion.LowPart);
	DWORD Build = LOWORD(adapter_identifer.DriverVersion.LowPart);
	DEBUG_TRACE("\t%d.%d.%d.%d\n", Product, Version, SubVersion, Build);
}

void stDeviceInfo::PrintCaps()
{
	DEBUG_TRACE("dev caps:\n");
	if (caps.DevCaps2 & D3DDEVCAPS2_STREAMOFFSET)
		DEBUG_TRACE("Device supports stream offsets \n");
}

//////////////////////////////////////////////////////////////////////////
#define MAX_VERTEX_SIZE	10000 * sizeof(VertexBase) * 4
#define MAX_INDEX_SIZE	10000 * sizeof(WORD) * 6

CDevice::CDevice()
: m_pD3D(NULL)
, m_pD3DDevice(NULL)
, m_pVB(NULL)
, m_pIB(NULL)
, m_dwVBOffset(0)
, m_dwIBOffset(0)
, m_CanvasRect()
{

}

CDevice::~CDevice()
{

}

void CDevice::Init()
{
	m_hWnd = GetWnd();
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (! m_pD3D)
	{
		DEBUG_TRACE("Create IDirect3D9 object failed!\n");
		return;
	}

	// adapter count
	int adapter_count = m_pD3D->GetAdapterCount();
	DEBUG_TRACE("adapter_count:%d\n", adapter_count);

	// adapter info
	if (FAILED(m_pD3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &m_DeviceInfo.adapter_identifer)))
	{
		DEBUG_TRACE("Get adapter identifier failed!\n");
		return;
	}
	m_DeviceInfo.PrintAdapterIdentifier();

	// current dispaly mode
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_DeviceInfo.display_mode)))
	{
		DEBUG_TRACE("Get adapter display mode failed!\n");
		return;
	}
	m_DeviceInfo.PrintCurDisplayMode();

	// adapter mode count
	DEBUG_TRACE("Other display mode:\n");
	int adapter_mode_count = m_pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8);
	for (int i = 0; i < adapter_mode_count; ++i)
	{
		D3DDISPLAYMODE display_mode;
		if (SUCCEEDED(m_pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, i, &display_mode)))
		{
			m_DeviceInfo.PrintDisplayMode(display_mode);
		}
	}

	// device caps
	if (FAILED(m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_DeviceInfo.caps)))
	{
		DEBUG_TRACE("Get device caps failed!\n");
		return;
	}
	m_DeviceInfo.PrintCaps();

	int vp = 0;
	if (m_DeviceInfo.caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Create device
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;
	d3dpp.hDeviceWindow = m_hWnd;
	d3dpp.BackBufferFormat = m_DeviceInfo.display_mode.Format;
	if (FAILED(m_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.BackBufferFormat, d3dpp.BackBufferFormat, d3dpp.Windowed)))
	{
		DEBUG_TRACE("Check device type failed!\n");
		return;
	}

	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, vp, &d3dpp, &m_pD3DDevice)))
	{
		DEBUG_TRACE("Create device failed!\n");
		return;
	}

	DEBUG_TRACE("Create device succeed!\n");

	SetUp();
}

void CDevice::Release()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pD3DDevice);
	SAFE_RELEASE(m_pD3D);
}

void CDevice::SetUp()
{
	if (! m_pD3DDevice)
		return;
	if (FAILED(m_pD3DDevice->CreateVertexBuffer(MAX_VERTEX_SIZE, D3DUSAGE_WRITEONLY, VertexBase_FVF, D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		DEBUG_TRACE("Create vertex buffer failed!\n");
		return;
	}
	
	if (FAILED(m_pD3DDevice->CreateIndexBuffer(MAX_INDEX_SIZE, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL)))
	{
		DEBUG_TRACE("Create index buffer failed!\n");
		return;
	}

	RECT rect;
	GetClientRect(m_hWnd, &rect);
	m_CanvasRect.SetRect(rect.left, rect.top, rect.right, rect.bottom);

	DEBUG_TRACE("Set up succeed!\n");
//	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CDevice::BeginScene(DWORD color)
{
	if (! m_pD3DDevice)
		return;
	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0);
	m_pD3DDevice->BeginScene();
	m_dwIBOffset = 0;
	m_dwVBOffset = 0;
}

void CDevice::EndScene()
{
	if (! m_pD3DDevice)
		return;

	m_pD3DDevice->EndScene();
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void CDevice::DrawRect(const RectF& rect, DWORD color)
{
	if (rect.IsRectEmpty())
		return;
	HRESULT hr = S_OK;
	VertexBase* v = NULL;
	int vb_size = sizeof(VertexBase) * 5;
	hr = m_pVB->Lock(m_dwVBOffset, vb_size, (void**)&v, D3DLOCK_NOOVERWRITE);
	if (FAILED(hr))
	{
		DEBUG_DXTRACE(hr);
		return;
	}

	v[0].x = rect.left;		v[0].y = rect.top;		v[0].z = 0.0f;	v[0].w = 1.0f;	v[0].color = color;
	v[1].x = rect.right;	v[1].y = rect.top;		v[1].z = 0.0f;	v[1].w = 1.0f;	v[1].color = color;
	v[2].x = rect.right;	v[2].y = rect.bottom;	v[2].z = 0.0f;	v[2].w = 1.0f;	v[2].color = color;
	v[3].x = rect.left;		v[3].y = rect.bottom;	v[3].z = 0.0f;	v[3].w = 1.0f;	v[3].color = color;
	v[4].x = rect.left;		v[4].y = rect.top;		v[4].z = 0.0f;	v[4].w = 1.0f;	v[4].color = color;
	m_pVB->Unlock();

	m_pD3DDevice->SetStreamSource(0, m_pVB, m_dwVBOffset, sizeof(VertexBase));
	m_pD3DDevice->SetFVF(VertexBase_FVF);
	m_pD3DDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, 4);

	m_dwVBOffset += vb_size;
}

void CDevice::FillRect(const RectF& rect, DWORD color)
{
	if (rect.IsRectEmpty())
		return;
	HRESULT hr = S_OK;
	VertexBase* v = NULL;
	int vb_size = sizeof(VertexBase) * 4;
	hr = m_pVB->Lock(m_dwVBOffset, vb_size, (void**)&v, D3DLOCK_NOOVERWRITE);
	if (FAILED(hr))
	{
		DEBUG_DXTRACE(hr);
		return;
	}

	v[0].x = rect.left;		v[0].y = rect.bottom;	v[0].z = 0.0f;	v[0].w = 1.0f;	v[0].color = color;
	v[1].x = rect.left;		v[1].y = rect.top;		v[1].z = 0.0f;	v[1].w = 1.0f;	v[1].color = color;
	v[2].x = rect.right;	v[2].y = rect.bottom;	v[2].z = 0.0f;	v[2].w = 1.0f;	v[2].color = color;
	v[3].x = rect.right;	v[3].y = rect.top;		v[3].z = 0.0f;	v[3].w = 1.0f;	v[3].color = color;
	m_pVB->Unlock();

	m_pD3DDevice->SetStreamSource(0, m_pVB, m_dwVBOffset, sizeof(VertexBase));
	m_pD3DDevice->SetFVF(VertexBase_FVF);
	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	m_dwVBOffset += vb_size;
}

void CDevice::FillRectList(const RectF* rect, DWORD num, DWORD color)
{
	if (! rect)
		return;
	HRESULT hr = S_OK;
	VertexBase* v = NULL;
	int vb_size = sizeof(VertexBase) * 4 * num;
	hr = m_pVB->Lock(m_dwVBOffset, vb_size, (void**)&v, D3DLOCK_NOOVERWRITE);
	if (FAILED(hr))
	{
		DEBUG_DXTRACE(hr);
		return;
	}

	for (int i = 0; i < num; ++i)
	{
		v->x = rect->left;	v->y = rect->top;		v->z = 0.0f;	v->w = 1.0f;	v->color = color;
		++v;
		v->x = rect->right;	v->y = rect->top;		v->z = 0.0f;	v->w = 1.0f;	v->color = color;
		++v;
		v->x = rect->right;	v->y = rect->bottom;	v->z = 0.0f;	v->w = 1.0f;	v->color = color;
		++v;
		v->x = rect->left;	v->y = rect->bottom;	v->z = 0.0f;	v->w = 1.0f;	v->color = color;
		++v;	++rect;
	}
	m_pVB->Unlock();

	WORD* w = NULL;
	int ib_size = sizeof(WORD) * 6 * num;
	hr = m_pIB->Lock(m_dwIBOffset, ib_size, (void**)&w, D3DLOCK_NOOVERWRITE);
	if (FAILED(hr))
	{
		DEBUG_DXTRACE(hr);
		return;
	}

	int ib_num = 6 * num;
	for (int i = 0, j = 0; i < ib_num; i += 6, j += 4)
	{
		w[i] = j + 3;
		w[i + 1] = j;
		w[i + 2] = j + 1;
		w[i + 3] = j + 3;
		w[i + 4] = j + 1;
		w[i + 5] = j + 2;
	}

	m_pD3DDevice->SetStreamSource(0, m_pVB, m_dwVBOffset, sizeof(VertexBase));
	m_pD3DDevice->SetIndices(m_pIB);
	m_pD3DDevice->SetFVF(VertexBase_FVF);
	m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4 * num, 0, 2 * num);

	m_dwVBOffset += vb_size;
	m_dwIBOffset += ib_size;
}


void CDevice::DrawLine(const Vec2F& pt0, const Vec2F& pt1, DWORD color)
{
	HRESULT hr = S_OK;
	VertexBase* v = NULL;
	int vb_size = sizeof(VertexBase) * 2;
	hr = m_pVB->Lock(m_dwVBOffset, vb_size, (void**)&v, D3DLOCK_NOOVERWRITE);
	if (FAILED(hr))
	{
		DEBUG_DXTRACE(hr);
		return;
	}

	v[0].x = pt0.x;		v[0].y = pt0.y;		v[0].z = 0.0f;	v[0].w = 1.0f;	v[0].color = color;
	v[1].x = pt1.x;		v[1].y = pt1.y;		v[1].z = 0.0f;	v[1].w = 1.0f;	v[1].color = color;
	m_pVB->Unlock();

	m_pD3DDevice->SetStreamSource(0, m_pVB, m_dwVBOffset, sizeof(VertexBase));
	m_pD3DDevice->SetFVF(VertexBase_FVF);
	m_pD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);

	m_dwVBOffset += vb_size;
}

void CDevice::DrawLineStrip(const Vec2F* p, DWORD num, DWORD color)
{
	if (! p || num < 2)
		return;
	HRESULT hr = S_OK;
	VertexBase* v = NULL;
	int vb_size = sizeof(VertexBase) * num;
	hr = m_pVB->Lock(m_dwVBOffset, vb_size, (void**)&v, D3DLOCK_NOOVERWRITE);
	if (FAILED(hr))
	{
		DEBUG_DXTRACE(hr);
		return;
	}
	
	for (DWORD i = 0; i < num; ++i, ++v, ++p)
	{
		v->x = p->x;
		v->y = p->y;
		v->z = 0.0f;
		v->w = 1.0f;
		v->color = color;
	}
	m_pVB->Unlock();

	m_pD3DDevice->SetStreamSource(0, m_pVB, m_dwVBOffset, sizeof(VertexBase));
	m_pD3DDevice->SetFVF(VertexBase_FVF);
	m_pD3DDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, num - 1);

	m_dwVBOffset += vb_size;
}

void CDevice::DrawLineList(const Vec2F* p, DWORD num, DWORD color)
{
	if (! p || num < 2)
		return;
	HRESULT hr = S_OK;
	VertexBase* v = NULL;
	int line_num = num / 2;
	int vb_size = sizeof(VertexBase) * line_num * 2;
	hr = m_pVB->Lock(m_dwVBOffset, vb_size, (void**)&v, D3DLOCK_NOOVERWRITE);
	if (FAILED(hr))
	{
		DEBUG_DXTRACE(hr);
		return;
	}

	for (int i = 0; i < line_num; ++i)
	{
		v->x = p->x;	v->y = p->y;	v->z = 0.0f;	v->w = 1.0f;	v->color = color;
		++v;
		++p;
		v->x = p->x;	v->y = p->y;	v->z = 0.0f;	v->w = 1.0f;	v->color = color;
		++v;
		++p;
	}
	m_pVB->Unlock();

	m_pD3DDevice->SetStreamSource(0, m_pVB, m_dwVBOffset, sizeof(VertexBase));
	m_pD3DDevice->SetFVF(VertexBase_FVF);
	m_pD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, line_num);

	m_dwVBOffset += vb_size;
}

void CDevice::DrawText(const char* text, const Vec2F& pt, BYTE format, DWORD color)
{
//	m_pTextOut->DrawText(text, pt, format, color);
}


NAMESPACE_END