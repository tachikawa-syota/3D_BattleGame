/**
* @file   BurstEffect.h
* @brief  衝撃波エフェクトを管理
* @author Syota Tachikawa
*/

#ifndef ___BURSTEFFECT_H
#define ___BURSTEFFECT_H

#include "..\IObject.h"

/**
* @brief 衝撃波エフェクトクラス
*/
class BurstEffect : public IEffect
{
public:
	/**
	* @brief コンストラクタ
	* @param pos - 描画位置
	* @param view - ビュー行列
	*/
	BurstEffect(const Vector3& pos, const Matrix& view);

	/**
	* @brief デストラクタ
	*/
	~BurstEffect(){};

	/**
	* @brief 更新
	*/
	bool Update();

	/**
	* @brief 描画
	*/
	void Render();

private:
	/// 座標
	Vector3 m_pos;

	/// 速度
	Vector3 m_spd;

	/// 角度
	Vector3 m_rot;

	/// 大きさ
	float m_scale;

	/// アルファ値
	int m_alpha;

	/// ビュー行列
	Matrix m_view;

};

#endif