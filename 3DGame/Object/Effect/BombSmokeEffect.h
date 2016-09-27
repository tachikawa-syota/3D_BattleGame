/**
* @file   BombSmokeEffect.h
* @brief  煙エフェクトの処理
* @author Syota Tachikawa
*/

#ifndef ___BOMBSMOKEEFFECT_H
#define ___BOMBSMOKEEFFECT_H

#include "..\IObject.h"

/**
* @brief ボムから出る煙エフェクトクラス
*/
class BombSmokeEffect : public IEffect
{
public:
	/**
	* @brief コンストラクタ
	*/
	BombSmokeEffect(const Vector3& pos, const Matrix& view);

	/**
	* @brief デストラクタ
	*/
	~BombSmokeEffect();

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