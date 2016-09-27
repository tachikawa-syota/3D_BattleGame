#include "SmokeEffect.h"

/**
* @brief �R���X�g���N�^
* @param pos - �`��ʒu
* @param dir - �`�����
*/
SmokeEffect::SmokeEffect(const Vector3& pos, const Matrix& view)
{
	// �C���X�^���X�̎擾
	m_2dTex = TextureManager::GetInstance();
	
	// �ʒu��ݒ�
	m_pos = pos;
	m_view = view;
	m_alpha = 255;
}


/**
* @brief �f�X�g���N�^
*/
SmokeEffect::~SmokeEffect()
{

}

/**
* @brief �X�V
*/
bool SmokeEffect::Update()
{
	if (m_alpha < 0){
		return false;
	}

	m_alpha -= 33;

	return true;
}

/**
* @brief �`��
*/
void SmokeEffect::Render()
{
	m_2dTex->Draw3DTexture(m_view, SMOKE_EFFECT_PNG, m_pos, Vector3(), 0.005f, m_alpha);
}