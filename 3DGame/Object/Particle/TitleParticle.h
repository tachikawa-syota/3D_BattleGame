/**
* @file   TitleParticle.h
* @brief  タイトル画面で使用するパーティクルの処理
* @author Syota Tachikawa
*/

#ifndef ___TITLEPARTICLE_H
#define ___TITLEPARTICLE_H

#include "..\..\Common\FileData.h"
#include "..\..\Common\STDXStruct.h"
#include "..\IObject.h"

/**
* @brief タイトル画面のパーティクル
*/
class TitleParticle : public IParticle
{
public:
	/**
	* @brief コンストラクタ
	*/
	TitleParticle(const Vector2& pos);

	/**
	* @brief デストラクタ
	*/
	~TitleParticle();

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
	Vector2 m_pos;

	/// 速度
	Vector2 m_spd;

	/// フレーム
	int m_frame;
};

#endif