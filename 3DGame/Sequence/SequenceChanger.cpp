#include "SequenceChanger.h"
#include "ScreenEffect.h"

/// �t�F�[�h��������t���[��
#define FADE_FRAME 90

/**
* @brief ������
*/
void SequenceChanger::Initialize()
{
	m_nowScene = nullptr;
}

/**
* @brief �V�[���̐؂�ւ�
* @param �؂�ւ������V�[��������
*/
void SequenceChanger::ChangeScene(const ISequencePtr& next)
{
	// ���݂̃V�[���̏I������
	if (m_nowScene != nullptr){
		m_nowScene->Finalize();
	}

	// �V�[���̐؂�ւ�
	m_nowScene = next;
	
	// �t�F�[�h�Z�b�g
	ScreenEffect::GetInstance()->SetFade(Fade::In, FadeColor::Black, FADE_FRAME);

	// �V�[���̍쐬
	m_nowScene->Create();

	// �V�[���̏�����
	m_nowScene->Initialize();
}

/**
* @brief ���݂̃V�[���̎擾
*/
const ISequencePtr& SequenceChanger::GetNowScene() const
{
	return m_nowScene;
}