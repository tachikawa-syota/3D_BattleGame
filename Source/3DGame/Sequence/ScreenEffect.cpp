#include "ScreenEffect.h"
#include "..\DirectX\Direct3DData.h"

/**
* @brief ������
*/
void ScreenEffect::Initialize()
{
	m_add = 0;
	m_color = FadeColor::Black;
	m_frame = 0;
	m_fade_cnt = 0;
	m_state = Fade::None;
}

/**
* @brief �t�F�[�h�̐ݒ�
* @param type - �t�F�[�h�̎��(In �܂��� Out)
* @param color - �t�F�[�h�F
* @param frame - �t�F�[�g���Ă�t���[��
*/
bool ScreenEffect::SetFade(Fade type, FadeColor color, int frame)
{
	// �t�F�[�h�̎��
	switch (type)
	{
		//�t�F�[�h�C��
	case Fade::In:
		m_fade_cnt = frame;
		m_add = -1;
		break;
		//�t�F�[�h�A�E�g
	case Fade::Out:
		m_fade_cnt = 0;
		m_add = 1;
		break;
		//�t�F�[�h�Ȃ�
	default:
		m_add = 0;
		break;
	}
	m_color = color;
	m_frame = frame;
	m_state = type;

	// �t�F�[�h���|�����Ă��Ȃ�������
	if (m_state == Fade::None){
		return false; 
	}

	return true;
}

/**
* @brief �t�F�[�h���s�������`�F�b�N
* @return None�ȊO����true���Ԃ�
*/
bool ScreenEffect::isPlayFade()
{
	if (m_state == Fade::None){
		return false;
	}
	else{
		return true;
	}
}

/**
* @brief �X�V
*/
void ScreenEffect::Update()
{
	// �t�F�[�h���|�����Ă��Ȃ������牽�����Ȃ�
	if (m_state == Fade::None) return;

	//�t���[���̉��Z
	m_fade_cnt += m_add;
	//�t�F�[�h�A�E�g�I��
	if (m_fade_cnt > m_frame){
		m_fade_cnt = m_frame;
		m_state = Fade::None;
	}
	//�t�F�[�h�C���I��
	if (m_fade_cnt < 0){
		m_state = Fade::None;
		m_fade_cnt = 0;
		m_add = 0;
	}
}

/**
* @brief �`��
*/
void ScreenEffect::Draw()
{
	// �t�F�[�h���|�����Ă��Ȃ������牽�����Ȃ�
	if (m_state == Fade::None) return;

	// ���ߏ���������悤�ύX
	D3DIns->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	// ���ߑO�̕���(���n)�̐ݒ� �d�˂�摜�̓����x�������قǔ�������
	D3DIns->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// �d�˂�摜�͂��̓��ߐF��K�p
	D3DIns->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	// �{�b�N�X�̕`��
	if (m_fade_cnt > 0)
	{
		float alpha = (float)(m_fade_cnt) / m_frame;
		// ���A���t�@�u�����h
		if (m_color == FadeColor::Black){
			TextureManager::GetInstance()->DrawBox(0, 0, 1280.0f, 720.0f, (D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, alpha)));
		}
		// ���A���t�@�u�����h
		if (m_color == FadeColor::White){
			TextureManager::GetInstance()->DrawBox(0, 0, 1280.0f, 720.0f, (D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, alpha)));
		}
	}

	D3DIns->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}
