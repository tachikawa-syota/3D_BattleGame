/**
* @file   Rock.h
* @brief  �L�����N�^�["���b�N"���Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___ROCK_H
#define ___ROCK_H

#include "IPlayer.h"
#include "..\Common\STDXEnum.h"

#define ROCK_MOTION_JUMP1 400.0f / 4800.0f
#define ROCK_MOTION_JUMP2 1760.0f / 4800.0f
#define ROCK_MOTION_JUMP3 800.0f / 4800.0f

/**
* @brief �L�����N�^�["���b�N"�̃N���X
*/
class Rock : public IPlayer
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	Rock(unsigned int index);

	/**
	* @brief �f�X�g���N�^
	*/
	~Rock();

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
	* @brief �ʏ�U���J�n
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
	* @brief ���W�擾
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
	* @brief �n�ʂɕt���Ă��邩�ǂ�����ݒ肷��
	*/
	void SetGround(bool bGround);

	/**
	* @brief ���^�C�A�t���O���Z�b�g����
	*/
	void SetRetire(bool bRetire);

	/**
	* @brief �U�����肪�o�Ă��邩�̃t���O���Z�b�g����
	* @param index - �U���Ώۃv���C���[�ԍ�
	* @param bAttack - �t���O
	*/
	void SetAttack(int enemyID, bool bAttack);

	/**
	* @brief ���[�V������ݒ肷��
	*/
	void SetMotion(int nMotion, bool bLoop, bool bForce);

private:
	/// �I�u�W�F�N�g
	SkinMeshPtr m_obj;

	/// �v���C���[�ԍ�
	unsigned int m_id;

	/// ����̔ԍ�(�U�����Ă����v���C���[)
	int m_enemyID;

	/// ���W
	Vector3 m_pos;

	/// �O�t���[���̍��W
	Vector3 m_oldPos;

	/// ���x
	Vector3 m_spd;

	/// �O�t���[���̑��x
	Vector3 m_oldSpd;

	/// �s��
	Action m_action;

	/// �O�t���[���̍s��
	Action m_oldAction;

	/// ����
	Direction m_dir;

	/// �p�x
	float m_angle;

	/// �傫��
	float m_scale;

	/// �T�u���[�N
	int m_subWork;

	/// ���[�V�����ԍ�
	int m_nMotion;

	/// �U����
	DWORD m_power;

	/// �W�����v��
	unsigned char m_jumpCount;

	/// ���G����
	bool m_isInvincible;

	/// �U������
	vector<bool> m_isAttack;

	/// �n�ʂɕt���Ă��邩�ǂ���
	bool m_isGround;

	/// ����t���O
	bool m_isControl;

	/// ���^�C�A�t���O
	bool m_isRetire;

	/// ������эd���t���O
	bool m_isBlowRigidity;

	/// �d��
	float m_gravity;

	/**
	* @brief �A�C�h�����O�X�V
	*/
	void UpdateIdling();

	/**
	* @brief �E�ړ��X�V
	*/
	void UpdateMoveRight();

	/**
	* @brief ���ړ��X�V
	*/
	void UpdateMoveLeft();

	/**
	* @brief �W�����v�X�V
	*/
	void UpdateJump();

	/**
	* @brief �s���X�V
	*/
	void UpdateAction();

	/**
	* @brief ��e
	*/
	void UpdateDamage();

	/**
	* @brief �ʏ�U��
	*/
	void UpdateNormalAttack();

	/**
	* @brief �X�}�b�V���U��
	*/
	void UpdateSmashAttack();

	/**
	* @brief �K�E�U��(��)
	*/
	void UpdateSpecialAttackUP();

	/**
	* @brief �A�s�[��
	*/
	void UpdateAppeal();

	/**
	* @brief ���[�V�����̏I��
	*/
	void EndMotion();

	/**
	* @brief �p�����[�^�����Z�b�g����
	*/
	void ParamReset();
};

#endif