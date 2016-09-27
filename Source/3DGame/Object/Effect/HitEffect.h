/**
* @file   HitEffect.h
* @brief  ヒットエフェクトを管理
* @author Syota Tachikawa
*/

#ifndef ___HITEFFECT_H
#define ___HITEFFECT_H

#include "..\IObject.h"
#include "..\..\Common\FileData.h"

/**
* @brief ヒットエフェクト
*/
class HitEffect : public IEffect
{
public:
	/**
	* @brief コンストラクタ
	* @param pos - 描画位置
	* @param view - 逆行列
	*/
	HitEffect(const Vector3& pos, const Matrix& view);

	/**
	* @brief デストラクタ
	*/
	~HitEffect(){};

	/**
	* @brief 更新
	*/
	bool Update();

	/**
	* @brief 描画
	*/
	void Render();

private:
	/// グラフィックスマネージャ
	TextureManager* m_2dTex;

	/// 座標
	Vector3 m_pos;

	/// 速度
	Vector3 m_spd;

	/// 角度
	Vector3 m_rot;

	float m_scale;

	Matrix m_view;

	int m_frame;

	/// アニメーションテクスチャ
	int m_tex[HITEFFECT_ANIM_MAX];

	/// アニメーション用
	int m_animation;

	/// 生存フレーム
	int m_liveFrame;
};

#endif