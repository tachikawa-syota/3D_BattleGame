#include "Rock.h"
#include "Motion.h"
#include "CharacterParam.h"
#include "..\Common\CommonParam.h"
#include "..\Common\StringDefine.h"
#include "..\Common\UniqueData.h"
#include "..\Common\Utility.h"
#include "..\DirectX\Direct3DData.h"
#include "..\DirectX\SoundPlayer.h"
#include "..\DirectX\InputLib.h"

/**
* @brief �R���X�g���N�^
*/
Rock::Rock(unsigned int index)
{
	// �ԍ��̃Z�b�g
	m_id = index;

	// �v���C���[�ԍ��ɉ����č��W�ƌ�����ݒ肷��
	switch (index)
	{
		// 1P
	case 0:
		m_pos = Vector3(GetComPrm->InitPlayer1PosX, GetComPrm->InitPosY, GetComPrm->InitPosZ);
		m_angle = RAD(270);
		break;
		// 2P
	case 1:
		m_pos = Vector3(GetComPrm->InitPlayer2PosX, GetComPrm->InitPosY, GetComPrm->InitPosZ);
		m_angle = RAD(90);
		break;
		// 3P
	case 2:
		m_pos = Vector3(GetComPrm->InitPlayer3PosX, GetComPrm->InitPosY, GetComPrm->InitPosZ);
		m_angle = RAD(270);
		break;
		// 4P
	case 3:
		m_pos = Vector3(GetComPrm->InitPlayer4PosX, GetComPrm->InitPosY, GetComPrm->InitPosZ);
		m_angle = RAD(90);
		break;
		// �G���[(��������l�����Ă���)
	default:
		m_pos = Vector3(GetComPrm->InitPlayer2PosX, GetComPrm->InitPosY, GetComPrm->InitPosZ);
		m_angle = RAD(90);
		break;
	}

	// ���W���L�^
	m_oldPos = m_pos;
	
	// �v���C���[�̐���
	m_obj = make_shared<SkinMesh>();

	// �X�L�����b�V���ǂݍ���
	m_obj->LoadAnimeXMesh("Data/Model/Player/", "player_1.X");
}

/**
* @brief �f�X�g���N�^
*/
Rock::~Rock()
{
	m_isAttack.clear();
}

/**
* @brief ������
*/
void Rock::Initialize()
{
	// ���W�̏�����
	m_obj->SetPos(m_pos);

	// ���x�̏�����
	m_spd = Vector3();
	m_oldSpd = Vector3();

	// �傫���̏�����
	m_scale = GetChrPrm->RockScale;
	m_obj->SetScale(m_scale);

	// ��Ԃ̏�����
	m_action = Action::Idle;
	m_nMotion = MIdle;
	m_oldAction = m_action;

	// ���[�V�����̏�����
	SetMotion(0, TRUE, TRUE);

	// �t���O�̐ݒ�
	m_isInvincible = false;
	// �v���C���[�̐������U������t���O������������
	for (int i = 0; i < UniqueData::GetInstance()->GetBattleNum(); i++){
		m_isAttack.push_back(false);
	}
	m_isGround = true;
	m_isControl = true;
	m_isRetire = false;
	m_isBlowRigidity = false;

	// �W�����v�񐔂�������
	m_jumpCount = 0;

	// �U����
	m_power = 0;

	m_enemyID = -1;

	m_subWork = 0;
}

