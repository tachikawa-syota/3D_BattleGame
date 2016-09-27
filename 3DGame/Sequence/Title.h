/**
* @file   Title.h
* @brief  タイトルの処理を管理する
* @author Syota Tachikawa
*/

#ifndef ___TITLE_H
#define ___TITLE_H

#include "ISequence.h"
#include "SelectCursor.h"
#include "..\DirectX\Direct3DSystem.h"
#include "..\DirectX\Direct3DData.h"

/**
* @brief ゲームメインクラス
*/
class Title : public ISequence
{
public:
	/**
	* @brief 作成
	*/
	void Create();

	/**
	* @brief 初期化
	*/
	void Initialize();

	/**
	* @brief 更新
	*/
	void Update();

	/**
	* @brief 描画
	*/
	void Render();

	/**
	* @brief 終了
	*/
	void Finalize();
	
private:
	/// グラフィックスマネージャ
	TextureManager* m_2dTex;

	/// カーソル
	SelectCursorPtr m_cursor;

	/// バトル文字の大きさ
	float m_battleFontScale;

	/// オプション文字の大きさ
	float m_optionFontScale;

	/// クレジット文字の大きさ
	float m_creditTextScale;

	/// 終了文字の大きさ
	float m_endTextScale;

	/// 拡大フラグ
	bool m_isAddScale;

	/// サブワーク
	int m_subWork;

	/// レベル選択ワーク
	int m_levelWork;

	/// コンピューター番号
	int m_index;

	/// コンピューターのレベル
	vector<int> m_computerLevel;

	/// プレイヤー文字
	int m_playerText[PLAYER_TEXTURE_MAX];

	/// 数字テクスチャ
	int m_numTex[NUMBER_TEXTURE_MAX];

	/// アルファ値
	int m_alpha;

	/// BGM停止フラグ
	bool m_isEndBGM;

	/**
	* @brief タイトルの更新
	*/
	void UpdateTitleWork();

	/**
	* @brief コンピューターのレベル選択の更新
	*/
	bool UpdateComputerLevelSelect();
};

#endif