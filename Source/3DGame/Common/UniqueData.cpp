#include "UniqueData.h"

/**
* @brief 対戦人数を設定
*/
void UniqueData::SetBattleNum(int num)
{
	m_battleNum = num;
}

/**
* @brief 対戦人数を取得
*/
int UniqueData::GetBattleNum() const
{
	return m_battleNum;
}

/**
* @brief コンピューターレベルの取得
*/
vector<int> UniqueData::GetComputerLevel() const
{
	return m_computerLevel;
}

/**
* @brief コンピューターレベルの設定
*/
void UniqueData::SetComputerLevel(vector<int> level)
{
	// 配列確保
	m_computerLevel.resize(m_computerCount);
	// セット
	m_computerLevel = level;
}

/**
* @brief コンピューターの数を設定
*/
void UniqueData::SetComputerCount(int count)
{
	m_computerCount = count;
}

/**
* @brief コンピューターの数を取得
*/
int UniqueData::GetComputerCount() const
{
	return m_computerCount;
}

/**
* @brief バトルモードの設定
*/
void UniqueData::SetBattleMode(BattleMode mode)
{
	m_mode = mode;
}

/**
* @brief バトルモードを取得
*/
BattleMode UniqueData::GetBattleMode() const
{
	return m_mode;
}

/**
* @brief リザルトランクデータのクリア(可変長配列)
*/
void UniqueData::RankClear()
{
	m_rank.clear();
	m_resultData.resize(m_battleNum);
}

/**
* @brief 順位のセット(下から)
* @param プレイヤー番号
*/
void UniqueData::SetRankUnder(int index)
{
	m_rank.emplace_back(index);
}

/**
* @brief 順位のセット(上から)
* @param プレイヤー番号
*/
void UniqueData::SetRankOver(int index)
{
	auto it = m_rank.begin();
	m_rank.insert(it, index);
}

/**
* @brief 順位の取得
* @param プレイヤー番号
* @return 順位
*/
int UniqueData::GetRank(int index) const
{
	return m_rank[index];
}

/**
* @brief リザルトデータの登録
*/
void UniqueData::SetResultData(int score, int kill, int dead, int index)
{
	m_resultData[index].score = score;
	m_resultData[index].kill = kill;
	m_resultData[index].dead = dead;
}

/**
* @brief リザルトデータの取得
*/
const RESULT_DATA& UniqueData::GetResultData(int index) const
{
	return m_resultData[index];
}

/**
* @brief スペシャルバトルフラグの設定
*/
void UniqueData::SetSpecialBattleFlag(bool bFlag)
{
	m_isSpecialBattle = bFlag;
}

/**
* @brief スペシャルバトルフラグの取得
*/
const bool UniqueData::GetSpecialBattleFlag() const
{
	return m_isSpecialBattle;
}