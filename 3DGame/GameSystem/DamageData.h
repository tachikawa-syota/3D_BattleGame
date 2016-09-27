/**
* @file   DamageData.h
* @brief  �_���[�W�f�[�^���Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___DAMAGEDATA_H
#define ___DAMAGEDATA_H

#include "..\Common\CommonLib.h"

/**
* @brief �_���[�W���\����
*/
struct DAMAGE_DATA
{
	/// �_���[�W����"1"�̈�
	int damageOnesPlace;

	/// �_���[�W����"10"�̈�
	int damageTensPlace;

	/// �_���[�W����"100"�̈�
	int damageHundredsPlace;

	/// ���Z�_���[�W
	DWORD addDamage;

	/// �_���[�W��
	DWORD damageRate;
};

/**
* @brief �_���[�W�����Ǘ�����N���X
*/
class DamageData
{
public:
	/**
	* @brief ������
	* @param �v���C���[�̐�
	*/
	void Initialize(int size);

	/**
	* @brief �X�V
	*/
	void Update();

	/**
	* @brief ���Z�b�g����ԍ��̃Z�b�g
	*/
	void SetReset(bool bFlag, int index);

	/**
	* @brief ���Z�b�g
	*/
	void Reset();

	/**
	* @brief �_���[�W��"100"�̈ʎ擾
	*/
	int GetHundredId(int index) const;

	/**
	* @brief �_���[�W��"10"�̈ʎ擾
	*/
	int GetTenId(int index) const;

	/**
	* @brief �_���[�W��"1"�̈ʎ擾
	*/
	int GetOneId(int index) const;

	/**
	* @brief �_���[�W���̎擾
	*/
	DWORD GetDamageRate(int index);

	/**
	* @brief �_���[�W���̒ǉ�
	*/
	void AddDamageRate(DWORD add, int index);

private:
	/// �f�[�^�\����
	vector<DAMAGE_DATA> m_data;

	/// ���Z�b�g�t���O
	bool m_isReset;

	/// ���Z�b�g�ԍ�
	int m_resetNum;
};

#endif