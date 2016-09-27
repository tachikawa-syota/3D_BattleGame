#include "CorrectionValue.h"

enum ATTACK_TYPE
{
	// 通常攻撃
	NormalAttack,
	// スマッシュ攻撃
	SmashAttack,
	// 必殺攻撃(上)
	SpecialAttackUp,
};

/**
* @brief 初期化
*/
void CorrectionValue::Initialize()
{
	m_rockCorValue[NormalAttack] = 0.0001f;
	m_rockCorValue[SmashAttack] = 0.009f;
	m_rockCorValue[SpecialAttackUp] = 0.005f;
}

/**
* @brief 補正値を取得する
* @param type - キャラクターのクラスを入れる
* @param player - プレイヤーのポインタ
* @param index - 攻撃者の番号
*/
float CorrectionValue::GetValue(const type_info& type, vector<IPlayerPtr>& player, int index) const
{
	if (type == typeid(Rock))
	{
		switch (player[index]->GetAction())
		{
		case Action::NormalAttack:
			return m_rockCorValue[NormalAttack];
			break;

		case Action::SmashAttack:
			return m_rockCorValue[SmashAttack];
			break;

		case Action::SpecialAttackUp:
			return m_rockCorValue[SpecialAttackUp];
			break;
		}
	}

	return 0.0f;
}