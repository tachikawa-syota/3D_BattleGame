/**
* @file   IComputer.h
* @brief  AI�̊Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___ICOMPUTER_H
#define ___ICOMPUTER_H

#include "..\Common\STDXStruct.h"
#include "..\Character\IPlayer.h"

// �O���錾
class DamageData;

__interface IComputer
{
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
	* @brief �O��̍s�����p������
	*/
	DeviceState& ThinkContinue();

	/**
	* @brief �X�e�[�W�ɕ��A����s��������
	* @param player - �v���C���[�N���X
	* @param stage - �X�e�[�W�N���X
	* @param index - �v���C���[�ԍ�
	*/
	DeviceState& StageReturnAction(vector<IPlayerPtr>& player, IStagePtr& stage, int index);
};

/// �X�}�[�g�|�C���^
using IComputerPtr = shared_ptr<IComputer>;

#endif