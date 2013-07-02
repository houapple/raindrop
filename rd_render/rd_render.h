#ifndef _RD_RENDER_H
#define _RD_RENDER_H

#include "../rd_core/rd_types.h"

enum eTextFormat
{
	eTF_Left	= 0x1,
	eTF_Top		= 0x2,
	eTF_Right	= 0x4,
	eTF_Bottom	= 0x8,
	eTF_Center	= 0x10,
	eTF_VCenter	= 0x20,
};

class IRender
{
public:
	virtual void Init() = 0;
	virtual void Release() = 0;

	virtual void BeginScene(DWORD color) = 0;
	virtual void EndScene() = 0;

	virtual void DrawRect(const RectF& rect, DWORD color) = 0;
	virtual void FillRect(const RectF& rect, DWORD color) = 0;
	virtual void FillRectList(const RectF* rect, DWORD num, DWORD color) = 0;
	virtual void DrawLine(const Vec2F& pt0, const Vec2F& pt1, DWORD color) = 0;
	virtual void DrawLineStrip(const Vec2F* p, DWORD num, DWORD color) = 0;
	virtual void DrawLineList(const Vec2F* p, DWORD num, DWORD color) = 0;

	virtual void DrawText(const char* text, const Vec2F& pt, BYTE format, DWORD color) = 0;
};

IRender* GetRender();

#endif