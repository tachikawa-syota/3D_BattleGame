#include "Collision.h"


Collision::~Collision()
{

}

/**
* @brief ステージのメッシュを設定する
* @param ステージのメッシュ情報
*/
void Collision::SetStageObj(MeshPtr obj)
{
	m_stage = obj; 
}

/**
* @brief 床との当たり判定
* @param プレイヤーの座標
*/
float Collision::GetHeight(const Vector3& pos)
{
	Vector3	out;
	Vector3	p = pos + Vector3(0, 2.0f, 0);
	Vector3 v(0, -1, 0);
	float d = 2.0f;
	// レイピック
	int index = m_stage->RayPick(&out, &p, &v, &d);
	if (index < 0){
		return pos.y;
	}
	return out.y;
}

/**
* @brief 球体同士の当たり判定
* @param pos - オブジェクトの座標
* @param radius - オブジェクトの半径
*/
bool Collision::CheckShpere(const Vector3& posA, float radiusA, const Vector3& posB, float radiusB)
{
	// 最初は当たっていないをセットする
	bool isHit = false;

	// 距離
	Vector3 v = posB - posA;
	float	d = v.Length();
	// 距離　< 当て半径 + 受け半径
	if (d < radiusA + radiusB){
		isHit = true;
	}

	return isHit;
}
