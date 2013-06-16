#pragma once
#include "rd_types.h"

struct Vertex
{
	float x, y, z, w;
	DWORD color;
	static const DWORD FVF;
};

class IRender
{
public:
	virtual void InitDevice() = 0;
	virtual void ReleaseDevice() = 0;

	virtual void BeginScene() = 0;
	virtual void EndScene() = 0;
	virtual void Render() = 0;
	// Draw triange list, use index buffer if index != NULL
	virtual void DrawTriangeList(DWORD size, const Vertex* vertex, DWORD vertexNum, const WORD* index, DWORD indexNum) = 0;
};