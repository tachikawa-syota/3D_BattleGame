#include "ScoreManager.h"
#include <functional>
#include <algorithm>

/**
* @brief コンストラクタ
*/
ScoreManager::ScoreManager(int size)
{
	// サイズ分の配列を生成
	for (int i = 0; i < size; i++)
	{
		m_score.emplace_back(0);
		m_killCount.emplace_back(0);
		m_deadCount.emplace_back(0);
	}
}

/**
* @brief デストラクタ
*/
ScoreManager::~ScoreManager()
{

}

/**
* @brief スコアを増やす
* @param プレイヤー番号
*/
void ScoreManager::AddScore(int index)
{
	// スコア加算
	m_score[index]++;
	// 撃破数も加算
	m_killCount[index]++;
}

/**
* @brief スコアを減らす
* @param プレイヤー番号
*/
void ScoreManager::SubScore(int index)
{
	// スコアが"1"以上なら減算
	m_score[index]--;
	// 死亡数を加算
	m_deadCount[index]++;
}

/**
* @brief スコアを取得する
*/
int ScoreManager::GetScore(int index) const
{
	return m_score[index];
}

/**
* @brief 撃破数を取得する
*/
int ScoreManager::GetKillCount(int index) const
{
	return m_killCount[index];
}

/**
* @brief 死亡数を取得する
*/
int ScoreManager::GetDeadCount(int index) const
{
	return m_deadCount[index];
}