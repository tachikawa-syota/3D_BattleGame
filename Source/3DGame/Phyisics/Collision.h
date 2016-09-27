/**
* @file   Collision.h
* @brief  当たり判定を管理する
* @author Syota Tachikawa
*/

#ifndef ___COLLISION_H
#define ___COLLISION_H

#include "..\DirectX\Mesh.h"

/**
* @brief 当たり判定クラス
*/
class Collision
{
public:
	/**
	* @brief ゲットインスタンス
	*/
	static Collision* GetInstance(){
		static Collision singleton;
		return &singleton;
	}

	/**
	* @brief デストラクタ
	*/
	~Collision();

	/**
	* @brief ステージのメッシュを設定する
	* @param ステージのメッシュ情報
	*/
	void SetStageObj(MeshPtr obj);

	/**
	* @brief 床との当たり判定
	* @param プレイヤーの座標
	*/
	float GetHeight(const Vector3& pos);

	/**
	* @brief 球体同士の当たり判定
	* @param pos - オブジェクトの座標
	* @param radius - オブジェクトの半径
	*/
	bool CheckShpere(const Vector3& posA, float radiusA, const Vector3& posB, float radiusB);

private:
	/// ステージ
	MeshPtr m_stage;
};

#endif