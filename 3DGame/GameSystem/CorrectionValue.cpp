#include "CorrectionValue.h"

enum ATTACK_TYPE
{
	// �ʏ�U��
	NormalAttack,
	// �X�}�b�V���U��
	SmashAttack,
	// �K�E�U��(��)
	SpecialAttackUp,
};

/**
* @brief ������
*/
void CorrectionValue::Initialize()
{
	m_rockCorValue[NormalAttack] = 0.0001f;
	m_rockCorValue[SmashAttack] = 0.009f;
	m_rockCorValue[SpecialAttackUp] = 0.005f;
}

/**
* @brief �␳�l���擾����
* @param type - �L�����N�^�[�̃N���X������
* @param player - �v���C���[�̃|�C���^
* @param index - �U���҂̔ԍ�
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