#include "DamageData.h"
#include "..\Common\CommonParam.h"

/**
* @brief 初期化
*/
void DamageData::Initialize(int size)
{
	// データサイズ確保
	m_data.resize(size);

	// リセットフラグを初期化する
	m_isReset = false;
	m_resetNum = -1;

	for (int i = 0; i != m_data.size(); i++)
	{
		m_data[i].damageRate = 0;
		m_data[i].addDamage = 0;
		m_data[i].damageOnesPlace = 0;
		m_data[i].damageTensPlace = 0;
		m_data[i].damageHundredsPlace = 0;
	}
}

/**
* @brief 更新
*/
void DamageData::Update()
{
	// リセットフラグが立ったら初期化(プレイヤーが死んだ時にフラグが立つ)
	if (m_isReset){
		Reset();
	}

	// データ分ループさせる
	for (int i = 0; i != m_data.size(); i++)
	{
		// ダメージを喰らわなかったら処理をしない
		if (m_data[i].addDamage == 0) continue;

		for (unsigned int j = 0; j < m_data[i].addDamage; j++)
		{
			m_data[i].damageOnesPlace++;

			// 1の位が9を超えたら10の位を足す
			if (m_data[i].damageOnesPlace > 9)
			{
				m_data[i].damageOnesPlace = 0;
				m_data[i].damageTensPlace++;
			}
			// 10の位が9を超えたら100の位を足す
			if (m_data[i].damageTensPlace > 9)
			{
				m_data[i].damageTensPlace = 0;
				m_data[i].damageHundredsPlace++;
			}
		}

		// ダメージ率を加算
		m_data[i].damageRate += m_data[i].addDamage;
		// 追加するダメージ量を初期化
		m_data[i].addDamage = 0;
	}
}


/**
* @brief ダメージの"100"の位取得
*/
int DamageData::GetHundredId(int index) const
{
	return m_data[index].damageHundredsPlace;
}

/**
* @brief ダメージの"10"の位取得
*/
int DamageData::GetTenId(int index) const
{
	return m_data[index].damageTensPlace;
}

/**
* @brief ダメージの"1"の位取得
*/
int DamageData::GetOneId(int index) const
{
	return m_data[index].damageOnesPlace;
}

/**
* @brief リセット
*/
void DamageData::SetReset(bool bFlag, int index)
{
	m_isReset = bFlag;

	m_resetNum = index;
}

/**
* @brief リセット
*/
void DamageData::Reset()
{
	// データの初期化
	m_data[m_resetNum].damageRate = 0;
	m_data[m_resetNum].addDamage = 0;
	m_data[m_resetNum].damageOnesPlace = 0;
	m_data[m_resetNum].damageTensPlace = 0;
	m_data[m_resetNum].damageHundredsPlace = 0;

	// リセットフラグを切る
	m_isReset = false;
	m_resetNum = -1;
}

/**
* @brief ダメージ率の取得
*/
DWORD DamageData::GetDamageRate(int index)
{
	return m_data[index].damageRate;
}

/**
* @brief ダメージ率の追加
*/
void DamageData::AddDamageRate(DWORD add, int index)
{
	m_data[index].addDamage = add;
}