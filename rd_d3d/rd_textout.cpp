#include "rd_textout.h"
#include "rd_utility.h"
#include "../rd_render/interface_device.h"

void CTextOut::Init(IDirect3DDevice9* pd3dd)
{
	if (! pd3dd)
		return;
	
	D3DXFONT_DESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.Height = 12;
	desc.Width = 0;
	desc.Weight = 400;
	desc.MipLevels = D3DX_DEFAULT;
	desc.Italic = false;
	desc.CharSet = DEFAULT_CHARSET;
	desc.OutputPrecision = 0;
	desc.Quality = 0;
	desc.PitchAndFamily = 0;
	rd_strncpy(desc.FaceName, "ËÎÌו", sizeof(desc.FaceName));

	HRESULT hr = D3DXCreateFontIndirect(pd3dd, &desc, &m_pFont);
	if (FAILED(hr))
	{
		DEBUG_DXTRACE(hr);
	}
}

void CTextOut::Release()
{
	SAFE_RELEASE(m_pFont);
}

void CTextOut::DrawString(const char* text, const Vec2F& pt, BYTE format, DWORD color)
{
	RECT rect;
	rect.left = pt.x;
	rect.top = pt.y;
	rect.right = pt.x + 500;
	rect.bottom = pt.x + 30;

	DWORD f = 0;
	if (format & eDT_Left)
		f |= DT_LEFT;
	if (format & eDT_Top)
		f |= DT_TOP;
	m_pFont->DrawText(NULL, text, -1, &rect, f, color);
}

