#include "HitEffect.h"

/**
* @brief コンストラクタ
* @param pos - 描画位置
* @param dir - 描画向き
*/
HitEffect::HitEffect(const Vector3& pos, const Matrix& view)
{
	// インスタンスの取得
	m_2dTex = TextureManager::GetInstance();
	
	// テクスチャコピー
	for (int i = 0; i < HITEFFECT_ANIM_MAX; i++){
		m_tex[i] = FileData::GetInstance()->GetHitEffectTexture(i);
	}
	
	// 位置を設定
	m_pos = pos;
	m_pos.z = 12.5f;
	m_view = view;
	m_rot = Vector3();
	m_scale = 0.04f;

	m_animation = 0;
	m_frame = 0;

	m_liveFrame = 10;
}

/**
* @brief 更新
*/
bool HitEffect::Update()
{
	// フレーム
	m_frame++;

	if (m_frame % 5 == 0){
		m_animation++;
	}
	if (m_animation >= HITEFFECT_ANIM_MAX){
		m_animation = 0;
	}
	// 生存フレームを減産
	m_liveFrame--;

	if (m_liveFrame < 0){
		return false;
	}

	return true;
}

/**
* @brief 描画
*/
void HitEffect::Render()
{
	m_2dTex->Draw3DTexture(m_view, m_tex[m_animation], m_pos, m_rot, m_scale, 255);
}