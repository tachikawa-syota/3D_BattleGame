#include "HitEffect.h"

/**
* @brief �R���X�g���N�^
* @param pos - �`��ʒu
* @param dir - �`�����
*/
HitEffect::HitEffect(const Vector3& pos, const Matrix& view)
{
	// �C���X�^���X�̎擾
	m_2dTex = TextureManager::GetInstance();
	
	// �e�N�X�`���R�s�[
	for (int i = 0; i < HITEFFECT_ANIM_MAX; i++){
		m_tex[i] = FileData::GetInstance()->GetHitEffectTexture(i);
	}
	
	// �ʒu��ݒ�
	m_pos = pos;
	m_pos.z = 12.5f;
	m_view = view;
	m_rot = Vector3();
	m_scale = 0.04f;

	m_animation = 0;
	m_frame = 0;

	m_liveFrame = 10;
}

/**
* @brief �X�V
*/
bool HitEffect::Update()
{
	// �t���[��
	m_frame++;

	if (m_frame % 5 == 0){
		m_animation++;
	}
	if (m_animation >= HITEFFECT_ANIM_MAX){
		m_animation = 0;
	}
	// �����t���[�������Y
	m_liveFrame--;

	if (m_liveFrame < 0){
		return false;
	}

	return true;
}

/**
* @brief �`��
*/
void HitEffect::Render()
{
	m_2dTex->Draw3DTexture(m_view, m_tex[m_animation], m_pos, m_rot, m_scale, 255);
}