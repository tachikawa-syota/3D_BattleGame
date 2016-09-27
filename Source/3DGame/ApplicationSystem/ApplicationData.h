/**
* @file ApplicationData.h
* @brief アプリケーションシステムデータの管理
* @author Syota Tachikawa
*/

#ifndef ___APPLICATIONSYSTEMDATA_H
#define ___APPLICATIONSYSTEMDATA_H

#include "..\Common\CommonLib.h"

// システムデータのインスタンス
#define AppIns ApplicationData::GetInstance()

/**
* @brief アプリケーションのシステムデータを管理するクラス
*/
class ApplicationData
{
public:
	/**
	* @brief ゲットインスタンス
	*/
	static ApplicationData* GetInstance(){
		static ApplicationData singlton;
		return &singlton;
	}

	/**
	* @brief アプリケーションの名前を設定する
	*/
	void SetApplicationName(LPCTSTR appName);

	/**
	* @brief ウィンドウハンドルの設定する
	*/
	void SetWindowHandle(HWND hWnd);

	/**
	* @brief ウィンドウのサイズを設定する
	* @param width  ： 横幅
	* @param height ： 縦幅
	*/
	void SetWindowSize(const DWORD width, const DWORD height);

	/**
	* @brief ウィンドウモードを設定する
	*/
	void SetWindowed(bool windowed);

	/**
	* @brief アプリケーションの名前を取得する
	*/
	LPCTSTR GetApplicationName();

	/**
	* @brief ウィンドウハンドルの取得する
	*/
	HWND GetWindowHandle();

	/**
	* @brief ウィンドウの横幅を取得する
	*/
	DWORD GetWindowWidth();

	/**
	* @brief ウィンドウの縦幅を取得する
	*/
	DWORD GetWindowHeight();

	/**
	* @brief ウィンドウモードを取得する
	*/
	bool GetWindowed();

private:
	/// ウィンドウハンドル
	HWND m_hWnd;

	/// アプリケーションの名前
	LPCTSTR m_applicationName;

	/// ウィンドウの横幅
	DWORD m_Width;

	/// ウィンドウの縦幅
	DWORD m_Height;

	/// ウィンドウモード
	bool m_windowed;
};

#endif