/**
* @brief �X�V
*/
bool Rock::Update()
{
	// �n�ʔ���
	if (m_isGround)
	{
		// �W�����v�J�E���g�̃��Z�b�g
		if (m_action != Action::JumpStart){
			m_jumpCount = 0;
		}

		//��e���ȊO�Ȃ�
		if (m_action != Action::Damage){
			m_enemyID = -1;
		}
		// �n�ʂɖ��܂�̂�}������
		if (m_action != Action::JumpStart){
			m_spd.y = 0.0f;
		}

		m_gravity = 0.0f;
	}
	// �󒆂Ȃ�d�ʂ�������
	else{
		m_gravity = GetComPrm->Gravity;
	}

	// ������эd��
	if (m_isBlowRigidity){
		// ���X�Ɍ���
		m_spd.x *= 0.97f;
	}

	// �d�͂����Z����
	m_spd.y += m_gravity;

	// ��O�̍s���ƈႤ�s�������Ă��銎�s���\���
	if (m_action != m_oldAction && m_isControl)
	{
		if (m_action != Action::JumpStart){
			ParamReset();
		}
	}

	// �s���X�V
	UpdateAction();

	// ���W�ɑ��x�����Z
	m_pos += m_spd;
	// ���W�X�V
	m_obj->SetPos(m_pos);
	// �p�x�X�V
	m_obj->SetAngle(m_angle);
	// �v���C���[�X�V
	m_obj->Update();

	// ���W���L�^����
	m_oldPos = m_pos;
	// ���x���L�^����
	m_oldSpd = m_spd;
	// ��Ԃ��L�^����
	m_oldAction = m_action;

	return true;
}

/**
* @brief �`��(�V�F�[�_�[)
*/
void Rock::Render(const Matrix& view, const Matrix& proj)
{
	// Z�o�b�t�@ON
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	// ���^�C�A���̃v���C���[�͕`�悵�Ȃ�
	if (!m_isRetire){
		m_obj->RenderAnimeXMesh(view, proj);
	}
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
}

/**
* @brief �X�L�����b�V���f�[�^���擾����
*/
const SkinMeshPtr& Rock::GetSkinMesh() const
{
	return m_obj;
}

/**
* @brief �A�C�h�����O
*/
void Rock::Idling()
{
	// �t���[���X�V
	m_obj->UpdateFrame();

	// ���[�V�����̐ݒ�ƃt���[���̍X�V
	if (m_nMotion != MIdle){
		SetMotion(MIdle, true, true);
	}
	// �A�C�h�����
	m_action = Action::Idle;
	// ���x������
	m_spd = Vector3(0.0f, m_spd.y, 0.0f);
}

/**
* @brief �A�C�h�����O�X�V
*/
void Rock::UpdateIdling()
{
	m_obj->UpdateFrame();
}

/**
* @brief �E�ړ�
*/
void Rock::MoveRight()
{
	if (m_action != Action::JumpNow && m_action != Action::JumpStart)
	{
		//�_�b�V�����[�V�������[�v
		if (m_nMotion != MRun) SetMotion(MRun, true, true);

		// ����
		m_action = Action::RightWalk;
	}

	// ������
	m_dir = Direction::Right;

	// �X�V
	UpdateMoveRight();
}

/**
* @brief ���ړ�
*/
void Rock::MoveLeft()
{
	if (m_action != Action::JumpNow && m_action != Action::JumpStart)
	{
		//�_�b�V�����[�V�������[�v
		if (m_nMotion != MRun) SetMotion(MRun, true, true);

		// ����
		m_action = Action::LeftWalk;
	}

	// ������
	m_dir = Direction::Left;

	// �X�V
	UpdateMoveLeft();
}

/**
* @brief �E�ړ��X�V
*/
void Rock::UpdateMoveRight()
{
	// ������эd���t���O������
	m_isBlowRigidity = false;

	// �t���[�������Z����
	m_obj->UpdateFrame();

	// ���x�Z�b�g
	m_spd.x = -0.1f;

	// �p�x�Z�b�g
	m_angle = RAD(90);
}

/**
* @brief ���ړ��X�V
*/
void Rock::UpdateMoveLeft()
{
	// ������эd���t���O������
	m_isBlowRigidity = false;

	// �t���[�������Z����
	m_obj->UpdateFrame();

	// ���x�Z�b�g
	m_spd.x = 0.1f;

	// �p�x�Z�b�g
	m_angle = RAD(270);

}

