#include "SoundPlayer.h"

/**
* @brief ������
*/
HRESULT SoundPlayer::Initialize()
{
	// �C���X�^���X�擾
	m_dSound = make_unique<DirectSound>();

	// ����
	return S_OK;
}