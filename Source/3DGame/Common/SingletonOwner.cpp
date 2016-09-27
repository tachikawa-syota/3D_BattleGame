#include "SingletonOwner.h"

/**
* @brief ������
*/
bool SingletonOwner::Initialize()
{
	m_deviceManager = DeviceManager::GetInstance();
	// �f�o�C�X�}�l�[�W���[�̏�����
	if (FAILED(m_deviceManager->Initialize())){
		return false;
	}
	// �f�o�C�X�}�l�[�W���[�̃Z�b�g�A�b�v
	if (FAILED(m_deviceManager->SetUp())){
		return false;
	}

	// �e�N�X�`���}�l�[�W���[�̏�����
	m_textureManager = TextureManager::GetInstance();
	if (FAILED(m_textureManager->Initialize())){
		return false;
	}

	// �T�E���h�v���C���[�̏�����
	m_soundPlayer = SoundPlayer::GetInstance();
	if (FAILED(m_soundPlayer->Initialize())){
		return false;
	}

	// �V�F�[�_�[�}�l�[�W���[�̏�����
	m_shaderManager = ShaderManager::GetInstance();
	if (FAILED(m_shaderManager->Create())){
		return false;
	}

	// �V�[�N�G���X�`�F���W���[�̏�����
	m_sequenceChanger = SequenceChanger::GetInstance();
	m_sequenceChanger->Initialize();

	// �X�N���[���G�t�F�N�g�̏�����
	m_screenEffect = ScreenEffect::GetInstance();
	m_screenEffect->Initialize();

	// ������΂��␳�l�N���X�̏�����
	m_correctionValue = CorrectionValue::GetInstance();
	m_correctionValue->Initialize();

	return true;
}