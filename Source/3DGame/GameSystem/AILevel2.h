/**
* @file   AILevel2.h
* @brief  ���x��2��AI�̋������Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___AILEVEL2_H
#define ___AILEVEL2_H

#include "IComputer.h"

/**
* @brief ���x��2��AI�̋������Ǘ�����N���X
*/
class AILevel2 : public IComputer
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	AILevel2();

	/**
	* @brief �f�X�g���N�^
	*/
	~AILevel2();

	/**
	* @brief �v�l����
	* @param player - �v���C���[�N���X
	* @param stage - �X�e�[�W�N���X
	* @param damage - �_���[�W�f�[�^�N���X
	* @param index - �v���C���[�ԍ�
	*/
	DeviceState& Think(vector<IPlayerPtr>& player, list<IItemPtr>& item, IStagePtr& stage, DamageData damage, int index);

	/**
	* @brief �V�����s�����v�l����
	* @param player - �v���C���[�N���X
	* @param stage - �X�e�[�W�N���X
	* @param damage - �_���[�W�f�[�^�N���X
	* @param index - �v���C���[�ԍ�
	*/
	DeviceState& ThinkNewAction(vector<IPlayerPtr>& player, IStagePtr& stage, DamageData damage, int index);

	/**
	* @brief �X�e�[�W�ɕ��A����s��������
	* @param player - �v���C���[�N���X
	* @param stage - �X�e�[�W�N���X
	* @param index - �v���C���[�ԍ�
	*/
	DeviceState& StageReturnAction(vector<IPlayerPtr>& player, IStagePtr& stage, int index);

	/**
	* @brief �O��̍s�����p������
	*/
	DeviceState& ThinkContinue();

private:
	/// �s��
	Action m_action;

	/// ���
	DeviceState m_deviceState;

	/// �s���t���[��
	unsigned int m_actionFrame;

	/// ���̍s�����l����t���[��
	unsigned int m_nextActionFrame;

	/// ���A�t���[��
	unsigned int returnFrame;

	/**
	* @brief �s����ݒ肷��
	* @param action - �s��
	* @param flag - ���̓t���O
	*/
	void SetAction(Action action, unsigned int flag);

	/**
	* @brief �ړ���ݒ肷��
	* @param action - �s��(�ړ��n�̍s��������)
	* @param flag - ���̓t���O
	*/
	void SetMoveAction(Action action, unsigned int flag);
};

#endif