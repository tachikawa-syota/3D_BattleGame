/**
* @file SoundPlayer.h
* @brief �T�E���h���Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___SOUNDPLAYER_H
#define ___SOUNDPLAYER_H

#include "DirectSound.h"

/**
* @brief �T�E���h���Đ�����N���X
*/
class SoundPlayer
{
public:
	/**
	* @brief �Q�b�g�C���X�^���X
	*/
	static SoundPlayer* GetInstance(){
		static SoundPlayer singleton;
		return &singleton;
	}

	/**
	* @brief ������
	*/
	HRESULT Initialize();

	/**
	* @brief ���[�h
	* @param �T�E���h�t�@�C����
	*/
	BOOL Load(LPSTR fileName){
		return m_dSound->Load(fileName);
	}

	/**
	* @brief �Đ�
	* @param �T�E���h�t�@�C����
	*/
	HRESULT Play(LPSTR fileName){
		return m_dSound->Play(fileName);
	}

	/**
	* @brief ��~
	* @param �T�E���h�t�@�C����
	*/
	HRESULT Stop(LPSTR fileName){
		return m_dSound->Stop(fileName);
	}

	/**
	* @brief ���[�v�Đ�
	* @param �T�E���h�t�@�C����
	*/
	HRESULT LoopPlay(LPSTR fileName){
		return m_dSound->LoopPlay(fileName);
	}

	/**
	* @brief �Đ������ǂ������`�F�b�N����
	* @param �T�E���h�t�@�C����
	*/
	BOOL IsPlay(LPSTR fileName){
		return m_dSound->IsPlay(fileName);
	}

private:
	/// �_�C���N�g�T�E���h
	unique_ptr<DirectSound> m_dSound;
};

#endif