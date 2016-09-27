#include "SkyDome.h"
#include "..\..\DirectX\Direct3DData.h"
#include "..\..\Common\FileData.h"
/**
* @brief コンストラクタ
*/
SkyDome::SkyDome()
{
	// モデルの読み込み
	m_obj = FileData::GetInstance()->GetModel(0);

	// 座標の設定
	m_pos = Vector3();
	m_obj->SetPos(m_pos);

	// アンビエントの設定
	m_ambient = Vector4(0.8f, 0.8f, 0.8f, 1.0f);

	m_subWork = 0;

	// 角度を設定
	m_obj->SetAngle(135.0f);
	// 大きさの設定
	m_obj->SetScale(1.0f);
}

/**
* @brief デストラクタ
*/
SkyDome::~SkyDome()
{

}

/**
* @brief 更新
*/
bool SkyDome::Update()
{
	switch (m_subWork)
	{
		// 背景を赤色だけにする(夕焼けをイメージ)
	case 0:
		// 緑色を抜く
		m_ambient.y -= 0.0003f;
		// 青色を抜く
		m_ambient.z -= 0.0003f;
		// 色が抜けたら
		if (m_ambient.y < 0.0f &&
			m_ambient.z < 0.0f){
			m_subWork++;
		}
		break;

		// 背景を青色にする(夜をイメージ ※途中で紫色になる)
	case 1:
		// 赤色を抜く
		m_ambient.x -= 0.0003f;
		// 青色を足す
		m_ambient.z += 0.0003f;
		// 完全に青色になったら
		if (m_ambient.x < 0.0f &&
			m_ambient.z > 1.0f){
			m_subWork++;
		}
		break;

	case 2:
		// 何もしない
		break;
	}

	// 座標のセット
	m_obj->SetPos(m_pos);

	// スカイドーム更新
	m_obj->Update();

	return true;
}

/**
* @brief 描画
*/
void SkyDome::Render()
{
}

/**
* @brief 描画
*/
void SkyDome::Render(const Matrix& view, const Matrix& proj)
{
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_obj->RenderXMesh(view, proj);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
}

/**
* @brief ワールド行列を取得する
*/
const Matrix& SkyDome::GetWorld()
{
	return m_obj->GetTransMatrix();
}

/**
* @brief アンビエントを取得する
*/
const Vector4& SkyDome::GetAmbient()
{
	return m_ambient;
}

/**
* @brief メッシュの取得
*/
const MeshPtr& SkyDome::GetMesh() const
{
	return m_obj;
}
