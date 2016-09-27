/**
* @file   FrameWork.h
* @brief  フレームワーク
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
* @brief フレームワーククラス
*/
class FrameWork
{
public:
	/**
	* @brief コンストラクタ
	*/
	FrameWork();
	
	/**
	* @brief デストラクタ
	*/
	~FrameWork();

	/**
	* @brief 作成
	* @param インスタンス
	* @return true - 成功：false - 失敗
	*/
	bool Create(HINSTANCE hInst);

	/**
	* @brief 初期化
	*/
	HRESULT Initialize();

	/**
	* @brief 更新
	*/
	void Update();

	/**
	* @brief 描画
	*/
	void Render();

	/**
	* @brief 更新と描画
	*/
	void Run();

private:
	/// アプリケーションシステム
	unique_ptr<WindowsApplication> m_pSys;

	/// D3Dシステム
	unique_ptr<Direct3DSystem> m_pD3d;

	/// シーン
	ISequencePtr m_nowScene;

	/// フォント
	unique_ptr<Font> m_font;

	/// シングルトンオーナー
	unique_ptr<SingletonOwner> m_singletonOwner;

	/// FPSカウンター
	unique_ptr<FPSCounter> m_fpsCounter;

	/**
	* @brief 画面クリア
	*/
	BOOL HCls();

	/**
	* @brief フリッピング
	*/
	BOOL Flip();

	/**
	* @brief FPSの表示
	*/
	VOID RenderFPS();
};



#endif