/**
* @brief �W�����v
*/
void Rock::Jump()
{
	// �ő吔�ɒB���Ă����牽�����Ȃ�
	if (m_jumpCount >= GetComPrm->JumpMax) return;

	// �u�ԓI�ȘA���W�����v��}������
	if (m_jumpCount > 0){
		if (m_nMotion != MJump2 && m_nMotion != MJump3) return;
	}

	m_subWork = 0;

	// �A�j���[�V�����؂�ւ��t���[��
	m_obj->SetAnimShiftTime(0.0f);

	// �W�����v���
	m_action = Action::JumpStart;

	// �W�����v�J�E���g�𑝂₷
	m_jumpCount++;

	// ���x��ݒ肷��
	m_spd = Vector3(m_spd.x, 0.0f, m_spd.z);
	m_spd.y += 0.2f;

	// ���[�V�����̐ݒ�
	SetMotion(MJump1, false, true);
}

/**
* @brief �W�����v�X�V
*/
void Rock::UpdateJump()
{
	switch (m_subWork)
	{
	case 0:
		if (m_obj->GetTime() > ROCK_MOTION_JUMP1){
			if (m_nMotion == MJump1){
				SetMotion(MJump2, false, true);
				m_subWork++;
			}
		}
		else{
			m_obj->UpdateFrame();
		}
		break;

	case 1:
		if (m_obj->GetTime() > ROCK_MOTION_JUMP2){
			if (m_nMotion == MJump2){
				SetMotion(MJump3, false, true);
				m_subWork++;
			}
		}
		else{
			m_obj->UpdateFrame();
		}
		break;

	case 2:
		if (m_obj->GetTime() > ROCK_MOTION_JUMP3){
		}
		else{
			m_obj->UpdateFrame();
		}
		break;
	}

	// �n�ʂɕt������p�����[�^�����Z�b�g����
	if (m_isGround){
		EndMotion();
		ParamReset();
	}
}

/**
* @brief ��e���
* @param blownOff - ������ԃX�s�[�h
* @param attackerID - �U���҂̔ԍ�
*/
void Rock::Damage(Vector3 blownOff, int attackerID)
{
	// �p�����[�^�����Z�b�g����
	ParamReset();

	// ��e���
	m_action = Action::Damage;

	// ���[�V�����̃Z�b�g
	SetMotion(MSmallDamage, false, true);

	// ����s��
	m_isControl = false;

	// ���G�t���O�𗧂Ă�
	m_isInvincible = true;

	// ������ё��x�̐ݒ�
	m_spd = blownOff;

	// ��Βl�����l�ȏ�Ȃ琁����я�Ԃɂ���
	if (abs(m_spd.x) > 0.3f){
		m_action = Action::Blow;
	}

	// ����̔ԍ���o�^
	m_enemyID = attackerID;
}

/**
* @brief ��e��Ԃ̍X�V
*/
void Rock::UpdateDamage()
{
	// �A�j���[�V�������I��������
	if (m_obj->IsAnimPeriod())
	{
		// �n�ʂɕt���Ă�����
		if (m_isGround){
			// �A�C�h�����O�ɖ߂�
			m_action = Action::Idle;
		}
		// �󒆂Ȃ�W�����v�ɂ���
		else{
			m_isBlowRigidity = true;
			m_action = Action::JumpNow;
			SetMotion(MJump3, false, true);
		}

		// ����\
		m_isControl = true;
		// ���G�t���O������
		m_isInvincible = false;
	}
	else{
		// �t���[���X�V
		m_obj->UpdateFrame(0.7f);
	}

	// �A���Ńq�b�g���Ȃ����߂̐���p
	static int frame = 0;
	if (frame > 6){
		m_isInvincible = false;
		frame = 0;
	}
	frame++;
}

