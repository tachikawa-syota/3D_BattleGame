/**
* @file   DeadEffect.h
* @brief  死亡エフェクトを管理する
* @author Syota Tachikawa
*/

#ifndef ___DEADEFFECT_H
#define ___DEADEFFECT_H

#include "..\IObject.h"
#include "..\..\Common\STDXStruct.h"
#include "..\..\Common\FileData.h"

/**
* @brief 死亡エフェクト
*/
class DeadEffect : public IEffect
{
public:
	/**
	* @brief コンストラクタ
	* @param pos - 描画位置
	*/
	DeadEffect(int type);

	/**
	* @brief デストラクタ
	*/
	~DeadEffect();

	/**
	* @brief 更新
	*/
	bool Update();

	/**
	* @brief 描画
	*/
	void Render();

private:
	/// テクスチャ	
	int m_tex[DEADEFFECT_TEXTURE_MAX];

	/// アニメーション用
	int m_animation;

	/// 座標
	Vector2 m_pos;

	/// 角度
	float m_rot;

	/// フレーム
	int m_frame;
};

#endif