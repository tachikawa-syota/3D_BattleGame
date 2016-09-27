#include "SnowParticle.h"
#include "..\..\Common\Utility.h"

/**
* @brief �R���X�g���N�^
*/
SnowParticle::SnowParticle(const Vector3& pos, const Matrix& view)
{
	// �p�����[�^���Z�b�g����
	m_pos = pos;
	m_view = view;

	m_spd = Vector3();
	m_spd.x = -0.05f;

	// �C���X�^���X���擾
	m_2dTex = TextureManager::GetInstance();

	m_frame = 0;
}

/**
* @brief �f�X�g���N�^
*/
SnowParticle::~SnowParticle()
{

}

/**
* @brief �X�V
* @return true - �X�V �F false - �I�u�W�F�N�g�폜
*/
bool SnowParticle::Update()
{
	// �t�B�[���h�O
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

	// ���x���Z
	m_spd.x += 0.0001f;
	m_spd.y = -0.14f;

	m_pos += m_spd;

	// �t���[�����Z
	m_frame++;

	return true;
}

/**
* @brief �`��
*/
void SnowParticle::Render()
{
	m_2dTex->Draw3DTexture(m_view, SNOW_PARTICLE_PNG, m_pos, Vector3(), 0.0005f, 255, false);
}