/**
* @brief ���S
* @return �Ō�ɍU�����ꂽ����̔ԍ���Ԃ�
*/
int Rock::Dead()
{
	// �W�����v�J�E���g�̏�����
	m_jumpCount = 0;

	// �{�C�X�̍Đ�
	SoundPlayer::GetInstance()->Play(ROCK_DEAD_VOICE);

	// ���[�V�����̏�����
	EndMotion();

	// �p�����[�^�̏�����
	ParamReset();

	// ���x������������
	m_spd = Vector3();

	// ����̔ԍ�
	return m_enemyID;
}

/**
* @brief �ʏ�U���J�n
*/
void Rock::NormalAttack()
{
	// �ʏ�U��
	m_action = Action::NormalAttack;
	// �{�C�X�̍Đ�
	SoundPlayer::GetInstance()->Play(ROCK_NORMALATTACK_VOICE);
}

/**
* @brief �X�}�b�V���U��
*/
void Rock::SmashAttack()
{
	// �X�}�b�V���U��
	m_action = Action::SmashAttack;
	// �{�C�X�̍Đ�
	SoundPlayer::GetInstance()->Play(ROCK_SMASHATTACK_VOICE);
}

/**
* @brief �K�E�U��(��)
*/
void Rock::SpecialAttackUP()
{
	// �ʏ�U��
	m_action = Action::SpecialAttackUp;
	// �{�C�X�̍Đ�
	SoundPlayer::GetInstance()->Play(ROCK_SPECIALATTACKUP_VOICE);
}

/**
* @brief �A�s�[��
*/
void Rock::Appeal()
{
	// �ʏ�U��
	m_action = Action::Appeal;
	// �{�C�X�̍Đ�
	SoundPlayer::GetInstance()->Play(ROCK_APPEAL_VOICE);
}

/**
* @brief �ʏ�U��
*/
void Rock::UpdateNormalAttack()
{
	switch (m_subWork)
	{
	case 0:
		// ���[�V�����̐ݒ�
		if (m_nMotion != MVAttack1) SetMotion(MVAttack1, false, true);
		
		// �n�ʂɕt���Ă����瑬�x������
		if (m_isGround) m_spd = Vector3();

		// �U���͂̐ݒ�
		m_power = GetChrPrm->RockNormalAttackPower;

		// ����s��
		m_isControl = false;

		for (int i = 0; i != m_isAttack.size(); i++){
			m_isAttack[i] = true;
		}

		m_subWork++;
		break;

	case 1:
		// ���t���[���o�߂œ����蔻����폜
		if (m_obj->GetTime() > 2400.0f / 4800.0f)
		{
			for (int i = 0; i != m_isAttack.size(); i++){
				m_isAttack[i] = false;
			}
		}
		// �A�j���[�V�������I��������p�����[�^�����Z�b�g����
		if (m_obj->IsAnimPeriod()){
			EndMotion();
			ParamReset();
		}
		else{
			// �A�j���[�V�����X�V
			m_obj->UpdateFrame(1.6f);
		}
		break;
	}
}

/**
* @brief �X�}�b�V���U��
*/
void Rock::UpdateSmashAttack()
{
	switch (m_subWork)
	{
	case 0:
		// ���[�V�����̐ݒ�
		if (m_nMotion != MBAttack1) SetMotion(MBAttack1, false, true);

		// �n�ʂɕt���Ă����瑬�x������
		if (m_isGround) m_spd = Vector3();

		// �U���͂̐ݒ�
		m_power = GetChrPrm->RockSmashAttackPower;

		// ����s��
		m_isControl = false;

		m_subWork++;
		break;

	case 1:
		// ���t���[���o�߂œ����蔻��𐶐�����
		if (m_obj->GetTime() > 1700.0f / 4800.0f)
		{
			for (int i = 0; i != m_isAttack.size(); i++){
				m_isAttack[i] = true;
			}
		}

		// �A�j���[�V�����̍Đ����I��������
		if (m_obj->IsAnimPeriod()){
			// ���Z�b�g����
			EndMotion();
			ParamReset();
		}
		else{
			// �A�j���[�V�������X�V
			m_obj->UpdateFrame();
		}
		break;
	}
}

