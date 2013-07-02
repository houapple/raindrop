#ifndef _RD_CURSOR_H
#define _RD_CURSOR_H

#include "rd_types.h"

class CCursor
{
public:
	Vec2I GetPosition() { Update(); return m_Position; }
	void Update();

private:
	Vec2I	m_Position;
};

inline CCursor* GetAppCursor()
{
	static CCursor cursor;
	return &cursor;
}

#endif
