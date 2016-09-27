#include "Credit.h"
#include "Title.h"
#include "SequenceChanger.h"
#include "..\Common\FileData.h"

/**
* @brief �쐬
*/
void Credit::Create()
{
	// �f�o�C�X�̏��L����"1P"�̂�(�L�[�{�[�h������)
	m_dev = make_shared<InputMask>(0);
}

/**
* @brief ������
*/
void Credit::Initialize()
{

}

/**
* @brief �X�V
*/
void Credit::Update()
{
	// �f�o�C�X������͂𓾂�
	m_dev->GetInput();

	// �{�^�����͂��ꂽ��
	if (m_dev->CheckInputMask(INPUT_01)){
		// ���ʉ����Đ�
		SoundPlayer::GetInstance()->Play(DECISION_SE);
		// �^�C�g����ʂɖ߂�
		SequenceChanger::GetInstance()->ChangeScene(make_shared<Title>());
	}
}

/**
* @brief �`��
*/
void Credit::Render()
{
	// �C���X�^���X
	TextureManager* tex = TextureManager::GetInstance();

	// �N���W�b�g��ʂ̕`��
	tex->DrawTexture(CREDIT_BACK_PNG, 640.0f, 360.0f);

	// �^�C�g�����S�̕`��
	tex->DrawTexture(TITLE_LOGO_PNG, 640.0f, 50.0f, 0.0f, 0.5f);
}

/**
* @brief �I��
*/
void Credit::Finalize()
{

}