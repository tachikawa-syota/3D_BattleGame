/**
* @file   DamageData.h
* @brief  ダメージデータを管理する
* @author Syota Tachikawa
*/

#ifndef ___DAMAGEDATA_H
#define ___DAMAGEDATA_H

#include "..\Common\CommonLib.h"

/**
* @brief ダメージ率構造体
*/
struct DAMAGE_DATA
{
	/// ダメージ率の"1"の位
	int damageOnesPlace;

	/// ダメージ率の"10"の位
	int damageTensPlace;

	/// ダメージ率の"100"の位
	int damageHundredsPlace;

	/// 加算ダメージ
	DWORD addDamage;

	/// ダメージ率
	DWORD damageRate;
};

/**
* @brief ダメージ情報を管理するクラス
*/
class DamageData
{
public:
	/**
	* @brief 初期化
	* @param プレイヤーの数
	*/
	void Initialize(int size);

	/**
	* @brief 更新
	*/
	void Update();

	/**
	* @brief リセットする番号のセット
	*/
	void SetReset(bool bFlag, int index);

	/**
	* @brief リセット
	*/
	void Reset();

	/**
	* @brief ダメージの"100"の位取得
	*/
	int GetHundredId(int index) const;

	/**
	* @brief ダメージの"10"の位取得
	*/
	int GetTenId(int index) const;

	/**
	* @brief ダメージの"1"の位取得
	*/
	int GetOneId(int index) const;

	/**
	* @brief ダメージ率の取得
	*/
	DWORD GetDamageRate(int index);

	/**
	* @brief ダメージ率の追加
	*/
	void AddDamageRate(DWORD add, int index);

private:
	/// データ構造体
	vector<DAMAGE_DATA> m_data;

	/// リセットフラグ
	bool m_isReset;

	/// リセット番号
	int m_resetNum;
};

#endif