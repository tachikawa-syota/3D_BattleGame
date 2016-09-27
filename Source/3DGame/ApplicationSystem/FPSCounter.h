/**
* @file   FPSCounter.h
* @brief  FPSを計測する
* @author Syota Tachikawa
*/

#ifndef ___FPSCOUNTER_H
#define ___FPSCOUNTER_H

#include "..\Common\CommonLib.h"

/**
* @enum FPSMode
* @brief FPSモード
*/
enum class FPSMode : BYTE
{
	/// 固定６０フレーム
	FPS_60,

	/// 固定３０フレーム
	FPS_30,

	/// 可変フレーム
	FPS_FLEX
};

/**
* @brief FPSを計測するクラス
*/
class FPSCounter
{
public:
	/**
	* @brief コンストラクタ
	* @param FPSモードの指定
	*/
	FPSCounter(FPSMode fpsMode);

	/**
	* @brief デストラクタ
	*/
	~FPSCounter();

	/**
	* @brief 更新
	*/
	BOOL Update();

	/**
	* @brief 描画フラグの取得
	*/
	BOOL IsRender();

	/**
	* @brief フレームレートの取得
	*/
	DWORD GetFPS();

	/**
	* @brief 描画フレームレートの取得
	*/
	DWORD GetRenderFPS();

private:
	/// 描画フラグ
	BOOL bRender;

	/// 起動からのフレーム数
	DWORD m_dwGameFrame;

	/// フレーム制御モード
	FPSMode	m_FPSMode;
	DWORD m_dwFrameTime;

	/// 処理フレームレート
	DWORD m_dwFPS;

	/// 描画フレームレート
	DWORD m_dwRenderFPS;

	/// 処理カウンタ
	DWORD m_dwCurFrame;

	/// 描画カウンタ
	DWORD m_dwRCurFrame;
};

#endif