/**
* @file   Motion.h
* @brief  モーションの名前を管理する
* @author Syota Tachikawa
*/

#ifndef ___MOTION_H
#define ___MOTION_H

/**
* @brief モーションの名前
*/
const char s_szMotionNameArray[][32] = {
	/// アイドリング 0
	"idle",

	/// 走り 1
	"run",

	/// ジャンプ1 2
	"jump1",

	/// ジャンプ2 3
	"jump2",

	/// ジャンプ3 4
	"jump3",

	/// ジャンプ4 5
	"jump4",

	/// 回避 6
	"avoid",

	/// ダメージ大 7
	"bigDamage",

	/// ダメージ小 8
	"smallDamage",

	/// 必殺 9
	"Special",

	/// KO 10
	"ko", 

	/// 縦攻撃1 11
	"verticalAttack",

	/// 縦攻撃2 12
	"verticalAttack2",

	/// 縦攻撃3 13
	"verticalAttack3",

	/// 横攻撃1 14
	"besideAttack",

	/// 横攻撃2 15
	"besideAttack2",

	/// 横攻撃3 16
	"besideAttack3",

	/// アピール1 17
	"appeal1",

	/// アピール2 18
	"appeal2",

	/// ジャンプ下攻撃 19
	"uJumpAttack",

	/// ジャンプ攻撃 20
	"jumpAttack",

	/// カウンター 21
	"appeal3",

	/// 縦攻撃派生 22
	"verticalAttackA",

	/// 横攻撃派生 23
	"besideAttackA",

	/// 高速ダッシュ 24
	"Highspeed",

	/// 上ジャンプ攻撃 25
	"upJumpAttack",

	/// ジャンプ中ダメージ 26
	"jumpDamage",

	/// 上昇ダメージ 27
	"upDamage",
};

/**
* @brief モーションの名前
*/
enum MotionName
{
	/// アイドリング 0
	MIdle,

	/// 走り 1
	MRun,

	/// ジャンプ1 2
	MJump1,

	/// ジャンプ2 3
	MJump2,

	/// ジャンプ3 4
	MJump3,

	/// ジャンプ4 5
	MJump4,

	/// 回避 6
	MAvoid,

	/// ダメージ大 7
	MBigDamage,

	/// ダメージ小 8
	MSmallDamage,

	/// 必殺 9
	MSpecial,

	/// KO 10
	MKo,

	/// 縦攻撃1 11
	MVAttack1,

	/// 縦攻撃2 12
	MVAttack2,

	/// 縦攻撃3 13
	MVAttack3,

	/// 横攻撃1 14
	MBAttack1,

	/// 横攻撃2 15
	MBAttack2,

	/// 横攻撃3 16
	MBAttack3,

	/// アピール1 17
	MAppeal1,

	/// アピール2 18
	MAppeal2,

	/// 下ジャンプ攻撃 19
	MUJumpAttack,

	/// ジャンプ攻撃 20
	MJumpAttack,

	/// カウンター 21
	MAppeal3,

	/// 縦攻撃派生 22
	MVAttackA,

	/// 横攻撃派生 23
	MBAttackA,

	/// 高速ダッシュ 24
	MHigheSpeed,

	/// 上ジャンプ攻撃 25
	MUPJumpAttack,

	/// ジャンプ中ダメージ 26
	MJumpDamage,

	/// 上昇ダメージ 27
	MUpDamage,
};

#endif