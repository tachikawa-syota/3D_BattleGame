/**
* @file   SnowParticle.h
* @brief  ヒットパーティクルの処理
* @author Syota Tachikawa
*/

#ifndef ___SNOWPARTICLE_H
#define ___SNOWPARTICLE_H

#include "..\..\Common\FileData.h"
#include "..\IObject.h"

/**
* @brief 雪パーティクルクラス
*/
class SnowParticle : public IParticle
{
public:
	/**
	* @brief コンストラクタ
	*/
	SnowParticle(const Vector3& pos, const Matrix& view);

	/**
	* @brief デストラクタ
	*/
	~SnowParticle();

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

	/// フレーム
	int m_frame;
};

#endif