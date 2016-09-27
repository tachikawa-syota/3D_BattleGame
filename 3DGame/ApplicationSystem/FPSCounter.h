/**
* @file   FPSCounter.h
* @brief  FPS���v������
* @author Syota Tachikawa
*/

#ifndef ___FPSCOUNTER_H
#define ___FPSCOUNTER_H

#include "..\Common\CommonLib.h"

/**
* @enum FPSMode
* @brief FPS���[�h
*/
enum class FPSMode : BYTE
{
	/// �Œ�U�O�t���[��
	FPS_60,

	/// �Œ�R�O�t���[��
	FPS_30,

	/// �σt���[��
	FPS_FLEX
};

/**
* @brief FPS���v������N���X
*/
class FPSCounter
{
public:
	/**
	* @brief �R���X�g���N�^
	* @param FPS���[�h�̎w��
	*/
	FPSCounter(FPSMode fpsMode);

	/**
	* @brief �f�X�g���N�^
	*/
	~FPSCounter();

	/**
	* @brief �X�V
	*/
	BOOL Update();

	/**
	* @brief �`��t���O�̎擾
	*/
	BOOL IsRender();

	/**
	* @brief �t���[�����[�g�̎擾
	*/
	DWORD GetFPS();

	/**
	* @brief �`��t���[�����[�g�̎擾
	*/
	DWORD GetRenderFPS();

private:
	/// �`��t���O
	BOOL bRender;

	/// �N������̃t���[����
	DWORD m_dwGameFrame;

	/// �t���[�����䃂�[�h
	FPSMode	m_FPSMode;
	DWORD m_dwFrameTime;

	/// �����t���[�����[�g
	DWORD m_dwFPS;

	/// �`��t���[�����[�g
	DWORD m_dwRenderFPS;

	/// �����J�E���^
	DWORD m_dwCurFrame;

	/// �`��J�E���^
	DWORD m_dwRCurFrame;
};

#endif