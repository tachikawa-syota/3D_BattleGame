/**
* @file ApplicationData.h
* @brief �A�v���P�[�V�����V�X�e���f�[�^�̊Ǘ�
* @author Syota Tachikawa
*/

#ifndef ___APPLICATIONSYSTEMDATA_H
#define ___APPLICATIONSYSTEMDATA_H

#include "..\Common\CommonLib.h"

// �V�X�e���f�[�^�̃C���X�^���X
#define AppIns ApplicationData::GetInstance()

/**
* @brief �A�v���P�[�V�����̃V�X�e���f�[�^���Ǘ�����N���X
*/
class ApplicationData
{
public:
	/**
	* @brief �Q�b�g�C���X�^���X
	*/
	static ApplicationData* GetInstance(){
		static ApplicationData singlton;
		return &singlton;
	}

	/**
	* @brief �A�v���P�[�V�����̖��O��ݒ肷��
	*/
	void SetApplicationName(LPCTSTR appName);

	/**
	* @brief �E�B���h�E�n���h���̐ݒ肷��
	*/
	void SetWindowHandle(HWND hWnd);

	/**
	* @brief �E�B���h�E�̃T�C�Y��ݒ肷��
	* @param width  �F ����
	* @param height �F �c��
	*/
	void SetWindowSize(const DWORD width, const DWORD height);

	/**
	* @brief �E�B���h�E���[�h��ݒ肷��
	*/
	void SetWindowed(bool windowed);

	/**
	* @brief �A�v���P�[�V�����̖��O���擾����
	*/
	LPCTSTR GetApplicationName();

	/**
	* @brief �E�B���h�E�n���h���̎擾����
	*/
	HWND GetWindowHandle();

	/**
	* @brief �E�B���h�E�̉������擾����
	*/
	DWORD GetWindowWidth();

	/**
	* @brief �E�B���h�E�̏c�����擾����
	*/
	DWORD GetWindowHeight();

	/**
	* @brief �E�B���h�E���[�h���擾����
	*/
	bool GetWindowed();

private:
	/// �E�B���h�E�n���h��
	HWND m_hWnd;

	/// �A�v���P�[�V�����̖��O
	LPCTSTR m_applicationName;

	/// �E�B���h�E�̉���
	DWORD m_Width;

	/// �E�B���h�E�̏c��
	DWORD m_Height;

	/// �E�B���h�E���[�h
	bool m_windowed;
};

#endif