/**
* @file   ScoreManager.h
* @brief  撃破数を管理する
* @author Syota Tachikawa
*/

#ifndef ___SCOREMANAGER_H
#define ___SCOREMANAGER_H

#include "..\Common\CommonLib.h"

/**
* @brief スコアを管理するクラス(撃破数と死亡数を管理)
*/
class ScoreManager
{
public:
	/**
	* @brief コンストラクタ
	* @param プレイヤーの総数
	*/
	ScoreManager(int size);
	
	/**
	* @brief デストラクタ
	*/
	~ScoreManager();

	/**
	* @brief スコアを増やす
	* @param プレイヤー番号
	*/
	void AddScore(int index);

	/**
	* @brief スコアを減らす
	* @param プレイヤー番号
	*/
	void SubScore(int index);

	/**
	* @brief スコアを取得する
	*/
	int GetScore(int index) const;

	/**
	* @brief 撃破数を取得する
	*/
	int GetKillCount(int index) const;

	/**
	* @brief 死亡数を取得する
	*/
	int GetDeadCount(int index) const;

private:
	/// スコア
	vector<int> m_score;

	/// 撃破数カウンター
	vector<int> m_killCount;

	/// 死亡数カウンター
	vector<int> m_deadCount;
};

// スコアマネージャーのスマートポインタ
using ScoreManagerPtr = shared_ptr<ScoreManager>;

#endif