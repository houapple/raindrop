#ifndef _RD_CURSOR_H
#define _RD_CURSOR_H

class CCursor
{
public:
	Vec2I GetPosition() { Update(); return m_Position; }
	void Update()
	{
		POINT pt;
		GetCursorPos(&pt);
		extern HWND GetWnd();
		ScreenToClient(GetWnd(), &pt);
		m_Position.Set(pt.x, pt.y);
	}

private:
	Vec2I	m_Position;
};

#endif
