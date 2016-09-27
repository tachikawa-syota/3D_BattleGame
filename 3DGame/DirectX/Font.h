/**
* @file Font.h
* @brief 2D�t�H���g
* @author Syota Tachikawa
*/

#ifndef ___FONT_H
#define ___FONT_H

#include "..\Common\CommonLib.h"

/**
* @brief 2D�t�H���g���Ǘ�����N���X
*/
class Font
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	Font();

	/**
	* @brief �f�X�g���N�^
	*/
	~Font();

	/**
	* @brief �t�H���g��`�悷��
	*/
	void DrawFont(LPSTR str, LONG x, LONG y, LONG width, LONG height, DWORD color, BOOL bMini = FALSE);

private:
	LPD3DXFONT m_pd3dFont;

	LPD3DXFONT m_pd3dFontM;
};
#endif