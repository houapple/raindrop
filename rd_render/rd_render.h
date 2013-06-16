#pragma once
#include "../rd_core/rd_types.h"

struct VertexBase
{
	float x, y, z, w;
	DWORD color;
};
#define VertexBase_FVF	D3DFVF_XYZRHW | D3DFVF_DIFFUSE


class IRender
{
public:
	virtual void InitDevice() = 0;
	virtual void ReleaseDevice() = 0;

	virtual void BeginScene() = 0;
	virtual void EndScene() = 0;
	virtual void Render() = 0;
};