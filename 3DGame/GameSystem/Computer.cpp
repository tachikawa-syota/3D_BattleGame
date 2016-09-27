#include "Computer.h"
#include "..\Common\Utility.h"
#include "..\DirectX\InputLib.h"
#include "..\GameSystem\DamageData.h"

/**
* @brief ������
* @param ���x��
*/
void Computer::Initialize(AILevel level)
{
	// �w�肳�ꂽ���x����AI���쐬����
	AILevelFactory factory;
	m_computer = factory.CreateAI(level);
}

/**
* @brief �v�l����
* @param player - �v���C���[�N���X
* @param stage - �X�e�[�W�N���X
* @param damage - �_���[�W�f�[�^�N���X
* @param index - �v���C���[�ԍ�
*/
DeviceState& Computer::Think(vector<IPlayerPtr>& player, list<IItemPtr>& item, IStagePtr& stage, DamageData damage, int index)
{
	return m_computer->Think(player, item, stage, damage, index);
}

/**
* @brief �O��̍s�����p������
*/
DeviceState& Computer::ThinkContinue()
{
	return m_computer->ThinkContinue();
}

/**
* @brief �V�����s�����v�l����
* @param player - �v���C���[�N���X
* @param stage - �X�e�[�W�N���X
* @param damage - �_���[�W�f�[�^�N���X
* @param index - �v���C���[�ԍ�
*/
DeviceState& Computer::ThinkNewAction(vector<IPlayerPtr>& player, IStagePtr& stage, DamageData damage, int index)
{
	return m_computer->ThinkNewAction(player, stage, damage, index);
}

/**
* @brief �X�e�[�W�ɕ��A����s��������
* @param player - �v���C���[�N���X
* @param stage - �X�e�[�W�N���X
* @param index - �v���C���[�ԍ�
*/
DeviceState& Computer::StageReturnAction(vector<IPlayerPtr>& player, IStagePtr& stage, int index)
{
	return m_computer->StageReturnAction(player, stage, index);
}