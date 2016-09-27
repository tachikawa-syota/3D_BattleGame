#include "Tree.h"
#include "..\..\DirectX\Direct3DData.h"
#include "..\..\Common\FileData.h"

/**
* @brief �R���X�g���N�^
*/
Tree::Tree(const Vector3& pos)
{
	// �C���X�^���X���擾����
	FileData* file = FileData::GetInstance();

	m_pos = pos;

	// ���f���̓ǂݍ���
	for (int i = 0; i < TREE_OBJECT_MAX; i++)
	{
		m_obj = file->GetModel(2);
		m_obj->SetPos(m_pos);
		m_obj->SetScale(1.5f);
		m_obj->SetAngle(90.0f);
	}
}

/**
* @brief �f�X�g���N�^
*/
Tree::~Tree()
{

}

/**
* @brief �X�V
*/
bool Tree::Update()
{
	m_obj->SetPos(m_pos);
	m_obj->Update();

	return true;
}

/**
* @brief �`��
*/
void Tree::Render(const Matrix& view, const Matrix& proj)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();
	// Z�o�b�t�@
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	for (int i = 0; i < TREE_OBJECT_MAX; i++){
		m_obj->RenderXMesh(view, proj);
	}
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
}

/**
* @brief ���b�V�������擾����
*/
const MeshPtr& Tree::GetMesh() const
{
	return m_obj;
}
