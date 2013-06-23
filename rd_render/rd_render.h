#ifndef _RD_RENDER_H
#define _RD_RENDER_H

#include "../rd_core/rd_types.h"

class IRender
{
public:
	virtual void InitDevice() = 0;
	virtual void ReleaseDevice() = 0;

	virtual void BeginScene(DWORD color) = 0;
	virtual void EndScene() = 0;

	virtual void DrawRect(const RectF& rect, DWORD color) = 0;
	virtual void FillRect(const RectF& rect, DWORD color) = 0;
	virtual void FillRectList(const RectF* rect, DWORD num, DWORD color) = 0;
	virtual void DrawLine(const Vec2F& pt0, const Vec2F& pt1, DWORD color) = 0;
	virtual void DrawLineStrip(const Vec2F* p, DWORD num, DWORD color) = 0;
	virtual void DrawLineList(const Vec2F* p, DWORD num, DWORD color) = 0;
};

#endif