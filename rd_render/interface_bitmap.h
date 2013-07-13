#ifndef _INTERFACE_BITMAP_H
#define _INTERFACE_BITMAP_H

#include "../rd_core/rd_types.h"

class IBitmap
{
public:
	IBitmap() {}
	virtual ~IBitmap() {}
	virtual void Render(const Vec2F& pt, const Vec2F& scale) = 0;
};

#endif