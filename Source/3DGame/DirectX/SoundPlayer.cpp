#include "SoundPlayer.h"

/**
* @brief 初期化
*/
HRESULT SoundPlayer::Initialize()
{
	// インスタンス取得
	m_dSound = make_unique<DirectSound>();

	// 成功
	return S_OK;
}