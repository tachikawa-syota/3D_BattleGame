#include "Font.h"
#include "..\DirectX\Direct3DData.h"

/**
* @brief コンストラクタ
*/
Font::Font()
{
	m_pd3dFont = nullptr;
	m_pd3dFontM = nullptr;

	HFONT		hF;
	D3DXFONT_DESC	lf;

	// フォント設定
	hF = (HFONT)GetStockObject(SYSTEM_FONT);
	GetObject(hF, sizeof(LOGFONT), (LPSTR)&lf);
	lf.Height = 24;
	lf.Width = 0;
	lf.Italic = 0;
	lf.CharSet = SHIFTJIS_CHARSET;
	strcpy_s(lf.FaceName, "ＭＳ ゴシック");

	D3DXCreateFontIndirect(D3DIns->GetDevice(), &lf, &m_pd3dFont);

	lf.Height = 16;
	D3DXCreateFontIndirect(D3DIns->GetDevice(), &lf, &m_pd3dFontM);
}

/**
* @brief デストラクタ
*/
Font::~Font()
{
	SafeRelease(m_pd3dFont);
	SafeRelease(m_pd3dFontM);
}

/**
* @brief フォントを描画する
*/
void Font::DrawFont(LPSTR str, LONG x, LONG y, LONG width, LONG height, DWORD color, BOOL bMini)
{
	RECT rect;

	rect.left = x;
	rect.top = y;
	rect.right = x + width;
	rect.bottom = y + height;

	D3DIns->GetDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE);

	if (!bMini) m_pd3dFont->DrawText(NULL, str, -1, &rect, DT_LEFT | DT_WORDBREAK, color);
	else        m_pd3dFontM->DrawText(NULL, str, -1, &rect, DT_LEFT | DT_WORDBREAK, color);
}