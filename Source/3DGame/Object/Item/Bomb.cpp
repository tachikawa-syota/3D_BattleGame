#include "Bomb.h"
#include "..\..\Common\CommonParam.h"
#include "..\..\DirectX\Direct3DData.h"
#include "..\..\Common\FileData.h"

/**
* @brief �R���X�g���N�^
*/
Bomb::Bomb(const Vector3& pos)
{
	// ���f���̃��[�h(�N���[�����������̂��ߖ��񃍁[�h����E�E�E)
	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->LoadNonAnimMesh("Data/Model/Bomb/", "inseki.x");
	
	// �R�s�[
	m_obj = mesh;
	m_pos = pos;

	// ���W�̐ݒ�
	m_obj->SetPos(m_pos);
	// �傫���̐ݒ�
	m_obj->SetScale(0.6f);

	// ���x�̐ݒ�
	m_spd = Vector3(0.0f, GetComPrm->Gravity, 0.0f);

	m_gravity = 0.0f;

	m_isExplosion = false;
	m_isGround = false;
	m_id = -1;
}

/**
* @brief �f�X�g���N�^
*/
Bomb::~Bomb()
{

}

/**
* @brief �X�V
*/
bool Bomb::Update()
{
	// �ڒn���Ă����猸������������(�d�͂�����)
	if (m_isGround){
		m_gravity = 0.0f;
		m_spd.y = m_gravity;
		m_spd *= 0.7f;
	}
	// �d�͂�������
	else{
		m_gravity = GetComPrm->Gravity;
	}

	// ���W�ɑ��x�����Z����
	m_spd.y += m_gravity;
	m_pos += m_spd;

	// �X�V
	m_obj->SetPos(m_pos);
	m_obj->Update();

	// �X�V
	return true;
}

/**
* @brief �`��(�V�F�[�_�[���g�p����)
*/
void Bomb::Render(const Matrix& view, const Matrix& proj)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();
	// Z�o�b�t�@
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_obj->RenderXMesh(view, proj);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
}

/**
* @brief ���[���h�s����擾����
*/
const Matrix& Bomb::GetWorld()
{
	return m_obj->GetTransMatrix();
}

/**
* @brief ���W�̎擾
*/
const Vector3& Bomb::GetPos() const
{
	return m_pos;
}

/**
* @brief �_���[�W�t���O
*/
bool Bomb::IsDamage() const
{
	return m_isExplosion;
}

/**
* @brief �n�ʂɕt���Ă��邩�̃t���O��ݒ肷��
*/
void Bomb::SetGround(bool bFlag)
{
	m_isGround = bFlag;
}

/**
* @brief �_���[�W�t���O��ݒ肷��
* @param bFlag - �Z�b�g���邩�ۂ��̃t���O
* @param playerID - �v���C���[�̔ԍ�
*/
void Bomb::SetDamage(bool bFlag, unsigned int playerID)
{
	m_isExplosion = bFlag;
	m_id = playerID;
}

/**
* @brief ���W��ݒ肷��
*/
void Bomb::SetPos(const Vector3& pos)
{
	m_pos = pos;
}

/**
* @brief ���x��ݒ肷��
*/
void Bomb::SetSpeed(const Vector3& speed)
{
	m_spd = speed;
}

/**
* @brief ���b�V�����擾����
*/
const MeshPtr& Bomb::GetMesh() const
{
	return m_obj;
}

/**
* @brief �Ō�ɐG�ꂽ�v���C���[�ԍ����擾����
*/
const unsigned int Bomb::GetID() const
{
	return m_id;
}