#include "UniqueData.h"

/**
* @brief �ΐ�l����ݒ�
*/
void UniqueData::SetBattleNum(int num)
{
	m_battleNum = num;
}

/**
* @brief �ΐ�l�����擾
*/
int UniqueData::GetBattleNum() const
{
	return m_battleNum;
}

/**
* @brief �R���s���[�^�[���x���̎擾
*/
vector<int> UniqueData::GetComputerLevel() const
{
	return m_computerLevel;
}

/**
* @brief �R���s���[�^�[���x���̐ݒ�
*/
void UniqueData::SetComputerLevel(vector<int> level)
{
	// �z��m��
	m_computerLevel.resize(m_computerCount);
	// �Z�b�g
	m_computerLevel = level;
}

/**
* @brief �R���s���[�^�[�̐���ݒ�
*/
void UniqueData::SetComputerCount(int count)
{
	m_computerCount = count;
}

/**
* @brief �R���s���[�^�[�̐����擾
*/
int UniqueData::GetComputerCount() const
{
	return m_computerCount;
}

/**
* @brief �o�g�����[�h�̐ݒ�
*/
void UniqueData::SetBattleMode(BattleMode mode)
{
	m_mode = mode;
}

/**
* @brief �o�g�����[�h���擾
*/
BattleMode UniqueData::GetBattleMode() const
{
	return m_mode;
}

/**
* @brief ���U���g�����N�f�[�^�̃N���A(�ϒ��z��)
*/
void UniqueData::RankClear()
{
	m_rank.clear();
	m_resultData.resize(m_battleNum);
}

/**
* @brief ���ʂ̃Z�b�g(������)
* @param �v���C���[�ԍ�
*/
void UniqueData::SetRankUnder(int index)
{
	m_rank.emplace_back(index);
}

/**
* @brief ���ʂ̃Z�b�g(�ォ��)
* @param �v���C���[�ԍ�
*/
void UniqueData::SetRankOver(int index)
{
	auto it = m_rank.begin();
	m_rank.insert(it, index);
}

/**
* @brief ���ʂ̎擾
* @param �v���C���[�ԍ�
* @return ����
*/
int UniqueData::GetRank(int index) const
{
	return m_rank[index];
}

/**
* @brief ���U���g�f�[�^�̓o�^
*/
void UniqueData::SetResultData(int score, int kill, int dead, int index)
{
	m_resultData[index].score = score;
	m_resultData[index].kill = kill;
	m_resultData[index].dead = dead;
}

/**
* @brief ���U���g�f�[�^�̎擾
*/
const RESULT_DATA& UniqueData::GetResultData(int index) const
{
	return m_resultData[index];
}

/**
* @brief �X�y�V�����o�g���t���O�̐ݒ�
*/
void UniqueData::SetSpecialBattleFlag(bool bFlag)
{
	m_isSpecialBattle = bFlag;
}

/**
* @brief �X�y�V�����o�g���t���O�̎擾
*/
const bool UniqueData::GetSpecialBattleFlag() const
{
	return m_isSpecialBattle;
}