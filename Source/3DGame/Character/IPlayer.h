/**
* @file   IPlayer.h
* @brief  プレイヤインターフェイス
* @author Syota Tachikawa
*/

#ifndef ___IPLAYER_H
#define ___IPLAYER_H

#include "..\Object\IObject.h"

/**
* @brief キャラクターの行動
*/
enum class Action : BYTE
{
	// アイドリング
	Idle,

	// 右移動
	RightWalk,

	// 左移動
	LeftWalk,

	// ダッシュ
	Dash,

	// ジャンプ開始
	JumpStart,

	// ジャンプ
	JumpNow,

	// 通常攻撃
	NormalAttack,

	// スマッシュ攻撃
	SmashAttack,

	// 必殺攻撃(上)
	SpecialAttackUp,

	// 被弾
	Damage,

	// 吹き飛び
	Blow,

	// アピール
	Appeal
};

/**
* @inteface IPlayer
* @brief    プレイヤーインターフェイス
*
* @note：全てのプレイヤーはこのインターフェイスを継承する
*/
__interface IPlayer : public ISkinObject
{
	/**
	* @brief 初期化
	*/
	void Initialize();

	/**
	* @brief 更新
	*/
	bool Update();

	/**
	* @brief 描画(シェーダー)
	*/
	void Render(const Matrix& view, const Matrix& proj);

	/**
	* @brief スキンメッシュデータを取得する
	*/
	const SkinMeshPtr& GetSkinMesh() const;

	/**
	* @brief アイドリング
	*/
	void Idling();

	/**
	* @brief 右移動
	*/
	void MoveRight();

	/**
	* @brief 左移動
	*/
	void MoveLeft();

	/**
	* @brief ジャンプ
	*/
	void Jump();

	/**
	* @brief 被弾状態
	* @param blownOff - 吹き飛ぶスピード
	* @param attackerID - 攻撃者の番号
	*/
	void Damage(Vector3 blownOff, int attackerID);

	/**
	* @brief 死亡
	* @return 最後に攻撃された相手の番号を返す
	*/
	int Dead();

	/**
	* @brief 通常攻撃
	*/
	void NormalAttack();

	/**
	* @brief スマッシュ攻撃
	*/
	void SmashAttack();

	/**
	* @brief 必殺攻撃
	*/
	void SpecialAttack();

	/**
	* @brief 必殺攻撃(上)
	*/
	void SpecialAttackUP();

	/**
	* @brief アピール
	*/
	void Appeal();

	/**
	* @brief 無敵判定があるかどうか
	*/
	bool IsInvincible() const;

	/**
	* @brief 攻撃判定があるかどうか
	* @param 攻撃対象のプレイヤー番号
	*/
	bool IsAttack(int enemyID) const;

	/**
	* @brief 地面に着いているかどうか
	*/
	bool IsGround() const;

	/**
	* @brief 操作可能状態かどうか
	*/
	bool IsControl() const;

	/**
	* @brief リタイアかどうかをチェック(ストック数が"0"を意味する)
	*/
	bool IsRetire() const;

	/**
	* @brief 座標を取得する
	*/
	const Vector3& GetPos() const;

	/**
	* @brief 前フレームの座標を取得する
	*/
	const Vector3& GetOldPos() const;

	/**
	* @brief 状態を取得する
	*/
	const Action GetAction() const;

	/**
	* @brief 攻撃力を取得する
	*/
	const DWORD GetPower() const;

	/**
	* @brief 剣先ボーンを取得する
	*/
	const Vector3 GetHitPos() const;

	/**
	* @brief ダメージ座標を取得する
	*/
	const Vector3 GetDamagePos() const;

	/**
	* @brief スキンメッシュ情報を取得する
	*/
	const SkinMeshPtr& GetSkinMesh();

	/**
	* @brief 座標を設定する
	*/
	void SetPos(const Vector3& pos);

	/**
	* @brief 攻撃判定が出ているかのフラグをセットする
	* @param index - 攻撃対象プレイヤー番号
	* @param bAttack - フラグ
	*/
	void SetAttack(int enemyID, bool bAttack);

	/**
	* @brief 地面に付いているかどうかを設定する
	*/
	void SetGround(bool bGround);

	/**
	* @brief リタイアフラグをセットする
	*/
	void SetRetire(bool bRetire);

	/**
	* @brief モーションを設定する
	*/
	void SetMotion(int nMotion, bool bLoop, bool bForce);
};

/// プレイヤーのスマートポインタ
using IPlayerPtr = shared_ptr<IPlayer>;

#endif