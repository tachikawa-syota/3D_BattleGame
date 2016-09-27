/**
* @file   ScoreManager.h
* @brief  ���j�����Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___SCOREMANAGER_H
#define ___SCOREMANAGER_H

#include "..\Common\CommonLib.h"

/**
* @brief �X�R�A���Ǘ�����N���X(���j���Ǝ��S�����Ǘ�)
*/
class ScoreManager
{
public:
	/**
	* @brief �R���X�g���N�^
	* @param �v���C���[�̑���
	*/
	ScoreManager(int size);
	
	/**
	* @brief �f�X�g���N�^
	*/
	~ScoreManager();

	/**
	* @brief �X�R�A�𑝂₷
	* @param �v���C���[�ԍ�
	*/
	void AddScore(int index);

	/**
	* @brief �X�R�A�����炷
	* @param �v���C���[�ԍ�
	*/
	void SubScore(int index);

	/**
	* @brief �X�R�A���擾����
	*/
	int GetScore(int index) const;

	/**
	* @brief ���j�����擾����
	*/
	int GetKillCount(int index) const;

	/**
	* @brief ���S�����擾����
	*/
	int GetDeadCount(int index) const;

private:
	/// �X�R�A
	vector<int> m_score;

	/// ���j���J�E���^�[
	vector<int> m_killCount;

	/// ���S���J�E���^�[
	vector<int> m_deadCount;
};

// �X�R�A�}�l�[�W���[�̃X�}�[�g�|�C���^
using ScoreManagerPtr = shared_ptr<ScoreManager>;

#endif