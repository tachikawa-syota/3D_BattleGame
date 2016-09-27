#include "Tree.h"
#include "..\..\DirectX\Direct3DData.h"
#include "..\..\Common\FileData.h"

/**
* @brief コンストラクタ
*/
Tree::Tree(const Vector3& pos)
{
	// インスタンスを取得する
	FileData* file = FileData::GetInstance();

	m_pos = pos;

	// モデルの読み込み
	for (int i = 0; i < TREE_OBJECT_MAX; i++)
	{
		m_obj = file->GetModel(2);
		m_obj->SetPos(m_pos);
		m_obj->SetScale(1.5f);
		m_obj->SetAngle(90.0f);
	}
}

/**
* @brief デストラクタ
*/
Tree::~Tree()
{

}

/**
* @brief 更新
*/
bool Tree::Update()
{
	m_obj->SetPos(m_pos);
	m_obj->Update();

	return true;
}

/**
* @brief 描画
*/
void Tree::Render(const Matrix& view, const Matrix& proj)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();
	// Zバッファ
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	for (int i = 0; i < TREE_OBJECT_MAX; i++){
		m_obj->RenderXMesh(view, proj);
	}
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
}

/**
* @brief メッシュ情報を取得する
*/
const MeshPtr& Tree::GetMesh() const
{
	return m_obj;
}
