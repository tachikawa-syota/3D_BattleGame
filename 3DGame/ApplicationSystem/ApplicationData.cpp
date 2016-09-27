#include "ApplicationData.h"

/**
* @brief アプリケーションの名前を設定する
*/
void ApplicationData::SetApplicationName(LPCTSTR appName)
{
	m_applicationName = appName;
}

/**
* @brief ウィンドウハンドルの設定
*/
void ApplicationData::SetWindowHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}

/**
* @brief ウィンドウのサイズを設定する
* @param width  ： 横幅
* @param height ： 縦幅
*/
void ApplicationData::SetWindowSize(const DWORD width, const DWORD height)
{
	m_Width = width;
	m_Height = height;
}

/**
* @brief ウィンドウモードを設定する
*/
void ApplicationData::SetWindowed(bool windowed)
{
	m_windowed = windowed;
}

/**
* @brief アプリケーションの名前を取得する
*/
LPCTSTR ApplicationData::GetApplicationName()
{
	return m_applicationName;
}

/**
* @brief ウィンドウハンドルの取得
*/
HWND ApplicationData::GetWindowHandle()
{
	return m_hWnd;
}

/**
* @brief ウィンドウの横幅を取得する
*/
DWORD ApplicationData::GetWindowWidth()
{
	return m_Width;
}

/**
* @brief ウィンドウの縦幅を取得する
*/
DWORD ApplicationData::GetWindowHeight()
{
	return m_Height;
}

/**
* @brief ウィンドウモードを取得する
*/
bool ApplicationData::GetWindowed()
{
	return m_windowed;
}