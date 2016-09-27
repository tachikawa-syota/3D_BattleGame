#include <algorithm>
#include "Result.h"
#include "Title.h"
#include "SequenceChanger.h"
#include "..\DirectX\SoundPlayer.h"
#include "..\Common\StringDefine.h"

/**
* @brief �쐬
*/
void Result::Create()
{
	// �C���X�^���X�̎擾
	m_2dTex = TextureManager::GetInstance();
	m_fileData = FileData::GetInstance();

	// �f�o�C�X�̍쐬
	m_dev = make_unique<InputMask>(0);

	// �e�N�X�`�����R�s�[����
	for (int i = 0; i < NUMBER_TEXTURE_MAX; i++){
		m_numberTex[i] = m_fileData->GetNumberTexture(i);
	}
	for (int i = 0; i < PLAYER_TEXTURE_MAX; i++){
		m_playerTextTex[i] = m_fileData->GetPlayerTexture(i);
	}
}

/**
* @brief ������
*/
void Result::Initialize()
{
	m_lightAngle = 0.0f;

	// ���y�̍Đ�
	SoundPlayer::GetInstance()->LoopPlay(TITLE_BGM);
	SoundPlayer::GetInstance()->Play(CHEER_SE);

	// �ΐ�l���̐����擾
	int count = UniqueData::GetInstance()->GetBattleNum();

	// ���ʂ̃Z�b�g
	for (int i = 0; i < count; i++)
	{
		m_rank.emplace_back(UniqueData::GetInstance()->GetRank(i));
		m_data.emplace_back(UniqueData::GetInstance()->GetResultData(i));
	}
	
}

/**
* @brief �X�V
*/
void Result::Update()
{
	// �f�o�C�X������͂𓾂�
	m_dev->GetInput();

	// �^�C�g����ʂɖ߂�
	if (UpdateResultWork()){
		SequenceChanger::GetInstance()->ChangeScene(make_shared<Title>());
	}

	// ���̊p�x�����Z����
	m_lightAngle++;
}

/**
* @brief �`��
*/
void Result::Render()
{
	// �w�i�̕`��
	m_2dTex->DrawTexture(TITLE_BACK_PNG, 640.0f, 360.0f, 0.0f, 1.0f, 150);

	// ���̗ւ̕`��
	m_2dTex->DrawTexture(COLORRING_PNG, 640.0f, 100.0f);

	// ���̕`��
	m_2dTex->DrawTexture(LIGHT_EFFECT_PNG, 640.0f, 100.0f, m_lightAngle, 1.25f, 255, true);

	// Winner�����̕`��
	m_2dTex->DrawTexture(RESULT_WINNER_PNG, 640.0f, 360.0f, 0.0f, 1.0f, 255);

	// �v���C���[�e�L�X�g�̕`��
	m_2dTex->DrawTexture(m_playerTextTex[m_rank[0]], 640.0f, 300.0f, 0.0f, 1.5f);
}

/**
* @brief �I��
*/
void Result::Finalize()
{
	m_rank.clear();
}

/**
* @brief ���U���g�̍X�V
*/
bool Result::UpdateResultWork()
{
	// ���͂��m�F������
	if (m_dev->CheckInputMask(INPUT_01)){
		return true;
	}

	return false;
}