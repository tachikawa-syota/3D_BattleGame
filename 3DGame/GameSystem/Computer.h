/**
* @file   Computer.h
* @brief  AI�̊Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___COMPUTER_H
#define ___COMPUTER_H

#include "..\Character\IPlayer.h"
#include "IComputer.h"
#include "AILevelFactory.h"

// �O���錾
class DamageData;

/**
* @brief �R���s���[�^�[(�l�H�m�\)�N���X
*/
class Computer
{
public:
	/**
	* @brief ������
	* @param ���x��
	*/
	void Initialize(AILevel level);

	/**
	* @brief �v�l����
	* @param player - �v���C���[�N���X
	* @param stage - �X�e�[�W�N���X
	* @param damage - �_���[�W�f�[�^�N���X
	* @param index - �v���C���[�ԍ�
	*/
	DeviceState& Think(vector<IPlayerPtr>& player, list<IItemPtr>& item, IStagePtr& stage, DamageData damage, int index);

private:
	/// �R���s���[�^�[�C���^�[�t�F�C�X
	IComputerPtr m_computer;

	/**
	* @brief �O��̍s�����p������
	*/
	DeviceState& ThinkContinue();

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

};

#endif