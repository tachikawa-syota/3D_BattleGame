/**
* @file DirectSound.h
* @brief �_�C���N�g�T�E���h
* @author Syota Tachikawa
*/

#ifndef ___DIRECTSOUND_H
#define ___DIRECTSOUND_H

#include <dsound.h>
#include "..\Common\CommonLib.h"

/**
* @brief �_�C���N�g�T�E���h�N���X
*/
class DirectSound
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	DirectSound();

	/**
	* @brief �f�X�g���N�^
	*/
	~DirectSound();

	/**
	* @brief ���[�h
	* @param ���[�h����T�E���h�t�@�C���̖��O
	*/
	BOOL Load(LPSTR fileName);

	/**
	* @brief �Đ�
	* @param �T�E���h�t�@�C����
	*/
	HRESULT Play(LPSTR fileName);

	/**
	* @brief ��~
	* @param �T�E���h�t�@�C����
	*/
	HRESULT Stop(LPSTR fileName);

	/**
	* @brief ���[�v�Đ�
	* @param �T�E���h�t�@�C����
	*/
	HRESULT LoopPlay(LPSTR fileName);

	/**
	* @brief �Đ������ǂ������`�F�b�N����
	* @param �T�E���h�t�@�C����
	*/
	BOOL IsPlay(LPSTR fileName);

	/**
	* @brief ���ʂ̐ݒ�
	* @param �T�E���h�t�@�C����
	*/
	void SetVolume(LPSTR fileName, int volume);

	/**
	* @brief �w�肵���T�E���h���������
	* @param �T�E���h�t�@�C����
	*/
	void SoundSingleFree(LPSTR fileName);

private:
	/// �T�E���h�f�o�C�X
	LPDIRECTSOUND8 pDS8;

	/// �T�E���h�o�b�t�@�[
	map<string, LPDIRECTSOUNDBUFFER> m_soundBuffer;


};

#endif