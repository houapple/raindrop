#ifndef _RD_TEXTOUT_H
#define _RD_TEXTOUT_H

class CTextOut
{
public:
	void Init();
	void Release();
	void DrawText(const char* text, const Vec2F& pt, BYTE format, DWORD color);
};

#endif

