#include "ExplosionSmokeEffect.h"
#include "..\..\Common\FileData.h"

/**
* @brief �R���X�g���N�^
* @param pos - �ʒu
* @param view - �r���[�s��
*/
ExplosionSmokeEffect::ExplosionSmokeEffect(const Vector3& pos, const Matrix& view)
{
	// �ʒu��ݒ�
	m_pos = pos;
	m_view = view;
	m_rot = Vector3();
	m_scale = 0.0f;
	m_alpha = 255;
}

/**
* @brief �X�V
*/
bool ExplosionSmokeEffect::Update()
{
	// �g�債�Ă���
	m_scale += 0.0004f;

	// �A���t�@�𔲂��Ă���
	m_alpha -= 2;

	if (m_alpha < 0){
		return false;
	}

	return true;
}

/**
* @brief �`��
*/
void ExplosionSmokeEffect::Render()
{
	TextureManager* tex = TextureManager::GetInstance();
	// �`��
	tex->Draw3DTexture(m_view, EXPLOSION_SMOKE_EFFECT_PNG, m_pos, m_rot, m_scale, m_alpha, false);
}