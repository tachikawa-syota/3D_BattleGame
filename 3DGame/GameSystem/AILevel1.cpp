#include "AILevel1.h"
#include "DamageData.h"
#include "..\Common\Utility.h"
#include "..\DirectX\InputLib.h"

/// ���̍s�����v�l����܂ł̃t���[��
const int intervalFrame = 60;
/// ���A�Z���o���܂ł̃t���[��
const int intervalReturnFrame = 60;

/**
* @brief �R���X�g���N�^
*/
AILevel1::AILevel1()
{
	// �s�����A�C�h�����O��
	SetAction(Action::Idle, NULL);

	m_actionFrame = 0;
	returnFrame = 0;
	m_nextActionFrame = 0;
}

/**
* @brief �f�X�g���N�^
*/
AILevel1::~AILevel1()
{

}

/**
* @brief �v�l����
* @param player - �v���C���[�N���X
* @param stage - �X�e�[�W�N���X
* @param damage - �_���[�W�f�[�^�N���X
* @param index - �v���C���[�ԍ�
*/
DeviceState& AILevel1::Think(vector<IPlayerPtr>& player, list<IItemPtr>& item, IStagePtr& stage, DamageData damage, int index)
{
	// �O��̃f�o�C�X�����L�^���Ă���
	m_deviceState.prevFlag = m_deviceState.flag;

	// �n�ʂɕt���Ă��Ȃ� ���� ��O�������畜�A�s�������
	if (player[index]->GetPos().y < -0.5f ||
		player[index]->GetPos().x >= 10.0f || player[index]->GetPos().x <= -10.0f){
		return StageReturnAction(player, stage, index);
	}
	else{
		returnFrame = 0;
	}

	// �A�C�e���̈ʒu��T��
	for (auto it = item.begin(); it != item.end(); it++)
	{
		// ���W�擾
		Vector3 pos = (*it)->GetPos();

		// ����
		Vector3 diff = Utility::DiffVector(player[index]->GetPos(), pos);

		// ����
		float length = diff.Length();

		// ���������ȓ��E50%�̊m���ŉ���s�������
		if (length <= 2.0f && Utility::Random(0,1) == 0){
			
			int rand = Utility::Random(0, 2);
			// �W�����v������
			if (rand == 0) SetMoveAction(Action::JumpStart, INPUT_UP);
			// �X�}�b�V���U��������
			if (rand == 1) SetAction(Action::SmashAttack, INPUT_02);
			// ��U��������
			if (rand == 2) SetAction(Action::NormalAttack, INPUT_01);

			return m_deviceState;
		}
	}

	// �s���t���[�������Z����
	m_actionFrame++;

	// ���t���[���𒴂�����
	if (m_actionFrame > m_nextActionFrame){
		m_nextActionFrame = intervalFrame + m_actionFrame;

		// �V�����s�����v�l����
		return ThinkNewAction(player, stage, damage, index);
	}

	// �O��̍s�����p������
	return ThinkContinue();
}

/**
* @brief �V�����s�����v�l����
* @param player - �v���C���[�N���X
* @param stage - �X�e�[�W�N���X
* @param damage - �_���[�W�f�[�^�N���X
* @param index - �v���C���[�ԍ�
*/
DeviceState& AILevel1::ThinkNewAction(vector<IPlayerPtr>& player, IStagePtr& stage, DamageData damage, int index)
{
	// ������
	IPlayerPtr target = nullptr;

	// ��ԋ߂��v���C���[���擾����
	target = Utility::GetNearPlayer(player, index);

	for (int i = 0; i != player.size(); i++)
	{
		// ��O�ɂ��镜�A���̃v���C���[�͑_��Ȃ�
		if (player[i]->GetPos().x >= 10.0f || player[i]->GetPos().x <= -10.0f) {
			continue;
		}
	}

	// �^�[�Q�b�g�̎擾�Ɏ��s������ҋ@��Ԃɂ���
	if (target == nullptr){
		SetMoveAction(Action::Idle, NULL);
		return m_deviceState;
	}

	// �߂��ꍇ�U��������
	Vector3 diff = Utility::DiffVector(player[index]->GetPos(), target->GetPos());
	float length = diff.Length();

	// �����𑪂�
	if (length < 2.5f)
	{
		int rand = Utility::Random(0, 4);
		// �ʏ�U�����o��
		if (rand < 3){
			SetAction(Action::NormalAttack, INPUT_01);
		}
		// �X�}�b�V���U�����o��
		else if (rand == 4){
			SetAction(Action::SmashAttack, INPUT_02);
		}

		return m_deviceState;
	}

	// �����𑪂�
	if (length < 3.0f){
		// �U��
		SetAction(Action::SmashAttack, INPUT_02);
		return m_deviceState;
	}

	// �ēx�擾
	if (diff.x == 0){
		Vector3 diff = Utility::DiffVector(player[index]->GetPos(), target->GetPos());
	}

	// �E�ړ�
	if (diff.x < 0.0f && m_deviceState.axis != INPUT_RIGHT){
		SetMoveAction(Action::RightWalk, INPUT_RIGHT);
	}
	// ���ړ�
	else if (diff.x > 0.0f && m_deviceState.axis != INPUT_LEFT){
		SetMoveAction(Action::LeftWalk, INPUT_LEFT);
	}

	// ���܂ɃW�����v���Ȃ���߂Â��Ă���
	if (Utility::Random(0, 5) >= 4 && m_action != Action::JumpNow)
	{
		if (length > 5.0f){
			SetMoveAction(Action::JumpStart, INPUT_UP);
		}
	}

	// ����
	return m_deviceState;
}

