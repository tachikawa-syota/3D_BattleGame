#include "BombSmokeEffect.h"
#include "..\..\Common\FileData.h"
#include "..\..\Common\Utility.h"
#include "..\..\Common\CommonParam.h"

/**
* @brief �R���X�g���N�^
* @param pos - �`��ʒu
* @param dir - �`�����
*/
BombSmokeEffect::BombSmokeEffect(const Vector3& pos, const Matrix& view)
{
	// �ʒu��ݒ�
	m_pos = pos;
	m_pos.z = 12.5f;
	m_view = view;
	m_alpha = 255;

	// ���x�̏�����
	m_spd = Vector3();
	m_spd.x = Utility::Random(-0.4f, 0.4f);
	m_spd.y = Utility::Random(-0.4f, 0.4f);
}


/**
* @brief �f�X�g���N�^
*/
BombSmokeEffect::~BombSmokeEffect()
{

}

/**
* @brief �X�V
*/
bool BombSmokeEffect::Update()
{
	// �A���t�@�l��"0"�ȉ��ō폜
	if (m_alpha < 0) return false;

	// �A���t�@�l�𔲂��Ă���
	m_alpha -= 23;

	// ���W�ɑ��x�����Z����
	m_pos += m_spd;
	m_spd *= 0.76f;

	return true;
}

/**
* @brief �`��
*/
void BombSmokeEffect::Render()
{
	// �C���X�^���X�̎擾
	TextureManager* m_2dTex = TextureManager::GetInstance();

	m_2dTex->Draw3DTexture(m_view, BOMB_SMOKE_EFFECT_PNG, m_pos, Vector3(), 0.05f, m_alpha);
}