/**
* @brief �K�E�U��
*/
void Rock::SpecialAttack()
{
	switch (m_subWork)
	{
	case 0:
		// ���[�V�����̐ݒ�
		if (m_nMotion != MSpecial) SetMotion(MSpecial, false, true);

		// �n�ʂɕt���Ă����瑬�x������
		if (m_isGround) m_spd = Vector3();

		// �U���͂̐ݒ�
		m_power = GetChrPrm->RockSmashAttackPower;

		// ����s��
		m_isControl = false;

		// ���G�t���O�𗧂Ă�
		m_isInvincible = true;

		m_subWork++;
		break;

	case 1:
		// �A�j���[�V�����̍Đ����I��������
		if (m_obj->IsAnimPeriod()){
			// ���G�t���O������
			m_isInvincible = false;
			// ���Z�b�g����
			EndMotion();
			ParamReset();
		}
		else{
			// �A�j���[�V�������X�V
			m_obj->UpdateFrame();
		}
		break;
	}
}

/**
* @brief �K�E�U��(��)
*/
void Rock::UpdateSpecialAttackUP()
{
	switch (m_subWork)
	{
	case 0:
		// ���[�V�����̐ݒ�
		if (m_nMotion != MVAttack3)	SetMotion(MVAttack3, false, true);

		// ��ɑ��x�����������Ă���
		m_spd = Vector3();

		// �V���ɑ��x���Z�b�g����
		m_spd = Vector3(m_oldSpd.x, 0.4f, m_oldSpd.z);

		// �U���͂̐ݒ�
		m_power = GetChrPrm->RockSpecialAttackUpPower;

		// ����s��
		m_isControl = false;

		for (int i = 0; i != m_isAttack.size(); i++){
			m_isAttack[i] = true;
		}

		m_subWork++;
		break;

	case 1:
		// ��莞�Ԃ܂ŃA�j���[�V�������X�V����
		if (m_obj->GetTime() > 3300.0f / 4800.0f){
		}
		else{
			m_obj->UpdateFrame();
		}

		if (m_isGround){
			EndMotion();
			ParamReset();
		}
		break;
	}
}

/**
* @brief �A�s�[��
*/
void Rock::UpdateAppeal()
{
	switch (m_subWork)
	{
	case 0:
		// ���[�V�����̐ݒ�
		if (m_nMotion != MAppeal1)	SetMotion(MAppeal1, false, true);

		// ���x��������
		m_spd = Vector3();

		// ����s��
		m_isControl = false;
		m_subWork++;
		break;

	case 1:
		// �A�j���[�V�����̍Đ����I��������
		if (m_obj->IsAnimPeriod()){
			// ���Z�b�g����
			EndMotion();
			ParamReset();
		}
		else{
			// �A�j���[�V�������X�V
			m_obj->UpdateFrame();
		}
		break;
	}

	// �����Ȃ��悤�ɒ�������
	m_spd.x = 0.0f;
	m_spd.z = 0.0f;
}

/**
* @brief ���W�擾
*/
const Vector3& Rock::GetPos() const
{
	return m_pos;
}

/**
* @brief �O�t���[���̍��W���擾����
*/
const Vector3& Rock::GetOldPos() const
{
	return m_oldPos;
}

/**
* @brief ��Ԃ��擾����
*/
const Action Rock::GetAction() const
{
	return m_action;
}

/**
* @brief �U���͂��擾����
*/
const DWORD Rock::GetPower() const
{
	return m_power;
}

/**
* @brief ����{�[�����擾����
*/
const Vector3 Rock::GetHitPos() const
{
	Matrix mat;
	
	// ����̈ʒu
	m_obj->GetPartsMatrixByName(&mat, "Bone002");
	
	Vector3 p(mat._41, mat._42, mat._43);
	
	return p;
}

