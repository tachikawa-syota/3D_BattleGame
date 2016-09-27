/**
* @file   SmokeEffect.h
* @brief  煙エフェクトの処理
* @author Syota Tachikawa
*/

#ifndef ___SMOKEEFFECT_H
#define ___SMOKEEFFECT_H

#include "..\..\Common\FileData.h"
#include "..\IObject.h"

/**
* @brief 煙エフェクトクラス
*/
class SmokeEffect : public IEffect
{
public:
	/**
	* @brief コンストラクタ
	*/
	SmokeEffect(const Vector3& pos, const Matrix& view);

	/**
	* @brief デストラクタ
	*/
	~SmokeEffect();

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

	/// ビュー
	Matrix m_view;

	/// アルファ値
	int m_alpha;
};

#endif