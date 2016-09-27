#include "Stage1.h"
#include "..\..\DirectX\Direct3DData.h"
#include "..\..\Common\FileData.h"

/**
* @brief �R���X�g���N�^
*/
Stage1::Stage1()
{
	// �C���X�^���X���擾
	FileData* file = FileData::GetInstance();

	// ���f���̓ǂݍ���
	m_stage = file->GetModel(1);
	// ���W�̐ݒ�
	m_stage->SetPos(Vector3());
	// �傫���̐ݒ�
	m_stage->SetScale(1.0f);
}

/**
* @brief �f�X�g���N�^
*/
Stage1::~Stage1()
{
}

/**
* @brief �X�V
*/
bool Stage1::Update()
{
	// �X�e�[�W�̍X�V
	m_stage->Update();

	return true;
}

/**
* @brief �`��
*/
void Stage1::Render(const Matrix& view, const Matrix& proj)
{
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_stage->RenderXMesh(view, proj);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
}

/**
* @brief ���W�̎擾
*/
const Vector3& Stage1::GetPos() const
{
	return m_pos;
}

/**
* @brief ���b�V���̎擾
*/
const MeshPtr& Stage1::GetMesh() const
{
	return m_stage;
}