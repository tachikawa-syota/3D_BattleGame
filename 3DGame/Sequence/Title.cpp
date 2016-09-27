#include "Title.h"
#include "Option.h"
#include "Credit.h"
#include "GameMain.h"
#include "SequenceChanger.h"
#include "ScreenEffect.h"
#include "..\Common\Utility.h"
#include "..\Common\UniqueData.h"
#include "..\Object\Particle\TitleParticle.h"
#include "..\Object\ObjectManager.h"

/// �o�g���e�L�X�g�̕`��ʒu
const float BattleTextPosY = TitlePosY;
/// �I�v�V�����e�L�X�g�̕`��ʒu
const float OptionTextPosY = TitlePosY + TitleCursorAdd;
/// �N���W�b�g�e�L�X�g�̕`��ʒu
const float CreditTextPosY = TitlePosY + TitleCursorAdd * 2;
/// �I���e�L�X�g�̕`��ʒu
const float EndTextPosY = TitlePosY + TitleCursorAdd * 3;

enum SubworkState
{
	/// �t�F�[�h
	Fade = 0,
	/// �{�C�X�Đ�
	PlayVoice,
	/// ���[�h�I��
	ModeSelect,
	/// ���[�h����
	ModeDecision,
	/// CPU�̃��x����I��
	CPULevelSelect
};

/**
* @brief �쐬
*/
void Title::Create()
{
	// �C���X�^���X�̎擾
	m_2dTex = TextureManager::GetInstance();
	
	// �J�[�\���̍쐬
	m_cursor = make_unique<SelectCursor>(typeid(Title));

	// �v���C���[�e�N�X�`�����R�s�[
	for (int i = 0; i < PLAYER_TEXTURE_MAX; i++){
		m_playerText[i] = FileData::GetInstance()->GetPlayerTexture(i);
	}

	// �����e�N�X�`�����R�s�[
	for (int i = 0; i < NUMBER_TEXTURE_MAX; i++){
		m_numTex[i] = FileData::GetInstance()->GetNumberTexture(i);
	}
}

/**
* @brief ������
*/
void Title::Initialize()
{
	SoundPlayer* sound = SoundPlayer::GetInstance();

	// ���y�̍Đ�(���ɍĐ�����Ă�����Đ����Ȃ�)
	if (!sound->IsPlay(TITLE_BGM)){
		sound->LoopPlay(TITLE_BGM);
	}

	// �R���s���[�^�[�̐������z����m�ۂ���
	int count = UniqueData::GetInstance()->GetComputerCount();
	m_computerLevel.resize(count);
	for (int i = 0; i < count; i++){
		m_computerLevel[i] = 2;
	}

	// �p�����[�^�̏�����
	m_isEndBGM = false;
	m_isAddScale = true;
	m_battleFontScale = 1.0f;
	m_optionFontScale = 1.0f;
	m_creditTextScale = 1.0f;
	m_endTextScale = 1.0f;
	m_levelWork = -1;
	m_alpha = ALPHA_MAX;
	m_subWork = 0;
	m_index = 0;
}

/**
* @brief �X�V
*/
void Title::Update()
{
	// �p�[�e�B�N���̐���
	static DWORD frame = 0;
	float x = Utility::Random(0.0f, 1280.0f);
	if (frame % 15 == 0){
		ObjectManager::GetInstance()->AddBack(make_shared<TitleParticle>(Vector2(x, 720.0f)));
	}

	// �t���[�����Z
	frame++;

	// �^�C�g���X�V
	UpdateTitleWork();

	// ���X�g�ɓo�^����2D�I�u�W�F�N�g���X�V
	ObjectManager::GetInstance()->Update2D();
}

