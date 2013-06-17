#ifndef _RD_RENDER_H
#define _RD_RENDER_H

#include "../rd_core/rd_types.h"

class IRender
{
public:
	virtual void InitDevice() = 0;
	virtual void ReleaseDevice() = 0;

	virtual void BeginScene() = 0;
	virtual void EndScene() = 0;

	virtual void DrawRect(const RectF& rect, DWORD color) = 0;
};

#endif