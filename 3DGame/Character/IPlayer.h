/**
* @file   IPlayer.h
* @brief  �v���C���C���^�[�t�F�C�X
* @author Syota Tachikawa
*/

#ifndef ___IPLAYER_H
#define ___IPLAYER_H

#include "..\Object\IObject.h"

/**
* @brief �L�����N�^�[�̍s��
*/
enum class Action : BYTE
{
	// �A�C�h�����O
	Idle,

	// �E�ړ�
	RightWalk,

	// ���ړ�
	LeftWalk,

	// �_�b�V��
	Dash,

	// �W�����v�J�n
	JumpStart,

	// �W�����v
	JumpNow,

	// �ʏ�U��
	NormalAttack,

	// �X�}�b�V���U��
	SmashAttack,

	// �K�E�U��(��)
	SpecialAttackUp,

	// ��e
	Damage,

	// �������
	Blow,

	// �A�s�[��
	Appeal
};

/**
* @inteface IPlayer
* @brief    �v���C���[�C���^�[�t�F�C�X
*
* @note�F�S�Ẵv���C���[�͂��̃C���^�[�t�F�C�X���p������
*/
__interface IPlayer : public ISkinObject
{
	/**
	* @brief ������
	*/
	void Initialize();

	/**
	* @brief �X�V
	*/
	bool Update();

	/**
	* @brief �`��(�V�F�[�_�[)
	*/
	void Render(const Matrix& view, const Matrix& proj);

	/**
	* @brief �X�L�����b�V���f�[�^���擾����
	*/
	const SkinMeshPtr& GetSkinMesh() const;

	/**
	* @brief �A�C�h�����O
	*/
	void Idling();

	/**
	* @brief �E�ړ�
	*/
	void MoveRight();

	/**
	* @brief ���ړ�
	*/
	void MoveLeft();

	/**
	* @brief �W�����v
	*/
	void Jump();

	/**
	* @brief ��e���
	* @param blownOff - ������ԃX�s�[�h
	* @param attackerID - �U���҂̔ԍ�
	*/
	void Damage(Vector3 blownOff, int attackerID);

	/**
	* @brief ���S
	* @return �Ō�ɍU�����ꂽ����̔ԍ���Ԃ�
	*/
	int Dead();

	/**
	* @brief �ʏ�U��
	*/
	void NormalAttack();

	/**
	* @brief �X�}�b�V���U��
	*/
	void SmashAttack();

	/**
	* @brief �K�E�U��
	*/
	void SpecialAttack();

	/**
	* @brief �K�E�U��(��)
	*/
	void SpecialAttackUP();

	/**
	* @brief �A�s�[��
	*/
	void Appeal();

	/**
	* @brief ���G���肪���邩�ǂ���
	*/
	bool IsInvincible() const;

	/**
	* @brief �U�����肪���邩�ǂ���
	* @param �U���Ώۂ̃v���C���[�ԍ�
	*/
	bool IsAttack(int enemyID) const;

	/**
	* @brief �n�ʂɒ����Ă��邩�ǂ���
	*/
	bool IsGround() const;

	/**
	* @brief ����\��Ԃ��ǂ���
	*/
	bool IsControl() const;

	/**
	* @brief ���^�C�A���ǂ������`�F�b�N(�X�g�b�N����"0"���Ӗ�����)
	*/
	bool IsRetire() const;

	/**
	* @brief ���W���擾����
	*/
	const Vector3& GetPos() const;

	/**
	* @brief �O�t���[���̍��W���擾����
	*/
	const Vector3& GetOldPos() const;

	/**
	* @brief ��Ԃ��擾����
	*/
	const Action GetAction() const;

	/**
	* @brief �U���͂��擾����
	*/
	const DWORD GetPower() const;

	/**
	* @brief ����{�[�����擾����
	*/
	const Vector3 GetHitPos() const;

	/**
	* @brief �_���[�W���W���擾����
	*/
	const Vector3 GetDamagePos() const;

	/**
	* @brief �X�L�����b�V�������擾����
	*/
	const SkinMeshPtr& GetSkinMesh();

	/**
	* @brief ���W��ݒ肷��
	*/
	void SetPos(const Vector3& pos);

	/**
	* @brief �U�����肪�o�Ă��邩�̃t���O���Z�b�g����
	* @param index - �U���Ώۃv���C���[�ԍ�
	* @param bAttack - �t���O
	*/
	void SetAttack(int enemyID, bool bAttack);

	/**
	* @brief �n�ʂɕt���Ă��邩�ǂ�����ݒ肷��
	*/
	void SetGround(bool bGround);

	/**
	* @brief ���^�C�A�t���O���Z�b�g����
	*/
	void SetRetire(bool bRetire);

	/**
	* @brief ���[�V������ݒ肷��
	*/
	void SetMotion(int nMotion, bool bLoop, bool bForce);
};

/// �v���C���[�̃X�}�[�g�|�C���^
using IPlayerPtr = shared_ptr<IPlayer>;

#endif