#include "SkyDome.h"
#include "..\..\DirectX\Direct3DData.h"
#include "..\..\Common\FileData.h"
/**
* @brief �R���X�g���N�^
*/
SkyDome::SkyDome()
{
	// ���f���̓ǂݍ���
	m_obj = FileData::GetInstance()->GetModel(0);

	// ���W�̐ݒ�
	m_pos = Vector3();
	m_obj->SetPos(m_pos);

	// �A���r�G���g�̐ݒ�
	m_ambient = Vector4(0.8f, 0.8f, 0.8f, 1.0f);

	m_subWork = 0;

	// �p�x��ݒ�
	m_obj->SetAngle(135.0f);
	// �傫���̐ݒ�
	m_obj->SetScale(1.0f);
}

/**
* @brief �f�X�g���N�^
*/
SkyDome::~SkyDome()
{

}

/**
* @brief �X�V
*/
bool SkyDome::Update()
{
	switch (m_subWork)
	{
		// �w�i��ԐF�����ɂ���(�[�Ă����C���[�W)
	case 0:
		// �ΐF�𔲂�
		m_ambient.y -= 0.0003f;
		// �F�𔲂�
		m_ambient.z -= 0.0003f;
		// �F����������
		if (m_ambient.y < 0.0f &&
			m_ambient.z < 0.0f){
			m_subWork++;
		}
		break;

		// �w�i��F�ɂ���(����C���[�W ���r���Ŏ��F�ɂȂ�)
	case 1:
		// �ԐF�𔲂�
		m_ambient.x -= 0.0003f;
		// �F�𑫂�
		m_ambient.z += 0.0003f;
		// ���S�ɐF�ɂȂ�����
		if (m_ambient.x < 0.0f &&
			m_ambient.z > 1.0f){
			m_subWork++;
		}
		break;

	case 2:
		// �������Ȃ�
		break;
	}

	// ���W�̃Z�b�g
	m_obj->SetPos(m_pos);

	// �X�J�C�h�[���X�V
	m_obj->Update();

	return true;
}

/**
* @brief �`��
*/
void SkyDome::Render()
{
}

/**
* @brief �`��
*/
void SkyDome::Render(const Matrix& view, const Matrix& proj)
{
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_obj->RenderXMesh(view, proj);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
}

/**
* @brief ���[���h�s����擾����
*/
const Matrix& SkyDome::GetWorld()
{
	return m_obj->GetTransMatrix();
}

/**
* @brief �A���r�G���g���擾����
*/
const Vector4& SkyDome::GetAmbient()
{
	return m_ambient;
}

/**
* @brief ���b�V���̎擾
*/
const MeshPtr& SkyDome::GetMesh() const
{
	return m_obj;
}
