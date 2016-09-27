/**
* @file   UniqueData.h
* @brief  一意なデータを管理する
* @author Syota Tachikawa
*/

#ifndef ___UNIQUEDATA_H
#define ___UNIQUEDATA_H

#include "CommonLib.h"

/**
* @struct RESULT_DATA
* @brief リザルトで使うスコアデータ
*/
struct RESULT_DATA
{
	/// スコア
	int score;

	/// 撃破数
	int kill;

	/// 死亡数
	int dead;
};

/**
* @enum BattleMode
* @brief バトルモード
*/
enum class BattleMode : BYTE
{
	// タイム制
	Time,

	// ストック制
	Stock
};

/**
* @brief 一意なデータクラス
*/
class UniqueData
{
public:
	/**
	* @brief インスタンスの取得
	*/
	static UniqueData* GetInstance(){
		static UniqueData singleton;
		return &singleton;
	}

	/**
	* @brief 対戦人数を設定
	*/
	void SetBattleNum(int num);

	/**
	* @brief 対戦人数を取得
	*/
	int GetBattleNum() const;

	/**
	* @brief バトルモードの設定
	*/
	void SetBattleMode(BattleMode mode);

	/**
	* @brief コンピューターレベルの取得
	*/
	vector<int> GetComputerLevel() const;

	/**
	* @brief コンピューターレベルの設定
	*/
	void SetComputerLevel(vector<int> level);

	/**
	* @brief コンピューターの数を設定
	*/
	void SetComputerCount(int count);

	/**
	* @brief コンピューターの数を取得
	*/
	int GetComputerCount() const;

	/**
	* @brief バトルモードを取得
	*/
	BattleMode GetBattleMode() const;

	/**
	* @brief 順位の削除(可変長配列)
	*/
	void RankClear();

	/**
	* @brief 順位のセット(下から)
	* @param プレイヤー番号
	*/
	void SetRankUnder(int index);

	/**
	* @brief 順位のセット(上から)
	* @param プレイヤー番号
	*/
	void SetRankOver(int index);

	/**
	* @brief 順位の取得
	* @param プレイヤー番号
	* @return 順位
	*/
	int GetRank(int index) const;

	/**
	* @brief リザルトデータの登録
	*/
	void SetResultData(int score, int kill, int dead, int index);

	/**
	* @brief リザルトデータの取得
	*/
	const RESULT_DATA& GetResultData(int index) const;

	/**
	* @brief スペシャルバトルフラグの設定
	*/
	void SetSpecialBattleFlag(bool bFlag);

	/**
	* @brief スペシャルバトルフラグの取得
	*/
	const bool GetSpecialBattleFlag() const;

private:
	/// 対戦人数
	int m_battleNum;

	/// バトルモード
	BattleMode m_mode;

	/// 順位
	vector<int> m_rank;

	/// リザルトデータ
	vector<RESULT_DATA> m_resultData;

	/// コンピューターの数
	int m_computerCount;

	/// コンピューターのレベル
	vector<int> m_computerLevel;

	/// スペシャルバトル
	bool m_isSpecialBattle;
};

#endif