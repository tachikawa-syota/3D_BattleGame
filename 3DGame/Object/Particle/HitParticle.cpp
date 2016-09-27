#include "HitParticle.h"
#include "..\..\Common\Utility.h"

/**
* @brief �R���X�g���N�^
*/
HitParticle::HitParticle(const Vector3& pos, const Matrix& view)
{
	// �p�����[�^���Z�b�g����
	m_pos = pos;
	m_pos.z = 12.5f;
	m_view = view;

	// ���������Ă���
	m_spd = Vector3();
	m_spd.x = Utility::Random(-0.1f, 0.1f);
	m_spd.y = Utility::Random(-0.1f, 0.1f);

	// �C���X�^���X���擾
	m_2dTex = TextureManager::GetInstance();

	m_alpha = 255;
}

/**
* @brief �f�X�g���N�^
*/
HitParticle::~HitParticle()
{

}

/**
* @brief �X�V
* @return true - �X�V �F false - �I�u�W�F�N�g�폜
*/
bool HitParticle::Update()
{
	if (m_alpha < 0){
		return false;
	}

	// �A���t�@�l�𔲂�
	m_alpha -= 4;

	// ����������
	m_spd *= 0.97f;

	// ���x���Z
	m_pos += m_spd;

	return true;
}

/**
* @brief �`��
*/
void HitParticle::Render()
{
	m_2dTex->Draw3DTexture(m_view, FIRE2_PARTICLE_PNG, m_pos, Vector3(), 0.004f, m_alpha);
}