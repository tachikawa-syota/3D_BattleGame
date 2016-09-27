/**
* @file   HitParticle.h
* @brief  ヒットパーティクルの処理
* @author Syota Tachikawa
*/

#ifndef ___HITPARTICLE_H
#define ___HITPARTICLE_H

#include "..\..\Common\FileData.h"
#include "..\IObject.h"

/**
* @brief ヒットパーティクルクラス
*/
class HitParticle : public IParticle
{
public:
	/**
	* @brief コンストラクタ
	*/
	HitParticle(const Vector3& pos, const Matrix& view);

	/**
	* @brief デストラクタ
	*/
	~HitParticle();

	/**
	* @brief 更新
	* @return true - 更新 ： false - オブジェクト削除
	*/
	bool Update();

	/**
	* @brief 描画
	*/
	void Render();

private:
	/// テクスチャマネージャ
	TextureManager* m_2dTex;

	/// 座標
	Vector3 m_pos;

	/// 速度
	Vector3 m_spd;

	/// 逆行列
	Matrix m_view;

	/// アルファ値
	int m_alpha;
};

#endif