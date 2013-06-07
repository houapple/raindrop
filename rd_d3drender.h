#pragma once

#include "rd_render.h"
#include "rd_core.h"
#include <d3d9.h>


NAMESPACE_BEGINE(d3d)

class CDeviceInfo
{
public:
	D3DDISPLAYMODE display_mode;
	D3DADAPTER_IDENTIFIER9 adapter_identifer;
	D3DCAPS9 caps;

	void PrintDisplayMode(D3DDISPLAYMODE& display_mode);
	void PrintCurDisplayMode() { PrintDisplayMode(display_mode); }
	void PrintAdapterIdentifier();
};


class CRender : public IRender
{
public:
	CRender();
	~CRender();
	
	void InitDevice();

	void ReleaseDevice();
	
	void SetUp();
	
	void BeginScene();
	
	void EndScene();
	
	void Render();

	// Draw triange list, use index buffer if index != NULL
	void DrawTriangeList(DWORD size, const Vertex* vertex, DWORD vertexNum, const WORD* index, DWORD indexNum);
private:
	IDirect3D9*				m_pD3D;
	IDirect3DDevice9*		m_pD3DDevice;
	IDirect3DVertexBuffer9* m_pVB;
	IDirect3DIndexBuffer9*	m_pIB;
	
	HWND m_hWnd;
	CDeviceInfo m_DeviceInfo;

	// render queue
	struct stUnit
	{
		DWORD size;
		void* pVertex;
		void* pIndex;
		WORD singleVertexSize;
		DWORD vertexSize;
		DWORD indexSize;
		stUnit() : size(0), pVertex(0), pIndex(0), vertexSize(0), indexSize(0) { }
	};
	std::queue<stUnit>	m_queueUnit;
};

NAMESPACE_END