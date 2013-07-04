#ifndef _RD_TEXTOUT_H
#define _RD_TEXTOUT_H

#include <d3dx9core.h>
#include <d3d9.h>
#include "../rd_core/rd_core.h"

class CTextOut
{
public:
	CTextOut() : m_pFont(NULL) {}
	void Init(IDirect3DDevice9* pd3dd);
	void Release();
	void DrawString(const char* text, const Vec2F& pt, BYTE format, DWORD color);

private:
	ID3DXFont*	m_pFont;
};

#endif

