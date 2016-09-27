#include "TitleParticle.h"
#include "..\..\Common\Utility.h"

TitleParticle::TitleParticle(const Vector2& pos)
{
	// パラメータをセットする
	m_pos = pos;

	m_spd = Vector2(0.0f, -1.6f);

	// インスタンスを取得
	m_2dTex = TextureManager::GetInstance();

	m_frame = 0;
}

TitleParticle::~TitleParticle()
{

}

/**
* @brief 更新
* @return true - 更新 ： false - オブジェクト削除
*/
bool TitleParticle::Update()
{
	// 補正値
	float value = 0.03f;

	if (m_frame % 20 == 0){
		m_spd.x = Utility::Random(-1.5f, 1.5f);
		m_spd.y = m_spd.y + value;
	}

	// 座標に速度を加算する
	m_pos += m_spd;

	if (m_pos.y < -2.0f){
		return false;
	}

	// フレームを加算
	m_frame++;

	return true;
}

/**
* @brief 描画
*/
void TitleParticle::Render()
{
	m_2dTex->DrawTexture(FIRE_PARTICLE_PNG, m_pos.x, m_pos.y, 0.0f, 0.4f, 255, true);
}