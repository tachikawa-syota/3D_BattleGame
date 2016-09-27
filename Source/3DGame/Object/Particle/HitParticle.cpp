#include "HitParticle.h"
#include "..\..\Common\Utility.h"

/**
* @brief コンストラクタ
*/
HitParticle::HitParticle(const Vector3& pos, const Matrix& view)
{
	// パラメータをセットする
	m_pos = pos;
	m_pos.z = 12.5f;
	m_view = view;

	// 初期化しておく
	m_spd = Vector3();
	m_spd.x = Utility::Random(-0.1f, 0.1f);
	m_spd.y = Utility::Random(-0.1f, 0.1f);

	// インスタンスを取得
	m_2dTex = TextureManager::GetInstance();

	m_alpha = 255;
}

/**
* @brief デストラクタ
*/
HitParticle::~HitParticle()
{

}

/**
* @brief 更新
* @return true - 更新 ： false - オブジェクト削除
*/
bool HitParticle::Update()
{
	if (m_alpha < 0){
		return false;
	}

	// アルファ値を抜く
	m_alpha -= 4;

	// 減速させる
	m_spd *= 0.97f;

	// 速度加算
	m_pos += m_spd;

	return true;
}

/**
* @brief 描画
*/
void HitParticle::Render()
{
	m_2dTex->Draw3DTexture(m_view, FIRE2_PARTICLE_PNG, m_pos, Vector3(), 0.004f, m_alpha);
}