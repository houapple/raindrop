#ifndef _INTERFACE_DEVICE_H
#define _INTERFACE_DEVICE_H

#include "../rd_core/rd_types.h"

enum eDrawText
{
	eDT_Left	= 0x1,
	eDT_Top		= 0x2,
	eDT_Right	= 0x4,
	eDT_Bottom	= 0x8,
	eDT_Center	= 0x10,
	eDT_VCenter	= 0x20,
};

class IDevice
{
public:
	IDevice() {}
	virtual ~IDevice() {}
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

	virtual void DrawString(const char* text, const Vec2F& pt, BYTE format, DWORD color) = 0;
};

IDevice* GetDevice();

#endif