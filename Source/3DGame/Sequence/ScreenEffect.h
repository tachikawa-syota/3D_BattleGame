/**
* @file   ScreenEffect.h
* @brief  スクリーンエフェクト処理を管理する
* @author Syota Tachikawa
*/

#ifndef ___SCREANEFFECT_H
#define ___SCREANEFFECT_H

#include "..\DirectX\TextureManager.h"

/**
* @enum Fade
* @brief フェード
*/
enum class Fade
{
	/// フェードがかかっていない
	None,

	/// フェードイン
	In,

	/// フェードアウト
	Out
};

/**
* @enum FadeColor
* @brief フェード色
*/
enum class FadeColor
{
	/// 黒
	Black,

	/// 白
	White
};

/**
* @brief スクリーンエフェクトクラス
*/
class ScreenEffect
{
public:
	// インスタンス取得
	static ScreenEffect* GetInstance(){
		static ScreenEffect singleton;
		return &singleton;
	}

	/**
	* @brief 初期化
	*/
	void Initialize();

	/**
	* @brief フェードの設定
	*/
	bool SetFade(Fade type, FadeColor color, int frame);

	/**
	* @brief フェード実行中かをチェック
	*/
	bool isPlayFade();

	/**
	* @brief 更新
	*/
	void Update();

	/**
	* @brief 描画
	*/
	void Draw();

private:
	/// 加算値
	int m_add;

	/// 色
	FadeColor m_color;

	/// フレーム
	int m_frame;

	/// カウント
	int m_fade_cnt;

	/// フェード状態
	Fade m_state;

};

#endif