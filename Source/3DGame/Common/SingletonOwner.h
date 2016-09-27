/**
* @file   SingletonOwner.h
* @brief  �V���O���g�����܂Ƃ߂ĊǗ�����
* @author Syota Tachikawa
*/

#ifndef ___SINGLETONOWNER_H
#define ___SINGLETONOWNER_H

#include "..\DirectX\DeviceManager.h"
#include "..\DirectX\SoundPlayer.h"
#include "..\Sequence\ScreenEffect.h"
#include "..\Sequence\SequenceChanger.h"
#include "..\GameSystem\CorrectionValue.h"
#include "..\Shader\ShaderManager.h"

/**
* @brief �V���O���g�����܂Ƃ߂ĊǗ�����N���X
*/
class SingletonOwner
{
public:
	/**
	* @brief ������
	* @return true - ���� �F false - ���s
	*/
	bool Initialize();

private:
	/// �f�o�C�X
	DeviceManager* m_deviceManager;

	/// �O���t�B�b�N�X
	TextureManager* m_textureManager;

	/// �T�E���h
	SoundPlayer* m_soundPlayer;

	/// �V�F�[�_�[
	ShaderManager* m_shaderManager;

	/// �V�[���`�F���W���[
	SequenceChanger* m_sequenceChanger;

	/// �X�N���[���G�t�F�N�g
	ScreenEffect* m_screenEffect;

	/// �␳�l�f�[�^
	CorrectionValue* m_correctionValue;
};

#endif