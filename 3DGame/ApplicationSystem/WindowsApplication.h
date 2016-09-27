/**
* @file   WindowsApplication.h
* @brief  アプリケーションシステムの管理
* @author Syota Tachikawa
* @date   11/27
*/

#ifndef ___WINDOWSAPPLICATION_H
#define ___WINDOWSAPPLICATION_H

#include "..\Common\CommonLib.h"

/// アプリケーションの名前
#define APPLICATION_NAME "バトルゲイム"

/**
* @brief ウィンドウズアプリケーションクラス
*/
class WindowsApplication
{
public:
	/**
	* @brief コンストラクタ
	*/
	WindowsApplication();
	
	/**
	* @brief デストラクタ
	*/
	~WindowsApplication();

	/**
	* @brief 作成
	*/
	HRESULT Create(HINSTANCE hInst);

	/**
	* @brief ウィンドウからのメッセージ処理
	*/
	BOOL ProcessEvent();

	/**
	* @brief ウィンドウの初期化(作成)
	*/
	BOOL WindowInitialize(HINSTANCE hInst);

private:
	/// メッセージ
	MSG m_msg;

	/// ウィンドウハンドル
	HWND m_hWnd;

	/**
	* @brief ウィンドウクラスへ登録
	* @param hInst   ： インスタンス
	* @param WndProc ： ウィンドウプロシージャ
	*/
	ATOM RegisterWindowClass(HINSTANCE hInst, WNDPROC WndProc);

	/**
	* @brief ウィンドウプロシージャ
	*/
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	/**
	* @brief デバッグ用ウィンドウを開く
	*/
	static void OpenDebugWindow();

	/**
	* @brief デバッグ用ウィンドウを閉じる
	*/
	void CloseDebugWindow();
};

#endif