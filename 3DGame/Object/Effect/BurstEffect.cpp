#include "BurstEffect.h"
#include "..\..\Common\FileData.h"

/**
* @brief �R���X�g���N�^
* @param pos - �`��ʒu
* @param dir - �`�����
*/
BurstEffect::BurstEffect(const Vector3& pos, const Matrix& view)
{
	// �ʒu��ݒ�
	m_pos = pos;
	m_pos.z = 12.5f;
	m_view = view;
	m_rot = Vector3();
	m_scale = 0.0f;
	m_alpha = 255;
}

/**
* @brief �X�V
*/
bool BurstEffect::Update()
{
	// �g�債�Ă���
	m_scale += 0.0003f;

	// �A���t�@�𔲂��Ă���
	m_alpha -= 5;

	if (m_alpha < 0){
		return false;
	}

	return true;
}

/**
* @brief �`��
*/
void BurstEffect::Render()
{
	TextureManager* tex = TextureManager::GetInstance();
	// �`��
	tex->Draw3DTexture(m_view, BURSTRING_EFFECT_BMP, m_pos, m_rot, m_scale, m_alpha, false);
}