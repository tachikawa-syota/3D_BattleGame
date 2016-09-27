/**
* @file   Bomb.h
* @brief  アイテムボムの処理を管理する
* @author Syota Tachikawa
*/

#ifndef ___BOMB_H
#define ___BOMB_H

#include "..\IObject.h"

/**
* @brief ボムアイテムクラス
*/
class Bomb : public IItem
{
public:
	/**
	* @brief コンストラクタ
	*/
	Bomb(const Vector3& pos);

	/**
	* @brief デストラクタ
	*/
	~Bomb();

	/**
	* @brief 更新
	*/
	bool Update();

	/**
	* @brief 描画(シェーダーを使用する)
	*/
	void Render(const Matrix& view, const Matrix& proj);

	/**
	* @brief ワールド行列を取得する
	*/
	const Matrix& GetWorld();

	/**
	* @brief 座標の取得
	*/
	const Vector3& GetPos() const;

	/**
	* @brief ダメージフラグ
	*/
	bool IsDamage() const;

	/**
	* @brief 地面に付いているかのフラグを設定する
	*/
	void SetGround(bool bFlag);

	/**
	* @brief ダメージフラグを設定する
	* @param bFlag - セットするか否かのフラグ
	* @param playerID - プレイヤーの番号
	*/
	void SetDamage(bool bFlag, unsigned int playerID);

	/**
	* @brief 座標を設定する
	*/
	void SetPos(const Vector3& pos);

	/**
	* @brief 速度を設定する
	*/
	void SetSpeed(const Vector3& speed);

	/**
	* @brief メッシュを取得する
	*/
	const MeshPtr& GetMesh() const;

	/**
	* @brief 最後に触れたプレイヤー番号を取得する
	*/
	const unsigned int GetID() const;

private:
	/// オブジェクト
	MeshPtr m_obj;

	/// 座標
	Vector3 m_pos;

	/// 速度
	Vector3 m_spd;

	/// 重力
	float m_gravity;

	/// 爆発フラグ
	bool m_isExplosion;

	/// 接地フラグ
	bool m_isGround;

	/// 番号
	unsigned int m_id;
};

#endif