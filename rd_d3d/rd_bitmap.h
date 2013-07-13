#ifndef _RD_BITMAP_H
#define _RD_BITMAP_H

#include "../rd_render/interface_bitmap.h"

class CBitmap : public IBitmap
{
public:
	CBitmap();
	~CBitmap() {}

	void Render(const Vec2F& pt, const Vec2F& scale);

private:
	bool m_bLoad;
};




#endif