/**
* @brief �_���[�W���W���擾����
*/
const Vector3 Rock::GetDamagePos() const
{
	Matrix mat;
	//�v���C���[�w�b�h�{�[��
	m_obj->GetPartsMatrixByName(&mat, "Bip001");
	return Vector3(mat._41, mat._42, mat._43);
}

/**
* @brief �X�L�����b�V�������擾����
*/
const SkinMeshPtr& Rock::GetSkinMesh()
{
	return m_obj;
}

/**
* @brief ���W��ݒ肷��
*/
void Rock::SetPos(const Vector3& pos)
{
	m_pos = pos;
}

/**
* @brief �U������t���O���Z�b�g����
*/
void Rock::SetAttack(int enemyID, bool bAttack)
{
	m_isAttack[enemyID] = bAttack;
}

/**
* @brief �n�ʂɕt���Ă��邩�ǂ�����ݒ肷��
*/
void Rock::SetGround(bool bGround)
{
	m_isGround = bGround;
}

/**
* @brief ���^�C�A�t���O���Z�b�g����
*/
void Rock::SetRetire(bool bRetire)
{
	m_isRetire = bRetire;
}

/**
* @brief ���[�V������ݒ肷��
*/
void Rock::SetMotion(int nMotion, bool bLoop, bool bForce)
{
	//���݂Ǝ��̃��[�V�������قȂ�ꍇ
	if (bForce || m_nMotion != nMotion){
		//���O��胂�[�V�����ԍ����擾
		m_obj->SetAnimName(s_szMotionNameArray[nMotion], bLoop);
		//���[�V������ݒ�
		m_nMotion = nMotion;
	}
}

/**
* @brief �s���X�V
*/
void Rock::UpdateAction()
{
	// �s���ɉ������X�V����������
	switch (m_action)
	{
	case Action::Idle:
		UpdateIdling();
		break;

	case Action::JumpStart:
		m_action = Action::JumpNow;
		break;

	case Action::JumpNow:
		UpdateJump();
		break;

	case Action::NormalAttack:
		UpdateNormalAttack();
		break;

	case Action::SmashAttack:
		UpdateSmashAttack();
		break;

	case Action::SpecialAttackUp:
		UpdateSpecialAttackUP();
		break;

	case Action::Damage:
	case Action::Blow:
		UpdateDamage();
		break;

	case Action::Appeal:
		UpdateAppeal();
		break;
	}
}

/**
* @brief ���[�V�����̏I��
*/
void Rock::EndMotion()
{
	// �A�j���[�V�����̃V�t�g�^�C��
	m_obj->SetAnimShiftTime(0.25f);

	// �n��
	if (m_isGround){
		Idling();
	}
	// ��
	else {
		m_action = Action::JumpNow;
		SetMotion(MJump3, false, true);
	}
}

/**
* @brief �p�����[�^�����Z�b�g����
*/
void Rock::ParamReset()
{
	// �T�u���[�N���Z�b�g
	m_subWork = 0;
	// �U���͂̃��Z�b�g
	m_power = 0;
	// �U������t���O������
	for (int i = 0; i != m_isAttack.size(); i++){
		m_isAttack[i] = false;
	}
	// ����\
	m_isControl = true;
}

/**
* @brief ���G���肪���邩�ǂ���
*/
bool Rock::IsInvincible() const
{
	return m_isInvincible;
}

/**
* @brief �U�����肪���邩�ǂ���
*/
bool Rock::IsAttack(int enemyID) const
{
	return m_isAttack[enemyID];
}

/**
* @brief �n�ʂɒ����Ă��邩�ǂ���
*/
bool Rock::IsGround() const
{
	return m_isGround;
}

/**
* @brief ����\��Ԃ��ǂ���
*/
bool Rock::IsControl() const
{
	return m_isControl;
}

/**
* @brief ���^�C�A���ǂ������`�F�b�N(�X�g�b�N����"0"���Ӗ�����)
*/
bool Rock::IsRetire() const
{
	return m_isRetire;
}