#include "SnowParticle.h"
#include "..\..\Common\Utility.h"

/**
* @brief コンストラクタ
*/
SnowParticle::SnowParticle(const Vector3& pos, const Matrix& view)
{
	// パラメータをセットする
	m_pos = pos;
	m_view = view;

	m_spd = Vector3();
	m_spd.x = -0.05f;

	// インスタンスを取得
	m_2dTex = TextureManager::GetInstance();

	m_frame = 0;
}

/**
* @brief デストラクタ
*/
SnowParticle::~SnowParticle()
{

}

/**
* @brief 更新
* @return true - 更新 ： false - オブジェクト削除
*/
bool SnowParticle::Update()
{
	// フィールド外
	if (m_pos.x >= 13.0f || m_pos.x <= -13.0f)
	{
		if (m_pos.y <= -25.0f){
			return false;
		}

		if (m_pos.x <= -37.0f){
			return false;
		}
	}
	else if (m_pos.y <= 0){
		return false;
	}

	// 速度加算
	m_spd.x += 0.0001f;
	m_spd.y = -0.14f;

	m_pos += m_spd;

	// フレーム加算
	m_frame++;

	return true;
}

/**
* @brief 描画
*/
void SnowParticle::Render()
{
	m_2dTex->Draw3DTexture(m_view, SNOW_PARTICLE_PNG, m_pos, Vector3(), 0.0005f, 255, false);
}