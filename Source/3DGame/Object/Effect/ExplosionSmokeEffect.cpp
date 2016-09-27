#include "ExplosionSmokeEffect.h"
#include "..\..\Common\FileData.h"

/**
* @brief コンストラクタ
* @param pos - 位置
* @param view - ビュー行列
*/
ExplosionSmokeEffect::ExplosionSmokeEffect(const Vector3& pos, const Matrix& view)
{
	// 位置を設定
	m_pos = pos;
	m_view = view;
	m_rot = Vector3();
	m_scale = 0.0f;
	m_alpha = 255;
}

/**
* @brief 更新
*/
bool ExplosionSmokeEffect::Update()
{
	// 拡大していく
	m_scale += 0.0004f;

	// アルファを抜いていく
	m_alpha -= 2;

	if (m_alpha < 0){
		return false;
	}

	return true;
}

/**
* @brief 描画
*/
void ExplosionSmokeEffect::Render()
{
	TextureManager* tex = TextureManager::GetInstance();
	// 描画
	tex->Draw3DTexture(m_view, EXPLOSION_SMOKE_EFFECT_PNG, m_pos, m_rot, m_scale, m_alpha, false);
}