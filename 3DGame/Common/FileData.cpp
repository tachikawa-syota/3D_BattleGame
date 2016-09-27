#include "FileData.h"

/**
* @brief �e�N�X�`���f�[�^��ǂݍ���
*/
HRESULT FileData::LoadTexture()
{
	// �C���X�^���X�̐���
	TextureManager* tex = TextureManager::GetInstance();

	// �ʏ�e�N�X�`���̓ǂݍ���
	tex->LoadTexture(TITLE_BACK_PNG);
	tex->LoadTexture(TITLE_LOGO_PNG);
	tex->LoadTexture(CREDIT_BACK_PNG);
	tex->LoadTexture(CREDIT_TEXT_PNG);
	tex->LoadTexture(END_TEXT_PNG);
	tex->LoadTexture(CPULEVELSELECT_TEXT_PNG);
	tex->LoadTexture(LEVEL_TEXT_PNG);
	tex->LoadTexture(BATTLE_TEXT_PNG);
	tex->LoadTexture(SPECIALBATTLE_TEXT_PNG);
	tex->LoadTexture(ON_TEXT_PNG);
	tex->LoadTexture(OFF_TEXT_PNG);
	tex->LoadTexture(PAUSE_TEXT_PNG);
	tex->LoadTexture(CHECK_TITLEBACK_TEXT_PNG);
	tex->LoadTexture(YES_TEXT_PNG);
	tex->LoadTexture(NO_TEXT_PNG);
	tex->LoadTexture(CURSOL_PNG);
	tex->LoadTexture(ARROW_PNG);
	tex->LoadTexture(OPTION_TEXT_PNG);
	tex->LoadTexture(START_FONT_PNG);
	tex->LoadTexture(END_FONT_PNG);
	tex->LoadTexture(HIT_PARTICLE_PNG);
	tex->LoadTexture(BLUR_EFFECT_PNG);
	tex->LoadTexture(BURSTRING_EFFECT_BMP);
	tex->LoadTexture(SNOW_PARTICLE_PNG);
	tex->LoadTexture(FIRE_PARTICLE_PNG);
	tex->LoadTexture(FIRE2_PARTICLE_PNG);
	tex->LoadTexture(PLAYERTEXT_PNG);
	tex->LoadTexture(BATTLENUM_TEXT_PNG);
	tex->LoadTexture(BATTLEMODE_TEXT_PNG);
	tex->LoadTexture(TIME_TEXT_PNG);
	tex->LoadTexture(STOCK_TEXT_PNG);
	tex->LoadTexture(STOCK_PNG);
	tex->LoadTexture(RESULT_WINNER_PNG);
	tex->LoadTexture(SMOKE_EFFECT_PNG);
	tex->LoadTexture(BOMB_SMOKE_EFFECT_PNG);
	tex->LoadTexture(EXPLOSION_SMOKE_EFFECT_PNG);
	tex->LoadTexture(LIGHT_EFFECT_PNG);
	tex->LoadTexture(LIGHT2_EFFECT_PNG);
	tex->LoadTexture(COLORRING_PNG);
	tex->LoadTexture(LIGHTPOS_PNG);
	tex->LoadTexture(LIGHTEYE_PNG);

	// LPDIRECT3DTEXTURE9�^
	m_d3Texture[0] = tex->D3LoadTexture(NORMALMAP_TEXTURE_PNG);
	m_d3Texture[1] = tex->D3LoadTexture(NORMALMAP_TEXTURE2_PNG);

	// �u���b�N�^�e�N�X�`���̓ǂݍ���
	tex->LoadBlkTexture("Data/Texture/GUI/number.png", 64, 90, 10, 1, 10, numberTex);
	tex->LoadBlkTexture("Data/Texture/Effect/0.png", 192, 192, 3, 3, 9, hitEffect);
	tex->LoadBlkTexture("Data/Texture/GUI/playerText.png", 136, 80, 4, 1, 4, m_playerTextTex);
	tex->LoadBlkTexture("Data/Texture/Scene/Result/resultRank.png", 288, 168, 4, 1, 4, m_resultRankTex);
	tex->LoadBlkTexture("Data/Texture/Effect/dead.png", 920, 120, 2, 5, 10, m_deadEffectTex);
	tex->LoadBlkTexture("Data/Texture/GUI/startFinishText.png", 760, 168, 1, 2, 2, m_startFinishTex);

	// ����
	return S_OK;
}

/**
* @brief �T�E���h�f�[�^��ǂݍ���
*/
HRESULT FileData::LoadSound()
{
	// �C���X�^���X�̐���
	SoundPlayer* sound = SoundPlayer::GetInstance();

	// �{�C�X -----------------------------------------
	// �V�X�e��
	sound->Load(TITLE_VOICE);
	sound->Load(TITLE_START_VOICE);
	sound->Load(OPTION_VOICE);
	sound->Load(FINISH_VOICE);
	sound->Load(FIGHT_VOICE);

	// �L�����N�^�[
	sound->Load(ROCK_NORMALATTACK_VOICE);
	sound->Load(ROCK_SMASHATTACK_VOICE);
	sound->Load(ROCK_SPECIALATTACKUP_VOICE);
	sound->Load(ROCK_DEAD_VOICE);
	sound->Load(ROCK_APPEAL_VOICE);
	
	// ���y -------------------------------------------
	sound->Load(TITLE_BGM);
	sound->Load(BATTLE_BGM);

	// ���ʉ� -----------------------------------------
	sound->Load(DECISION_SE);
	sound->Load(CHEER_SE);
	sound->Load(HIT_SE);
	sound->Load(BOMB_HIT_SE);

	// ����
	return S_OK;
}

/**
* @brief ���f���f�[�^��ǂݍ���
*/
HRESULT FileData::LoadModel()
{
	// �C���X�^���X�𐶐�
	const int ModelMax = 3;
	MeshPtr mesh[ModelMax];
	for (int i = 0; i < ModelMax; i++) {
		mesh[i] = make_shared<Mesh>();
	}

	// ������`�悵�Ȃ�3D�I�u�W�F�N�g�݂̂������œǂݍ���(�N���[�����������̂��߁E�E�E)
	mesh[0]->LoadNonAnimMesh("Data/Model/SkyDome/", "Dome_Y701.x");
	m_mesh.emplace_back(mesh[0]);
	mesh[1]->LoadNonAnimMesh("Data/Model/Christmas/", "stage.x");
	m_mesh.emplace_back(mesh[1]);
	mesh[2]->LoadNonAnimMesh("Data/Model/Tree/", "Tree.x");
	m_mesh.emplace_back(mesh[2]);
	
	// ����
	return S_OK;
}