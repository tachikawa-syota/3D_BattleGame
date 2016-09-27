#include "TextureManager.h"

/**
* @brief 初期化
*/
HRESULT TextureManager::Initialize()
{
	// インスタンス取得
	m_p2Dtex = new Texture2D();

	// 成功
	return S_OK;
}