/**
* @brief �`��
*/
void Title::Render()
{
	// ���X�g�ɓo�^����2D�I�u�W�F�N�g��`��
	ObjectManager::GetInstance()->Render2D();

	// �w�i
	m_2dTex->DrawTexture(TITLE_BACK_PNG, 640.0f, 360.0f, 0.0f, 1.0f, m_alpha);
	// �^�C�g�����S
	m_2dTex->DrawTexture(TITLE_LOGO_PNG, 640.0f, 150.0f, 0.0f, 1.0f, m_alpha);
	// �o�g������
	m_2dTex->DrawTexture(BATTLE_TEXT_PNG, TitlePosX, BattleTextPosY, 0.0f, m_battleFontScale, m_alpha);
	// �I�v�V��������
	m_2dTex->DrawTexture(OPTION_TEXT_PNG, TitlePosX, OptionTextPosY, 0.0f, m_optionFontScale, m_alpha);
	// �N���W�b�g����
	m_2dTex->DrawTexture(CREDIT_TEXT_PNG, TitlePosX, CreditTextPosY, 0.0f, m_creditTextScale, m_alpha);
	// �N���W�b�g����
	m_2dTex->DrawTexture(END_TEXT_PNG, TitlePosX, EndTextPosY, 0.0f, m_endTextScale, m_alpha);
	// �J�[�\��
	m_cursor->Render(m_alpha);

	// ���x���I���������`��
	if (m_levelWork == 0){
		// �uCPU���x����I�����ĉ������v����
		m_2dTex->DrawTexture(CPULEVELSELECT_TEXT_PNG, 640.0f, 200.0f);
		// Level����
		m_2dTex->DrawTexture(LEVEL_TEXT_PNG, 500.0f, 450.0f);
		// �v���C���[�e�N�X�`���`��
		// �f�o�C�X�̐����擾����(�l�Ԃ̐�)
		int count = DeviceManager::GetInstance()->GetDeviceNum();
		m_2dTex->DrawTexture(m_playerText[m_index + count], 640.0f, 330.0f);
		// CPU�̃��x����`��
		m_2dTex->DrawTexture(m_numTex[m_computerLevel[m_index]], 750.0f, 450.0f);
	}
}

/**
* @brief �I��
*/
void Title::Finalize()
{
	// 2D�I�u�W�F�N�g���X�g���N���A
	ObjectManager::GetInstance()->Clear();

	// �o�g���J�n���I�����ꂽ������BGM���~����
	if (m_isEndBGM){
		SoundPlayer::GetInstance()->Stop(TITLE_BGM);
	}
}

