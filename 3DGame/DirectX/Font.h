/**
* @file Font.h
* @brief 2Dフォント
* @author Syota Tachikawa
*/

#ifndef ___FONT_H
#define ___FONT_H

#include "..\Common\CommonLib.h"

/**
* @brief 2Dフォントを管理するクラス
*/
class Font
{
public:
	/**
	* @brief コンストラクタ
	*/
	Font();

	/**
	* @brief デストラクタ
	*/
	~Font();

	/**
	* @brief フォントを描画する
	*/
	void DrawFont(LPSTR str, LONG x, LONG y, LONG width, LONG height, DWORD color, BOOL bMini = FALSE);

private:
	LPD3DXFONT m_pd3dFont;

	LPD3DXFONT m_pd3dFontM;
};
#endif