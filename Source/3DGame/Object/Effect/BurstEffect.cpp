#include "BurstEffect.h"
#include "..\..\Common\FileData.h"

/**
* @brief コンストラクタ
* @param pos - 描画位置
* @param dir - 描画向き
*/
BurstEffect::BurstEffect(const Vector3& pos, const Matrix& view)
{
	// 位置を設定
	m_pos = pos;
	m_pos.z = 12.5f;
	m_view = view;
	m_rot = Vector3();
	m_scale = 0.0f;
	m_alpha = 255;
}

/**
* @brief 更新
*/
bool BurstEffect::Update()
{
	// 拡大していく
	m_scale += 0.0003f;

	// アルファを抜いていく
	m_alpha -= 5;

	if (m_alpha < 0){
		return false;
	}

	return true;
}

/**
* @brief 描画
*/
void BurstEffect::Render()
{
	TextureManager* tex = TextureManager::GetInstance();
	// 描画
	tex->Draw3DTexture(m_view, BURSTRING_EFFECT_BMP, m_pos, m_rot, m_scale, m_alpha, false);
}