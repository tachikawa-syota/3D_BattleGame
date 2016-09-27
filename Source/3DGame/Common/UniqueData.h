/**
* @file   UniqueData.h
* @brief  ��ӂȃf�[�^���Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___UNIQUEDATA_H
#define ___UNIQUEDATA_H

#include "CommonLib.h"

/**
* @struct RESULT_DATA
* @brief ���U���g�Ŏg���X�R�A�f�[�^
*/
struct RESULT_DATA
{
	/// �X�R�A
	int score;

	/// ���j��
	int kill;

	/// ���S��
	int dead;
};

/**
* @enum BattleMode
* @brief �o�g�����[�h
*/
enum class BattleMode : BYTE
{
	// �^�C����
	Time,

	// �X�g�b�N��
	Stock
};

/**
* @brief ��ӂȃf�[�^�N���X
*/
class UniqueData
{
public:
	/**
	* @brief �C���X�^���X�̎擾
	*/
	static UniqueData* GetInstance(){
		static UniqueData singleton;
		return &singleton;
	}

	/**
	* @brief �ΐ�l����ݒ�
	*/
	void SetBattleNum(int num);

	/**
	* @brief �ΐ�l�����擾
	*/
	int GetBattleNum() const;

	/**
	* @brief �o�g�����[�h�̐ݒ�
	*/
	void SetBattleMode(BattleMode mode);

	/**
	* @brief �R���s���[�^�[���x���̎擾
	*/
	vector<int> GetComputerLevel() const;

	/**
	* @brief �R���s���[�^�[���x���̐ݒ�
	*/
	void SetComputerLevel(vector<int> level);

	/**
	* @brief �R���s���[�^�[�̐���ݒ�
	*/
	void SetComputerCount(int count);

	/**
	* @brief �R���s���[�^�[�̐����擾
	*/
	int GetComputerCount() const;

	/**
	* @brief �o�g�����[�h���擾
	*/
	BattleMode GetBattleMode() const;

	/**
	* @brief ���ʂ̍폜(�ϒ��z��)
	*/
	void RankClear();

	/**
	* @brief ���ʂ̃Z�b�g(������)
	* @param �v���C���[�ԍ�
	*/
	void SetRankUnder(int index);

	/**
	* @brief ���ʂ̃Z�b�g(�ォ��)
	* @param �v���C���[�ԍ�
	*/
	void SetRankOver(int index);

	/**
	* @brief ���ʂ̎擾
	* @param �v���C���[�ԍ�
	* @return ����
	*/
	int GetRank(int index) const;

	/**
	* @brief ���U���g�f�[�^�̓o�^
	*/
	void SetResultData(int score, int kill, int dead, int index);

	/**
	* @brief ���U���g�f�[�^�̎擾
	*/
	const RESULT_DATA& GetResultData(int index) const;

	/**
	* @brief �X�y�V�����o�g���t���O�̐ݒ�
	*/
	void SetSpecialBattleFlag(bool bFlag);

	/**
	* @brief �X�y�V�����o�g���t���O�̎擾
	*/
	const bool GetSpecialBattleFlag() const;

private:
	/// �ΐ�l��
	int m_battleNum;

	/// �o�g�����[�h
	BattleMode m_mode;

	/// ����
	vector<int> m_rank;

	/// ���U���g�f�[�^
	vector<RESULT_DATA> m_resultData;

	/// �R���s���[�^�[�̐�
	int m_computerCount;

	/// �R���s���[�^�[�̃��x��
	vector<int> m_computerLevel;

	/// �X�y�V�����o�g��
	bool m_isSpecialBattle;
};

#endif