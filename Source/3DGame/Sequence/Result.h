/**
* @file   Result.h
* @brief  リザルト画面を管理する
* @author Syota Tachikawa
*/

#ifndef ___RESULT_H
#define ___RESULT_H

#include "ISequence.h"
#include "SelectCursor.h"
#include "..\Common\UniqueData.h"

/**
* @brief リザルト画面を管理するクラス
*/
class Result : public ISequence
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

	/// 入力デバイス
	InputPtr m_dev;

	/// 順位
	vector<int> m_rank;

	/// リザルトデータ
	vector<RESULT_DATA> m_data;

	/// Winner描画位置
	Vector2 m_winnerPos;

	/// 光の角度
	float m_lightAngle;

	/// 数字テクスチャ
	int m_numberTex[NUMBER_TEXTURE_MAX];

	/// プレイヤーテキストテクスチャ
	int m_playerTextTex[PLAYER_TEXTURE_MAX];

	/**
	* @brief リザルトの更新
	*/
	bool UpdateResultWork();
};

#endif