/**
* @brief �X�e�[�W�ɕ��A����s��������
* @param player - �v���C���[�N���X
* @param stage - �X�e�[�W�N���X
* @param index - �v���C���[�ԍ�
*/
DeviceState& AILevel1::StageReturnAction(vector<IPlayerPtr>& player, IStagePtr& stage, int index)
{
	// �X�e�[�W���̏����ɂ����瑦���A�Z���o��
	if (player[index]->GetPos().y < -0.5f)
	{
		if (player[index]->GetPos().x <= 10.0f && player[index]->GetPos().x >= -10.0f){
			SetAction(Action::SpecialAttackUp, INPUT_03);
			return m_deviceState;
		}
	}

	// �����擾
	Vector3 diff = Utility::DiffVector(player[index]->GetPos(), stage->GetPos());

	// �E�ړ�
	if (diff.x < 0.0f && m_deviceState.axis != INPUT_RIGHT){
		SetMoveAction(Action::RightWalk, INPUT_RIGHT);
	}
	// ���ړ�
	else if (diff.x > 0.0f && m_deviceState.axis != INPUT_LEFT){
		SetMoveAction(Action::LeftWalk, INPUT_LEFT);
	}

	// �W�����v������(�X�e�[�W�̕����Ɉړ������Ă���W�����v���s�Ȃ����߂ɍŏ��̃t���[���͓���Ȃ�)
	if (returnFrame > 3){
		SetMoveAction(Action::JumpStart, INPUT_UP);
	}

	// ���t���[���o�߂ŕ��A�Z���g��
	if (returnFrame > intervalReturnFrame){
		returnFrame = 0;
		SetAction(Action::SpecialAttackUp, INPUT_03);
	}

	// �t���[�����Z(�����Ȃ���e���͉��Z���Ȃ�)
	if (player[index]->IsControl()){
		returnFrame++;
	}

	return m_deviceState;
}

/**
* @brief �O��̍s�����p������
*/
DeviceState& AILevel1::ThinkContinue()
{
	// ���͂����������Ă���
	m_deviceState.axis = NULL;
	m_deviceState.flag = NULL;

	switch (m_action)
	{
		// �A�C�h�����O
	case Action::Idle:
		m_deviceState.axis = NULL;
		break;

		// �E�ړ�
	case Action::RightWalk:
		m_deviceState.axis = INPUT_RIGHT;
		break;

		// ���ړ�
	case Action::LeftWalk:
		m_deviceState.axis = INPUT_LEFT;
		break;

		// �W�����v
	case Action::JumpStart:
		m_deviceState.axis = INPUT_UP;
		break;

		// �ʏ�U��
	case Action::NormalAttack:
		m_deviceState.flag = INPUT_01;
		break;

		// �X�}�b�V���U��
	case Action::SmashAttack:
		m_deviceState.flag = INPUT_02;
		break;

		// �K�E�Z(��)
	case Action::SpecialAttackUp:
		m_deviceState.flag = INPUT_03;
		break;

		// �A�s�[��
	case Action::Appeal:
		m_deviceState.flag = INPUT_04;
		break;
	}

	// �O��Ɠ��͂������Ȃ猻�݂̓���NULL�ɐݒ肵����
	// (�A���œ��͂��ꑱ���Ă���Ɣ��肵�Ȃ�����)
	if (m_deviceState.flag == m_deviceState.prevFlag){
		m_deviceState.flag = NULL;
	}

	// ����
	return m_deviceState;
}

/**
* @brief �s����ݒ肷��
* @param action - �s��
* @param flag - ���̓t���O
*/
void AILevel1::SetAction(Action action, unsigned int flag)
{
	m_action = action;
	m_deviceState.flag = flag;
}

/**
* @brief �ړ���ݒ肷��
* @param action - �s��(�ړ��n�̍s��������)
* @param flag - ���̓t���O
*/
void AILevel1::SetMoveAction(Action action, unsigned int flag)
{
	m_action = action;
	m_deviceState.axis = flag;
}