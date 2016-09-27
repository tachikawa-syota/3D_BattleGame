#include "DamageData.h"
#include "..\Common\CommonParam.h"

/**
* @brief ������
*/
void DamageData::Initialize(int size)
{
	// �f�[�^�T�C�Y�m��
	m_data.resize(size);

	// ���Z�b�g�t���O������������
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
* @brief �X�V
*/
void DamageData::Update()
{
	// ���Z�b�g�t���O���������珉����(�v���C���[�����񂾎��Ƀt���O������)
	if (m_isReset){
		Reset();
	}

	// �f�[�^�����[�v������
	for (int i = 0; i != m_data.size(); i++)
	{
		// �_���[�W������Ȃ������珈�������Ȃ�
		if (m_data[i].addDamage == 0) continue;

		for (unsigned int j = 0; j < m_data[i].addDamage; j++)
		{
			m_data[i].damageOnesPlace++;

			// 1�̈ʂ�9�𒴂�����10�̈ʂ𑫂�
			if (m_data[i].damageOnesPlace > 9)
			{
				m_data[i].damageOnesPlace = 0;
				m_data[i].damageTensPlace++;
			}
			// 10�̈ʂ�9�𒴂�����100�̈ʂ𑫂�
			if (m_data[i].damageTensPlace > 9)
			{
				m_data[i].damageTensPlace = 0;
				m_data[i].damageHundredsPlace++;
			}
		}

		// �_���[�W�������Z
		m_data[i].damageRate += m_data[i].addDamage;
		// �ǉ�����_���[�W�ʂ�������
		m_data[i].addDamage = 0;
	}
}


/**
* @brief �_���[�W��"100"�̈ʎ擾
*/
int DamageData::GetHundredId(int index) const
{
	return m_data[index].damageHundredsPlace;
}

/**
* @brief �_���[�W��"10"�̈ʎ擾
*/
int DamageData::GetTenId(int index) const
{
	return m_data[index].damageTensPlace;
}

/**
* @brief �_���[�W��"1"�̈ʎ擾
*/
int DamageData::GetOneId(int index) const
{
	return m_data[index].damageOnesPlace;
}

/**
* @brief ���Z�b�g
*/
void DamageData::SetReset(bool bFlag, int index)
{
	m_isReset = bFlag;

	m_resetNum = index;
}

/**
* @brief ���Z�b�g
*/
void DamageData::Reset()
{
	// �f�[�^�̏�����
	m_data[m_resetNum].damageRate = 0;
	m_data[m_resetNum].addDamage = 0;
	m_data[m_resetNum].damageOnesPlace = 0;
	m_data[m_resetNum].damageTensPlace = 0;
	m_data[m_resetNum].damageHundredsPlace = 0;

	// ���Z�b�g�t���O��؂�
	m_isReset = false;
	m_resetNum = -1;
}

/**
* @brief �_���[�W���̎擾
*/
DWORD DamageData::GetDamageRate(int index)
{
	return m_data[index].damageRate;
}

/**
* @brief �_���[�W���̒ǉ�
*/
void DamageData::AddDamageRate(DWORD add, int index)
{
	m_data[index].addDamage = add;
}