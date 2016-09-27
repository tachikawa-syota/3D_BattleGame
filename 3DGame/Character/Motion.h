/**
* @file   Motion.h
* @brief  ���[�V�����̖��O���Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___MOTION_H
#define ___MOTION_H

/**
* @brief ���[�V�����̖��O
*/
const char s_szMotionNameArray[][32] = {
	/// �A�C�h�����O 0
	"idle",

	/// ���� 1
	"run",

	/// �W�����v1 2
	"jump1",

	/// �W�����v2 3
	"jump2",

	/// �W�����v3 4
	"jump3",

	/// �W�����v4 5
	"jump4",

	/// ��� 6
	"avoid",

	/// �_���[�W�� 7
	"bigDamage",

	/// �_���[�W�� 8
	"smallDamage",

	/// �K�E 9
	"Special",

	/// KO 10
	"ko", 

	/// �c�U��1 11
	"verticalAttack",

	/// �c�U��2 12
	"verticalAttack2",

	/// �c�U��3 13
	"verticalAttack3",

	/// ���U��1 14
	"besideAttack",

	/// ���U��2 15
	"besideAttack2",

	/// ���U��3 16
	"besideAttack3",

	/// �A�s�[��1 17
	"appeal1",

	/// �A�s�[��2 18
	"appeal2",

	/// �W�����v���U�� 19
	"uJumpAttack",

	/// �W�����v�U�� 20
	"jumpAttack",

	/// �J�E���^�[ 21
	"appeal3",

	/// �c�U���h�� 22
	"verticalAttackA",

	/// ���U���h�� 23
	"besideAttackA",

	/// �����_�b�V�� 24
	"Highspeed",

	/// ��W�����v�U�� 25
	"upJumpAttack",

	/// �W�����v���_���[�W 26
	"jumpDamage",

	/// �㏸�_���[�W 27
	"upDamage",
};

/**
* @brief ���[�V�����̖��O
*/
enum MotionName
{
	/// �A�C�h�����O 0
	MIdle,

	/// ���� 1
	MRun,

	/// �W�����v1 2
	MJump1,

	/// �W�����v2 3
	MJump2,

	/// �W�����v3 4
	MJump3,

	/// �W�����v4 5
	MJump4,

	/// ��� 6
	MAvoid,

	/// �_���[�W�� 7
	MBigDamage,

	/// �_���[�W�� 8
	MSmallDamage,

	/// �K�E 9
	MSpecial,

	/// KO 10
	MKo,

	/// �c�U��1 11
	MVAttack1,

	/// �c�U��2 12
	MVAttack2,

	/// �c�U��3 13
	MVAttack3,

	/// ���U��1 14
	MBAttack1,

	/// ���U��2 15
	MBAttack2,

	/// ���U��3 16
	MBAttack3,

	/// �A�s�[��1 17
	MAppeal1,

	/// �A�s�[��2 18
	MAppeal2,

	/// ���W�����v�U�� 19
	MUJumpAttack,

	/// �W�����v�U�� 20
	MJumpAttack,

	/// �J�E���^�[ 21
	MAppeal3,

	/// �c�U���h�� 22
	MVAttackA,

	/// ���U���h�� 23
	MBAttackA,

	/// �����_�b�V�� 24
	MHigheSpeed,

	/// ��W�����v�U�� 25
	MUPJumpAttack,

	/// �W�����v���_���[�W 26
	MJumpDamage,

	/// �㏸�_���[�W 27
	MUpDamage,
};

#endif