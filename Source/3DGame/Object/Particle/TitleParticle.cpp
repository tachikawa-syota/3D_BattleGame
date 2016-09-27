#include "TitleParticle.h"
#include "..\..\Common\Utility.h"

TitleParticle::TitleParticle(const Vector2& pos)
{
	// �p�����[�^���Z�b�g����
	m_pos = pos;

	m_spd = Vector2(0.0f, -1.6f);

	// �C���X�^���X���擾
	m_2dTex = TextureManager::GetInstance();

	m_frame = 0;
}

TitleParticle::~TitleParticle()
{

}

/**
* @brief �X�V
* @return true - �X�V �F false - �I�u�W�F�N�g�폜
*/
bool TitleParticle::Update()
{
	// �␳�l
	float value = 0.03f;

	if (m_frame % 20 == 0){
		m_spd.x = Utility::Random(-1.5f, 1.5f);
		m_spd.y = m_spd.y + value;
	}

	// ���W�ɑ��x�����Z����
	m_pos += m_spd;

	if (m_pos.y < -2.0f){
		return false;
	}

	// �t���[�������Z
	m_frame++;

	return true;
}

/**
* @brief �`��
*/
void TitleParticle::Render()
{
	m_2dTex->DrawTexture(FIRE_PARTICLE_PNG, m_pos.x, m_pos.y, 0.0f, 0.4f, 255, true);
}