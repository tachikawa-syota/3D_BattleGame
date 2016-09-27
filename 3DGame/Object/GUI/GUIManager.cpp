#include "GUIManager.h"
#include "..\..\GameSystem\Timer.h"
#include "..\..\GameSystem\Stock.h"
#include "..\..\Object\Camera.h"
#include "..\..\Common\CommonParam.h"
#include "..\..\Common\UniqueData.h"

/**
* @brief �R���X�g���N�^
*/
GUI::GUI(int playerCount)
{
	// �C���X�^���X�̎擾
	m_2dTex = TextureManager::GetInstance();
	m_fileData = FileData::GetInstance();

	// �v���C���[�̐���o�^
	m_playerCount = playerCount;

	// �����e�N�X�`�����R�s�[����
	for (int i = 0; i < NUMBER_TEXTURE_MAX; i++){
		m_numberTex[i] = m_fileData->GetNumberTexture(i);
	}
	// �v���C���[�e�L�X�g�e�N�X�`�����R�s�[����
	for (int i = 0; i < PLAYER_TEXTURE_MAX; i++){
		m_playerTextTex[i] = m_fileData->GetPlayerTexture(i);
	}

	// �p�����[�^�̏�����
	m_fightAnimation = 0;

	// �v���C���[�e�L�X�g�`��ʒu�̃Z�b�g
	m_playerTextPos.clear();
	float addX = 0.0f;
	for (int i = 0; i < m_playerCount; i++){
		// ����������
		Vector2 pos = Vector2(0.0f, 650.0f);
		// ���[�v����250�����W�����Z
		addX += 250.0f;
		pos.x = addX;
		// �o�^
		m_playerTextPos.emplace_back(pos);
	}

	// �X�g�b�N�̕`��ʒu�̏�����
	// ��ɕ`��ʒu�f�[�^���N���A����
	m_stockPos.clear();
	// �_���[�W�`��ʒu
	float add2X = 0.0f;
	for (int i = 0; i < m_playerCount; i++){
		Vector2 pos = Vector2(0.0f, 600.0f);
		add2X += 250.0f;
		pos.x = add2X;
		m_stockPos.emplace_back(pos);
	}

	// ��ɕ`��ʒu�f�[�^���N���A����
	m_damagePos.clear();
	// �_���[�W�`��ʒu
	float addX3 = 0.0f;
	for (int i = 0; i < m_playerCount; i++){
		Vector2 pos = Vector2(0.0f, 650.0f);
		addX3 += 250.0f;
		pos.x = addX3;
		m_damagePos.emplace_back(pos);
	}
}

/**
* @brief �f�X�g���N�^
*/
GUI::~GUI()
{

}

/**
* @brief �`��
*/
void GUI::Render(shared_ptr<Timer>& time, 
                 shared_ptr<Stock>& stock, 
	             vector<IPlayerPtr> player, 
				 CameraPtr camera,
	             DamageData damage)
{
	// �^�C�}�[�̕`��
	if (UniqueData::GetInstance()->GetBattleMode() == BattleMode::Time){
		m_2dTex->DrawTexture(m_numberTex[time->GetTenId()], 620.0f, 70.0f, 0.0f, 0.7f);
		m_2dTex->DrawTexture(m_numberTex[time->GetOneId()], 660.0f, 70.0f, 0.0f, 0.7f);
	}
	// �X�g�b�N�̕`��
	else if (UniqueData::GetInstance()->GetBattleMode() == BattleMode::Stock){
		for (int i = 0; i < m_playerCount; i++){
			m_2dTex->DrawTexture(STOCK_PNG, m_stockPos[i].x, m_stockPos[i].y, 0.0f, 0.5f);
			m_2dTex->DrawTexture(m_numberTex[stock->GetStock(i)], m_stockPos[i].x + 30.0f, m_stockPos[i].y, 0.0f, 0.5f);
		}
	}

	// �_���[�W���̕`��
	for (int i = 0; i < m_playerCount; i++){
		m_2dTex->DrawTexture(m_numberTex[damage.GetHundredId(i)], m_damagePos[i].x + 50.0f, 650.0f, 0.0f, 0.4f);
		m_2dTex->DrawTexture(m_numberTex[damage.GetTenId(i)], m_damagePos[i].x + 80.0f, 650.0f, 0.0f, 0.4f);
		m_2dTex->DrawTexture(m_numberTex[damage.GetOneId(i)], m_damagePos[i].x + 110.0f, 650.0f, 0.0f, 0.4f);
	}

	// �v���C���[�e�L�X�g�`��
	for (int i = 0; i != m_playerTextPos.size(); i++){
		m_2dTex->DrawTexture(m_playerTextTex[i], m_playerTextPos[i].x, m_playerTextPos[i].y, 0.0f, 0.5f);
	}

	// �v���C���[�̓���ɔԍ���`��
	for (int i = 0; i != m_playerTextPos.size(); i++){
		// ���^�C�A���Ă�v���C���[�̔ԍ��͕`�悵�Ȃ�
		if (player[i]->IsRetire()) continue;
		// ���W�擾
		Vector3 pos = player[i]->GetPos();
		// ����ɍ��W�𒲐�
		pos.y = pos.y + 2.7f;
		// �r���{�[�h�ŕ`��
		m_2dTex->Draw3DTexture(camera->GetInverseView(), m_playerTextTex[i], pos, Vector3(), 0.01f, ALPHA_MAX);
		m_2dTex->Draw3DTexture(camera->GetInverseView(), m_playerTextTex[i], pos, Vector3(), 0.01f, ALPHA_MAX);
	}
}