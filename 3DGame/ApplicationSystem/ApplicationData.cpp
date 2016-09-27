#include "ApplicationData.h"

/**
* @brief �A�v���P�[�V�����̖��O��ݒ肷��
*/
void ApplicationData::SetApplicationName(LPCTSTR appName)
{
	m_applicationName = appName;
}

/**
* @brief �E�B���h�E�n���h���̐ݒ�
*/
void ApplicationData::SetWindowHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}

/**
* @brief �E�B���h�E�̃T�C�Y��ݒ肷��
* @param width  �F ����
* @param height �F �c��
*/
void ApplicationData::SetWindowSize(const DWORD width, const DWORD height)
{
	m_Width = width;
	m_Height = height;
}

/**
* @brief �E�B���h�E���[�h��ݒ肷��
*/
void ApplicationData::SetWindowed(bool windowed)
{
	m_windowed = windowed;
}

/**
* @brief �A�v���P�[�V�����̖��O���擾����
*/
LPCTSTR ApplicationData::GetApplicationName()
{
	return m_applicationName;
}

/**
* @brief �E�B���h�E�n���h���̎擾
*/
HWND ApplicationData::GetWindowHandle()
{
	return m_hWnd;
}

/**
* @brief �E�B���h�E�̉������擾����
*/
DWORD ApplicationData::GetWindowWidth()
{
	return m_Width;
}

/**
* @brief �E�B���h�E�̏c�����擾����
*/
DWORD ApplicationData::GetWindowHeight()
{
	return m_Height;
}

/**
* @brief �E�B���h�E���[�h���擾����
*/
bool ApplicationData::GetWindowed()
{
	return m_windowed;
}