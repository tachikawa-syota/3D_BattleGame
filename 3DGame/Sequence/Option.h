/**
* @file   Option.h
* @brief  ゲームのルールを設定する
* @author Syota Tachikawa
*/

#ifndef ___OPTION_H
#define ___OPTION_H

#include "ISequence.h"
#include "SelectCursor.h"
#include "..\DirectX\Direct3DSystem.h"
#include "..\DirectX\Direct3DData.h"
#include "..\Common\UniqueData.h"

/**
* @enum SettingNumber
* @brief セッティング番号
*/
enum SettingNumber
{
	/// 対戦人数
	SettingBattleNum = 1,

	/// バトルモード
	SettingBattleMode,

	/// スペシャルバトル
	SettingSpecialBattle
};

/**
* @brief ゲームのルールを管理するオプションクラス
*/
class Option : public ISequence
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

	/// ファイルデータ
	FileData* m_fileData;

	/// カーソル
	SelectCursorPtr m_cursor;

	/// バトルモード
	BattleMode m_battleMode;

	/// 数字テクスチャ
	int m_numTex[NUMBER_TEXTURE_MAX];

	/// 対戦人数
	int m_battleNum;
	
	/// スペシャルバトルフラグ
	bool m_isSpecialBattle;

	/// 表示フラグ
	bool m_isLeft;
	bool m_isRight;

	/// カーソルのY座標
	float m_cursorY;

	/**
	* @brief 矢印アイコンの表示フラグを更新
	*/
	void UpdateDisplayArrowIcon();
};

#endif