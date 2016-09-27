/**
* @file   FrameWork.h
* @brief  �t���[�����[�N
* @author Syota Tachikawa
*/

#ifndef ___FRAMEWORK_H
#define ___FRAMEWORK_H

#include "..\Common\SingletonOwner.h"
#include "..\ApplicationSystem\WindowsApplication.h"
#include "..\DirectX\Direct3DSystem.h"
#include "..\DirectX\Font.h"
#include "FPSCounter.h"

/**
* @brief �t���[�����[�N�N���X
*/
class FrameWork
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	FrameWork();
	
	/**
	* @brief �f�X�g���N�^
	*/
	~FrameWork();

	/**
	* @brief �쐬
	* @param �C���X�^���X
	* @return true - �����Ffalse - ���s
	*/
	bool Create(HINSTANCE hInst);

	/**
	* @brief ������
	*/
	HRESULT Initialize();

	/**
	* @brief �X�V
	*/
	void Update();

	/**
	* @brief �`��
	*/
	void Render();

	/**
	* @brief �X�V�ƕ`��
	*/
	void Run();

private:
	/// �A�v���P�[�V�����V�X�e��
	unique_ptr<WindowsApplication> m_pSys;

	/// D3D�V�X�e��
	unique_ptr<Direct3DSystem> m_pD3d;

	/// �V�[��
	ISequencePtr m_nowScene;

	/// �t�H���g
	unique_ptr<Font> m_font;

	/// �V���O���g���I�[�i�[
	unique_ptr<SingletonOwner> m_singletonOwner;

	/// FPS�J�E���^�[
	unique_ptr<FPSCounter> m_fpsCounter;

	/**
	* @brief ��ʃN���A
	*/
	BOOL HCls();

	/**
	* @brief �t���b�s���O
	*/
	BOOL Flip();

	/**
	* @brief FPS�̕\��
	*/
	VOID RenderFPS();
};



#endif