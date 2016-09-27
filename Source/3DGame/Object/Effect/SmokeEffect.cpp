#include "SmokeEffect.h"

/**
* @brief コンストラクタ
* @param pos - 描画位置
* @param dir - 描画向き
*/
SmokeEffect::SmokeEffect(const Vector3& pos, const Matrix& view)
{
	// インスタンスの取得
	m_2dTex = TextureManager::GetInstance();
	
	// 位置を設定
	m_pos = pos;
	m_view = view;
	m_alpha = 255;
}


/**
* @brief デストラクタ
*/
SmokeEffect::~SmokeEffect()
{

}

/**
* @brief 更新
*/
bool SmokeEffect::Update()
{
	if (m_alpha < 0){
		return false;
	}

	m_alpha -= 33;

	return true;
}

/**
* @brief 描画
*/
void SmokeEffect::Render()
{
	m_2dTex->Draw3DTexture(m_view, SMOKE_EFFECT_PNG, m_pos, Vector3(), 0.005f, m_alpha);
}