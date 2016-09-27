#include "DeadEffect.h"
#include "..\..\DirectX\TextureManager.h"

/**
* @brief �R���X�g���N�^
* @param pos - �`��ʒu
* @param dir - �`�����
*/
DeadEffect::DeadEffect(int type)
{
	// �e�N�X�`���̃R�s�[
	for (int i = 0; i < DEADEFFECT_TEXTURE_MAX; i++){
		m_tex[i] = FileData::GetInstance()->GetDeadEffectTexture(i);
	}

	// �p�����[�^�̃Z�b�g
	switch (type)
	{
	// ��
	case 0:
		// ���W�͂̂��̂�����
		m_pos = Vector2(640.0f, 720.0f);
		// �p�x�Ƃ��͂̂��̂���������
		m_rot = -90.0f;
		break;

	// ��
	case 1:
		// ���W�͂̂��̂�����
		m_pos = Vector2(0.0f, 360.0f);
		// �p�x�Ƃ��͂̂��̂���������
		m_rot = 0.0f;
		break;

	// �E
	case 2:
		// ���W�͂̂��̂�����
		m_pos = Vector2(1280.0f, 360.0f);
		// �p�x�Ƃ��͂̂��̂���������
		m_rot = 180.0f;
		break;
	}

	m_animation = 0;
	m_frame = 0;
}

/**
* @brief �f�X�g���N�^
*/
DeadEffect::~DeadEffect()
{
	m_animation = 0;
	m_frame = 0;
}

/**
* @brief �X�V
*/
bool DeadEffect::Update()
{
	// �A�j���[�V��������
	if (m_frame % 5 == 0){
		m_animation++;
	}

	// �A�j���[�V�������I��������폜
	if (m_animation >= DEADEFFECT_TEXTURE_MAX){
		return false;
	}

	// �t���[���̍X�V
	m_frame++;

	return true;
}

/**
* @brief �`��
*/
void DeadEffect::Render()
{
	TextureManager* tex = TextureManager::GetInstance();

	tex->DrawTexture(m_tex[m_animation], m_pos.x, m_pos.y, m_rot);
}