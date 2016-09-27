#include "Stage1.h"
#include "..\..\DirectX\Direct3DData.h"
#include "..\..\Common\FileData.h"

/**
* @brief コンストラクタ
*/
Stage1::Stage1()
{
	// インスタンスを取得
	FileData* file = FileData::GetInstance();

	// モデルの読み込み
	m_stage = file->GetModel(1);
	// 座標の設定
	m_stage->SetPos(Vector3());
	// 大きさの設定
	m_stage->SetScale(1.0f);
}

/**
* @brief デストラクタ
*/
Stage1::~Stage1()
{
}

/**
* @brief 更新
*/
bool Stage1::Update()
{
	// ステージの更新
	m_stage->Update();

	return true;
}

/**
* @brief 描画
*/
void Stage1::Render(const Matrix& view, const Matrix& proj)
{
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_stage->RenderXMesh(view, proj);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
}

/**
* @brief 座標の取得
*/
const Vector3& Stage1::GetPos() const
{
	return m_pos;
}

/**
* @brief メッシュの取得
*/
const MeshPtr& Stage1::GetMesh() const
{
	return m_stage;
}