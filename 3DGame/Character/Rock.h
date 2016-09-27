/**
* @file   Rock.h
* @brief  キャラクター"ロック"を管理する
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
* @brief キャラクター"ロック"のクラス
*/
class Rock : public IPlayer
{
public:
	/**
	* @brief コンストラクタ
	*/
	Rock(unsigned int index);

	/**
	* @brief デストラクタ
	*/
	~Rock();

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
	* @brief 通常攻撃開始
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
	* @brief 座標取得
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
	* @brief 地面に付いているかどうかを設定する
	*/
	void SetGround(bool bGround);

	/**
	* @brief リタイアフラグをセットする
	*/
	void SetRetire(bool bRetire);

	/**
	* @brief 攻撃判定が出ているかのフラグをセットする
	* @param index - 攻撃対象プレイヤー番号
	* @param bAttack - フラグ
	*/
	void SetAttack(int enemyID, bool bAttack);

	/**
	* @brief モーションを設定する
	*/
	void SetMotion(int nMotion, bool bLoop, bool bForce);

private:
	/// オブジェクト
	SkinMeshPtr m_obj;

	/// プレイヤー番号
	unsigned int m_id;

	/// 相手の番号(攻撃してきたプレイヤー)
	int m_enemyID;

	/// 座標
	Vector3 m_pos;

	/// 前フレームの座標
	Vector3 m_oldPos;

	/// 速度
	Vector3 m_spd;

	/// 前フレームの速度
	Vector3 m_oldSpd;

	/// 行動
	Action m_action;

	/// 前フレームの行動
	Action m_oldAction;

	/// 向き
	Direction m_dir;

	/// 角度
	float m_angle;

	/// 大きさ
	float m_scale;

	/// サブワーク
	int m_subWork;

	/// モーション番号
	int m_nMotion;

	/// 攻撃力
	DWORD m_power;

	/// ジャンプ回数
	unsigned char m_jumpCount;

	/// 無敵判定
	bool m_isInvincible;

	/// 攻撃判定
	vector<bool> m_isAttack;

	/// 地面に付いているかどうか
	bool m_isGround;

	/// 操作フラグ
	bool m_isControl;

	/// リタイアフラグ
	bool m_isRetire;

	/// 吹き飛び硬直フラグ
	bool m_isBlowRigidity;

	/// 重力
	float m_gravity;

	/**
	* @brief アイドリング更新
	*/
	void UpdateIdling();

	/**
	* @brief 右移動更新
	*/
	void UpdateMoveRight();

	/**
	* @brief 左移動更新
	*/
	void UpdateMoveLeft();

	/**
	* @brief ジャンプ更新
	*/
	void UpdateJump();

	/**
	* @brief 行動更新
	*/
	void UpdateAction();

	/**
	* @brief 被弾
	*/
	void UpdateDamage();

	/**
	* @brief 通常攻撃
	*/
	void UpdateNormalAttack();

	/**
	* @brief スマッシュ攻撃
	*/
	void UpdateSmashAttack();

	/**
	* @brief 必殺攻撃(上)
	*/
	void UpdateSpecialAttackUP();

	/**
	* @brief アピール
	*/
	void UpdateAppeal();

	/**
	* @brief モーションの終了
	*/
	void EndMotion();

	/**
	* @brief パラメータをリセットする
	*/
	void ParamReset();
};

#endif