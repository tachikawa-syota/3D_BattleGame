#include "DeadEffect.h"
#include "..\..\DirectX\TextureManager.h"

/**
* @brief コンストラクタ
* @param pos - 描画位置
* @param dir - 描画向き
*/
DeadEffect::DeadEffect(int type)
{
	// テクスチャのコピー
	for (int i = 0; i < DEADEFFECT_TEXTURE_MAX; i++){
		m_tex[i] = FileData::GetInstance()->GetDeadEffectTexture(i);
	}

	// パラメータのセット
	switch (type)
	{
	// 下
	case 0:
		// 座標はのちのち調整
		m_pos = Vector2(640.0f, 720.0f);
		// 角度とかはのちのち調整する
		m_rot = -90.0f;
		break;

	// 左
	case 1:
		// 座標はのちのち調整
		m_pos = Vector2(0.0f, 360.0f);
		// 角度とかはのちのち調整する
		m_rot = 0.0f;
		break;

	// 右
	case 2:
		// 座標はのちのち調整
		m_pos = Vector2(1280.0f, 360.0f);
		// 角度とかはのちのち調整する
		m_rot = 180.0f;
		break;
	}

	m_animation = 0;
	m_frame = 0;
}

/**
* @brief デストラクタ
*/
DeadEffect::~DeadEffect()
{
	m_animation = 0;
	m_frame = 0;
}

/**
* @brief 更新
*/
bool DeadEffect::Update()
{
	// アニメーションを回す
	if (m_frame % 5 == 0){
		m_animation++;
	}

	// アニメーションが終了したら削除
	if (m_animation >= DEADEFFECT_TEXTURE_MAX){
		return false;
	}

	// フレームの更新
	m_frame++;

	return true;
}

/**
* @brief 描画
*/
void DeadEffect::Render()
{
	TextureManager* tex = TextureManager::GetInstance();

	tex->DrawTexture(m_tex[m_animation], m_pos.x, m_pos.y, m_rot);
}