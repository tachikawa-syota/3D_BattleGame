#include "ScoreManager.h"
#include <functional>
#include <algorithm>

/**
* @brief �R���X�g���N�^
*/
ScoreManager::ScoreManager(int size)
{
	// �T�C�Y���̔z��𐶐�
	for (int i = 0; i < size; i++)
	{
		m_score.emplace_back(0);
		m_killCount.emplace_back(0);
		m_deadCount.emplace_back(0);
	}
}

/**
* @brief �f�X�g���N�^
*/
ScoreManager::~ScoreManager()
{

}

/**
* @brief �X�R�A�𑝂₷
* @param �v���C���[�ԍ�
*/
void ScoreManager::AddScore(int index)
{
	// �X�R�A���Z
	m_score[index]++;
	// ���j�������Z
	m_killCount[index]++;
}

/**
* @brief �X�R�A�����炷
* @param �v���C���[�ԍ�
*/
void ScoreManager::SubScore(int index)
{
	// �X�R�A��"1"�ȏ�Ȃ猸�Z
	m_score[index]--;
	// ���S�������Z
	m_deadCount[index]++;
}

/**
* @brief �X�R�A���擾����
*/
int ScoreManager::GetScore(int index) const
{
	return m_score[index];
}

/**
* @brief ���j�����擾����
*/
int ScoreManager::GetKillCount(int index) const
{
	return m_killCount[index];
}

/**
* @brief ���S�����擾����
*/
int ScoreManager::GetDeadCount(int index) const
{
	return m_deadCount[index];
}