#include "BombSmokeEffect.h"
#include "..\..\Common\FileData.h"
#include "..\..\Common\Utility.h"
#include "..\..\Common\CommonParam.h"

/**
* @brief コンストラクタ
* @param pos - 描画位置
* @param dir - 描画向き
*/
BombSmokeEffect::BombSmokeEffect(const Vector3& pos, const Matrix& view)
{
	// 位置を設定
	m_pos = pos;
	m_pos.z = 12.5f;
	m_view = view;
	m_alpha = 255;

	// 速度の初期化
	m_spd = Vector3();
	m_spd.x = Utility::Random(-0.4f, 0.4f);
	m_spd.y = Utility::Random(-0.4f, 0.4f);
}


/**
* @brief デストラクタ
*/
BombSmokeEffect::~BombSmokeEffect()
{

}

/**
* @brief 更新
*/
bool BombSmokeEffect::Update()
{
	// アルファ値が"0"以下で削除
	if (m_alpha < 0) return false;

	// アルファ値を抜いていく
	m_alpha -= 23;

	// 座標に速度を加算する
	m_pos += m_spd;
	m_spd *= 0.76f;

	return true;
}

/**
* @brief 描画
*/
void BombSmokeEffect::Render()
{
	// インスタンスの取得
	TextureManager* m_2dTex = TextureManager::GetInstance();

	m_2dTex->Draw3DTexture(m_view, BOMB_SMOKE_EFFECT_PNG, m_pos, Vector3(), 0.05f, m_alpha);
}