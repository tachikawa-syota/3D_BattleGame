#include "Option.h"
#include "Title.h"
#include "SequenceChanger.h"

/// �ΐ�l���e�L�X�g�̕`��ʒuX
#define BATTLENUM_TEXT_POSX 450.0f
/// �ΐ�l���e�L�X�g�̕`��ʒuY
#define BATTLENUM_TEXT_POSY 200.0f
/// �ΐ탂�[�h�̕`��ʒuX
#define BATTLEMODE_TEXT_POSX 450.0f
/// �ΐ탂�[�h�̕`��ʒuY
#define BATTLEMODE_TEXT_POSY 400.0f
/// �X�y�V�����o�g���̕`��ʒu
#define SPECIALBATTLE_TEXT_POSX 450.0f
/// �X�y�V�����o�g���̕`��ʒu
#define SPECIALBATTLE_TEXT_POSY 600.0f

/**
* @brief �쐬
*/
void Option::Create()
{
	// �C���X�^���X�̎擾
	m_2dTex = TextureManager::GetInstance();
	m_fileData = FileData::GetInstance();

	// �J�[�\���̍쐬
	m_cursor = make_unique<SelectCursor>(typeid(Option));

	// �����e�N�X�`�����R�s�[
	for (int i = 0; i < NUMBER_TEXTURE_MAX; i++){
		m_numTex[i] = m_fileData->GetNumberTexture(i);
	}
}

/**
* @brief ������
*/
void Option::Initialize()
{
	UniqueData* Udata = UniqueData::GetInstance();

	// �o�^����Ă���f�[�^�̎擾
	m_battleNum = Udata->GetBattleNum();
	m_battleMode = Udata->GetBattleMode();
	m_isSpecialBattle = Udata->GetSpecialBattleFlag();

	// �\���t���O�̏�����
	m_isLeft = false;
	m_isRight = false;
}

/**
* @brief �X�V
*/
void Option::Update()
{
	// �E����
	if (m_cursor->GetSelectRight())
	{
		// �J�[�\���̔ԍ�
		switch (m_cursor->GetNumber())
		{
		// �ΐ�l��
		case SettingBattleNum:
			// �J�E���g�𑝂₷
			m_battleNum++;

			// �ΐ�l���͍ō� 4�l
			if (m_battleNum > 4){
				m_battleNum = 4;
			}
			break;

		// �o�g�����[�h
		case SettingBattleMode:
			m_battleMode = BattleMode::Stock;
			break;

			// �X�y�V�����o�g��
		case SettingSpecialBattle:
			m_isSpecialBattle = true;
			break;
		}
	}

	// ������
	else if (m_cursor->GetSelectLeft())
	{
		switch (m_cursor->GetNumber())
		{
			// �ΐ�l��
		case SettingBattleNum:
			// �J�E���g�����炷
			m_battleNum--;

			// �ΐ�l���͍Œ� 2�l
			if (m_battleNum < 2){
				m_battleNum = 2;
			}
			break;
	
			// �o�g�����[�h
		case SettingBattleMode:
			m_battleMode = BattleMode::Time;
			break;

			// �X�y�V�����o�g��
		case SettingSpecialBattle:
			m_isSpecialBattle = false;
			break;
		}
	}

	// ���A�C�R���\���t���O�̍X�V
	UpdateDisplayArrowIcon();

	UniqueData* Udata = UniqueData::GetInstance();
	// �R���s���[�^�[�̐� = �ΐ�l�� - �f�o�C�X�̌�
	int computerCount = m_battleNum - DeviceManager::GetInstance()->GetDeviceNum();
	// 0�ȉ��ɂ͂Ȃ�Ȃ�
	if (computerCount < 0) computerCount = 0;
	// �R���s���[�^�[�̐����Z�b�g
	Udata->SetComputerCount(computerCount);
	// �ΐ�l���̃Z�b�g
	Udata->SetBattleNum(m_battleNum);
	// �o�g�����[�h�̃Z�b�g
	Udata->SetBattleMode(m_battleMode);
	// �X�y�V�����o�g���t���O�̃Z�b�g
	Udata->SetSpecialBattleFlag(m_isSpecialBattle);

	// �^�C�g����ʂɖ߂�
	if (m_cursor->Update()){
		SequenceChanger::GetInstance()->ChangeScene(make_shared<Title>());
	}
}