/**
* @brief �^�C�g���̍X�V
*/
void Title::UpdateTitleWork()
{
	SoundPlayer* sound = SoundPlayer::GetInstance();
	SequenceChanger* seq = SequenceChanger::GetInstance();

	switch (m_subWork)
	{
	case Fade: // �t�F�[�h�I���҂�
		if (!ScreenEffect::GetInstance()->isPlayFade()){
			m_subWork++;
		}
		break;

	case PlayVoice: // �{�C�X�̍Đ�
		sound->Play(TITLE_VOICE);
		m_subWork++;
		break;

	case ModeSelect: // ���[�h�I��
		if (m_cursor->Update()){
			m_subWork++;
		}
		break;

	case ModeDecision:
		switch (m_cursor->GetNumber())
		{
			// ���C���Q�[��
		case 1:
			m_subWork++;
			break;

			// �I�v�V����
		case 2:
			sound->Play(OPTION_VOICE);
			seq->ChangeScene(make_shared<Option>());
			break;

			// �N���W�b�g
		case 3:
			seq->ChangeScene(make_shared<Credit>());
			break;

			// �I��
		case 4:
			// ���ʉ��̍Đ����I��������
			if (!sound->IsPlay(DECISION_SE)){
				exit(0);
			}
			break;
		}
		break;

	case CPULevelSelect:
		if (UpdateComputerLevelSelect()){
			// �{�C�X�̍Đ�
			sound->Play(TITLE_START_VOICE);
			seq->ChangeScene(make_shared<GameMain>());
		}
		break;
	}
	
	// �X�P�[��
	static float scale = 1.0f;
	// �g��t���O��؂�
	if (scale >= 1.3f){
		m_isAddScale = false;
	}
	// �g��t���O������
	else if (scale <= 1.0f){
		m_isAddScale = true;
	}

	// �J�[�\���ԍ����ƂɊg�傷��t�H���g���قȂ�
	switch (m_cursor->GetNumber())
	{
	case 1:
		// �I��ԍ��ȊO�͏����X�P�[����
		m_optionFontScale = 1.0f;
		m_creditTextScale = 1.0f;
		m_endTextScale = 1.0f;

		if (m_isAddScale){
			m_battleFontScale += 0.01f;
		}
		else {
			m_battleFontScale -= 0.01f;
		}
		// �X�P�[���̋L�^
		scale = m_battleFontScale;
		break;

	case 2:
		// �I��ԍ��ȊO�͏����X�P�[����
		m_battleFontScale = 1.0f;
		m_creditTextScale = 1.0f;
		m_endTextScale = 1.0f;

		if (m_isAddScale){
			m_optionFontScale += 0.01f;
		}
		else {
			m_optionFontScale -= 0.01f;
		}
		// �X�P�[���̋L�^
		scale = m_optionFontScale;
		break;

	case 3:
		// �I��ԍ��ȊO�͏����X�P�[����
		m_optionFontScale = 1.0f;
		m_battleFontScale = 1.0f;
		m_endTextScale = 1.0f;

		if (m_isAddScale){
			m_creditTextScale += 0.01f;
		}
		else {
			m_creditTextScale -= 0.01f;
		}
		// �X�P�[���̋L�^
		scale = m_creditTextScale;
		break;

	case 4:
		// �I��ԍ��ȊO�͏����X�P�[����
		m_optionFontScale = 1.0f;
		m_battleFontScale = 1.0f;
		m_creditTextScale = 1.0f;

		if (m_isAddScale){
			m_endTextScale += 0.01f;
		}
		else {
			m_endTextScale -= 0.01f;
		}
		// �X�P�[���̋L�^
		scale = m_endTextScale;
		break;
	}
}

/**
* @brief �R���s���[�^�[�̃��x���I���̍X�V
*/
bool Title::UpdateComputerLevelSelect()
{
	// �C���X�^���X�̎擾
	UniqueData* data = UniqueData::GetInstance();

	// �R���s���[�^�[�̐����擾����
	int count = data->GetComputerCount();

	// �R���s���[�^�[�����݂��Ȃ�������X���[
	if (count == 0){
		// BGM��~�t���O���Z�b�g����
		m_isEndBGM = true;
		return true;
	}

	switch (m_levelWork)
	{
	case -1:
		m_levelWork++;
		break;

	case 0:
		m_alpha = 90;

		// ���L�[�Ń��x�������炷
		if (m_cursor->GetSelectLeft()){
			m_computerLevel[m_index]--;
		}
		// �E�L�[�Ń��x���𑝂₷
		if (m_cursor->GetSelectRight()){
			m_computerLevel[m_index]++;
		}

		// �␳
		if (m_computerLevel[m_index] < 1){
			m_computerLevel[m_index] = 1;
		}
		if (m_computerLevel[m_index] > 2){
			m_computerLevel[m_index] = 2;
		}

		// �L�[���͂��m�F������ԍ��𑝂₷
		if (m_cursor->Update()){
			m_index++;
			
		}
	
		// �S�R���s���[�^�[�̃��x�����Z�b�g�ł�����I��
		if (count <= m_index){
			m_levelWork++;
		}
		break;

	case 1:
		// BGM��~�t���O���Z�b�g����
		m_isEndBGM = true;
		// �R���s���[�^�[���x�����Z�b�g����
		data->SetComputerLevel(m_computerLevel);
		return true;
		break;
	}

	return false;
}