/**
* @brief �`��
*/
void Option::Render()
{
	// �w�i�̓A���t�@�l�������ĕ`�悷��
	m_2dTex->DrawTexture(TITLE_BACK_PNG, 640.0f, 360.0f, 0.0f, 1.0f, 130);

	// �J�[�\��
	m_cursor->Render();

	// �ΐ�l���e�L�X�g
	m_2dTex->DrawTexture(BATTLENUM_TEXT_PNG, BATTLENUM_TEXT_POSX, BATTLENUM_TEXT_POSY);
	
	// �ΐ�l������
	m_2dTex->DrawTexture(m_numTex[m_battleNum], BATTLENUM_TEXT_POSX + 500.0f, BATTLENUM_TEXT_POSY);
	
	// �o�g�����[�h
	m_2dTex->DrawTexture(BATTLEMODE_TEXT_PNG, BATTLEMODE_TEXT_POSX, BATTLEMODE_TEXT_POSY);

	// �^�C�������̕`��
	if (m_battleMode == BattleMode::Time){
		m_2dTex->DrawTexture(TIME_TEXT_PNG, BATTLEMODE_TEXT_POSX + 500.0f, BATTLEMODE_TEXT_POSY);
	}
	// �X�g�b�N�����̕`��
	else if (m_battleMode == BattleMode::Stock){
		m_2dTex->DrawTexture(STOCK_TEXT_PNG, BATTLEMODE_TEXT_POSX + 500.0f, BATTLEMODE_TEXT_POSY);
	}

	// �X�y�V�����o�g������
	m_2dTex->DrawTexture(SPECIALBATTLE_TEXT_PNG, SPECIALBATTLE_TEXT_POSX , SPECIALBATTLE_TEXT_POSY);

	// "ON"OFF"�̕`��
	if (m_isSpecialBattle){
		m_2dTex->DrawTexture(ON_TEXT_PNG, SPECIALBATTLE_TEXT_POSX + 500.0f, SPECIALBATTLE_TEXT_POSY);
	}
	else{
		m_2dTex->DrawTexture(OFF_TEXT_PNG, SPECIALBATTLE_TEXT_POSX + 500.0f, SPECIALBATTLE_TEXT_POSY);
	}

	// �����e�L�X�g�̍����ɕ`��
	if (m_isLeft){
		m_2dTex->DrawTexture(ARROW_PNG, BATTLEMODE_TEXT_POSX + 330.0f, m_cursorY, -90.0f, 0.8f);
	}
	// �����e�L�X�g�̉E���ɕ`��
	if (m_isRight){
		m_2dTex->DrawTexture(ARROW_PNG, BATTLEMODE_TEXT_POSX + 670.0f, m_cursorY, 90.0f, 0.8f);
	}
}

/**
* @brief �I��
*/
void Option::Finalize()
{

}

/**
* @brief ���A�C�R���̕\���t���O���X�V
*/
void Option::UpdateDisplayArrowIcon()
{
	// �J�[�\����Y���W���擾����
	m_cursorY = m_cursor->GetPos().y;

	// ���A�C�R���̕\���t���O���e��ݒ肷��
	switch (m_cursor->GetNumber())
	{
	case SettingBattleNum:
		if (m_battleNum == 2){
			m_isLeft = false;
			m_isRight = true;
		}
		if (m_battleNum == 3){
			m_isLeft = true;
			m_isRight = true;
		}
		if (m_battleNum == 4){
			m_isLeft = true;
			m_isRight = false;
		}
		break;

	case SettingBattleMode:
		if (m_battleMode == BattleMode::Time){
			m_isLeft = false;
			m_isRight = true;
		} 
		if (m_battleMode == BattleMode::Stock){
			m_isLeft = true;
			m_isRight = false;
		}
		break;

	case SettingSpecialBattle:
		if (m_isSpecialBattle){
			m_isLeft = true;
			m_isRight = false;
		}
		else{
			m_isLeft = false;
			m_isRight = true;
		}
		break;
	}
}