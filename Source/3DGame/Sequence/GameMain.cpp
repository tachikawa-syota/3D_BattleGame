#pragma region �w�b�_�C���N���[�h
// �V�[��
#include "GameMain.h"
#include "Result.h"
#include "SequenceChanger.h"
#include "ScreenEffect.h"
// �I�u�W�F�N�g
#include "..\Object\Camera.h"
#include "..\Object\Item\ItemFactory.h"
#include "..\Object\Particle\SnowParticle.h"
#include "..\Object\Effect\SmokeEffect.h"
#include "..\Object\Effect\BombSmokeEffect.h"
#include "..\Object\SkyDome\SkyDome.h"
#include "..\Object\Stage\Tree.h"
#include "..\Object\Stage\StageFactory.h"
#include "..\Character\CharacterFactory.h"
// �V�X�e��
#include "..\Phyisics\Collision.h"
#include "..\Common\CommonParam.h"
#include "..\Common\STDXDefine.h"
// �o�g���V�X�e��
#include "..\GameSystem\Timer.h"
#include "..\GameSystem\Stock.h"

#pragma endregion

using std::greater;

/**
* @brief �쐬
*/
void GameMain::Create()
{
	// �C���X�^���X���擾
	m_objMgr = ObjectManager::GetInstance();

	// �I�u�W�F�N�g���X�g�����������Ă���
	m_objMgr->Clear();

	// �L�����N�^�[���쐬(�l�ԂƃR���s���[�^�[��)
	auto charaterFactory = make_unique<CharacterFactory>();
	// �ΐ�l�����擾����
	m_playerNum = UniqueData::GetInstance()->GetBattleNum();
	for (int i = 0; i < m_playerNum; i++){
		m_player.emplace_back((charaterFactory->CreateCharacter(typeid(Rock), i)));
	}
	
	// ���͏�����
	DeviceState state = DeviceState();
	// ���̓f�o�C�X���쐬
	int deviceCount = DeviceManager::GetInstance()->GetDeviceNum();
	// �Q��v���C���[�����f�o�C�X�̌��̕��������ꍇ
	if (m_playerNum < deviceCount) deviceCount = m_playerNum;
	// �f�o�C�X���쐬����
	for (int i = 0; i < deviceCount; i++){
		m_dev.emplace_back(make_unique<InputMask>(i));
		m_deviceState.emplace_back(state);
	}

	// �R���s���[�^�[�̐����擾����
	int computerCount = UniqueData::GetInstance()->GetComputerCount();
	// �R���s���[�^�[�����݂�����쐬����
	if (computerCount > 0)
	{
		// �T�C�Y�擾
		m_computer.resize(computerCount);
		// �R���s���[�^�[�̐�����������
		for (int i = 0; i < computerCount; i++){
			// AI���x��1
			if (UniqueData::GetInstance()->GetComputerLevel()[i] == 1)
				m_computer[i].Initialize(AILevel::Level1);
			// AI���x��2
			if (UniqueData::GetInstance()->GetComputerLevel()[i] == 2)
				m_computer[i].Initialize(AILevel::Level2);
			// AI�̃f�o�C�X�X�e�C�g������
			m_deviceState.emplace_back(state);
		}
	}

	// �t�H���g���쐬
	m_font = make_unique<Font>();

	// �J�������쐬
	m_camera = make_shared<Camera>(1280, 720);

	// �Q�[�����[�h�̐ݒ�
	if (UniqueData::GetInstance()->GetBattleMode() == BattleMode::Time){
		m_timer = make_shared<Timer>();
	}
	else{
		m_stock = make_shared<Stock>(m_playerNum);
	}

	// �_���[�W�f�[�^�̐ݒ�
	m_damageData.Initialize(m_playerNum);

	// �X�R�A�}�l�[�W���[�̐ݒ�
	m_scoreMgr = make_shared<ScoreManager>(m_playerNum);
	
	// �X�e�[�W���쐬
	auto stageFactory = make_unique<StageFactory>();
	m_stage = stageFactory->CreateStage(StageType::Stage1);


	// �X�J�C�h�[�����쐬
	m_skyDome = make_shared<SkyDome>();

	// UI�̍쐬
	m_gui = make_shared<GUI>(m_playerNum);

	// �e�N�X�`���̃R�s�[
	for (int i = 0; i < STARTFINISH_TEXTURE_MAX; i++){
		m_startFinishTex[i] = FileData::GetInstance()->GetStartFinishTexture(i);
	}

	// ���X�g�ɃI�u�W�F�N�g��ǉ�
	AddObject();

	// �V�F�[�_�[�̍쐬
	CreateShader();
}

/**
* @brief �I�u�W�F�N�g��ǉ�����
*/
void GameMain::AddObject()
{
	// �X�e�[�W�����X�g�ɒǉ�
	m_objMgr->Add(m_stage);
	
	// �؃I�u�W�F�N�g�����X�g�ɒǉ�
	m_objMgr->Add(m_tree = make_shared<Tree>(Vector3(6.0f, 0.0f, 8.0f)));

	// �X�J�C�h�[�������X�g�ɒǉ�
	m_objMgr->Add(m_skyDome);
	
	// �v���C���[���I�u�W�F�N�g���X�g�ɒǉ�
	for (auto it = m_player.begin(); it != m_player.end(); it++){
		m_objMgr->AddPlayer((*it));
	}
}

/**
* @brief ������
*/
void GameMain::Initialize()
{
	// �p�����[�^�̏�����
	m_subWork = 0;
	m_gameFrame = 0;
	m_isPause = false;

	// ���ʔz��̍쐬
	UniqueData::GetInstance()->RankClear();

	// ���ʂ̃Z�b�g(�v���C���[�̐���)
	m_rank = m_playerNum;

	// �X�^�[�g��ԂɃZ�b�g
	m_gameState = GameMainState::Start;

	// �쐬���ꂽ�v���C���[�̐�����������
	for (int i = 0; i != m_player.size(); i++){
		m_player[i]->Initialize();
	}

	// �X�e�[�W�Ƃ̓����蔻��
	Collision::GetInstance()->SetStageObj(m_stage->GetMesh());
	
	// BGM���Đ�����
	SoundPlayer::GetInstance()->LoopPlay(BATTLE_BGM);

	// �J�������W
	m_oldCameraPos = m_camera->GetPos();
}

/**
* @brief �X�V
*/
void GameMain::Update()
{
	switch (m_gameState)
	{
		// �o�g���J�n
	case GameMainState::Start:
		if (UpdateGameStart()){
			m_gameState = GameMainState::Play;
		}
		break;

		// �o�g����
	case GameMainState::Play:
		// �|�[�Y���Ȃ�Q�[���̍X�V�����Ȃ�
		if (UpdatePause()) return;

		if (UpdateGamePlay()){
			m_gameState = GameMainState::Finish;
		}
		break;

		// �o�g���̏I��
	case GameMainState::Finish:
		UpdateGameFinish();
		break;
	}

	// �ȉ��̏����̓|�[�Y���͓K������Ȃ� ---------------------------------

	// �Q�[���p�̃t���[�������Z����
	m_gameFrame++;

	// �V���V�F�[�_�[�̃A���r�G���g��ݒ�
	m_skyDomeShader->SetValue("AmbientMat", m_skyDome->GetAmbient());
	// ���C�g�u���[���V�F�[�_�[�̃A���r�G���g��ݒ�
	m_lightBloomShader->SetValue("AmbientMat", m_skyDome->GetAmbient());
	m_lightBloomShader->SetValue("DiffuseMat", m_skyDome->GetAmbient());

	// ��p�[�e�B�N���̐���
	UpdateSnowParticle();

	// �A�C�e���̍X�V
	UpdateItem();

	// �����܂�-------------------------------------------------------------
}

/**
* @brief �`��
*/
void GameMain::Render()
{
	// �J����
	m_camera->Active();

	// �g�}�b�v(PC���ɂ���Ă͔��f����Ȃ���������)
	LPDIRECT3DTEXTURE9 bumpMap = m_waveShader->Render();
	
	// �m�[�}���}�b�v���Z�b�g
	m_PM->SetValue("NormalMapTexture", bumpMap);

	// �[�x�o�b�t�@�����_�����O�̊J�n-------------
	m_shaderMgr->BeginDepth();
	
	// �e�̕`��
	m_objMgr->RenderShadow(m_camera);

	// �[�x�o�b�t�@�����_�����O�̏I��-------------
	m_shaderMgr->EndDepth();

	// ���C�g�u���[�����V�F�[�_�[�K�p-------------
	m_shaderMgr->BeginBlur();

	// 3D�I�u�W�F�N�g�`�悷��(�V�F�[�_�[���g�p)
	m_objMgr->Render3D(m_camera);

	// ���C�g�u���[���V�F�[�_�[��؂�-------------
	m_shaderMgr->EndBlur();

	// 2D�I�u�W�F�N�g�̕`��
	m_objMgr->Render2D();

	// UI�̕`��
	RenderUI(bumpMap);
}

/**
* @brief �I��
*/
void GameMain::Finalize()
{
	// �I�u�W�F�N�g�}�l�[�W���̒��g���N���A����
	m_objMgr->Clear();

	// BGM���~����
	SoundPlayer::GetInstance()->Stop(BATTLE_BGM);
}

/**
* @brief �V�F�[�_�[���쐬����
*/
void GameMain::CreateShader()
{
	// �V�F�[�_�[�}�l�[�W���[�̃C���X�^���X���擾
	m_shaderMgr = ShaderManager::GetInstance();

	// ����������
	m_shaderMgr->Initialize();

	// �p�����b�N�X�}�b�v���쐬����(������)
	m_PM = make_shared<Shader>("Data/Shader/ParallaxMapping.fx");

	// �X�L�����b�V���e�X�g�p�̃V�F�[�_�[
	m_skinTest = make_shared<Shader>("Data/Shader/SkinMeshTest.fx");

	// �X�J�C�{�b�N�X�V�F�[�_�[
	m_skyDomeShader = make_shared<Shader>("Data/Shader/SkyDome.fx");

	// ���C�g�u���[���V�F�[�_�[
	m_lightBloomShader = make_shared<Shader>("Data/Shader/LightBloom.fx");

	// �g�V�F�[�_�[
	m_waveShader = make_shared<WaveShader>(1024, 1024);

	// �V�F�[�_�[���X�g�ɒǉ�(���X�g�ɒǉ����Ȃ��ƍX�V����Ȃ��̂Œ���)
	m_shaderMgr->Add(m_PM, "PM");
	m_shaderMgr->Add(m_lightBloomShader, "LightBloom");
	m_shaderMgr->Add(m_skyDomeShader, "SkyDome");
	m_shaderMgr->Add(m_skinTest, "SkinMeshShader");
	m_shaderMgr->Add(m_waveShader->GetShader(), "Wave");

	// �X�e�[�W�̃V�F�[�_�[�ݒ�
	m_stage->GetMesh()->SetShader(m_lightBloomShader);

	// �؂̃V�F�[�_�[��ݒ�
	m_tree->GetMesh()->SetShader(m_PM);

	// �X�J�C�h�[���ɃV�F�[�_�[��ݒ�
	m_skyDome->GetMesh()->SetShader(m_skyDomeShader);

	// �v���C���[�ɃV�F�[�_�[��ݒ�
	for (int i = 0; i != m_player.size(); i++){
		m_player[i]->GetSkinMesh()->SetShader(m_skinTest);
	}

}

/**
* @brief �t�F�[�h��Ԃ̍X�V
* @return true - ���̏�ԂɈڍs�Ffalse - ���݂̏������p��
*/
bool GameMain::UpdateGameStart()
{
	static int frame = 0;

	switch (m_subWork)
	{
	case 0:
		// �{�C�X���Đ�����
		SoundPlayer::GetInstance()->Play(FIGHT_VOICE);
		m_subWork++;
		break;

	case 1:
		// �t���[���`�F�b�N
		if (frame > 60){
			m_subWork = 0;
			frame = 0;
			return true;
		}

		// �t���[�����Z
		frame++;
		break;
	}

	// �J�����̍X�V����
	UpdateCamera();

	// �����蔻��̃`�F�b�N
	AllCollisionCheck();

	// �I�u�W�F�N�g���X�V����
	m_objMgr->Update2D();
	m_objMgr->Update3D();

	return false;
}

/**
* @brief �o�g����Ԃ̍X�V
* @return true - ���̏�ԂɈڍs�Ffalse - ���݂̏������p��
*/
bool GameMain::UpdateGamePlay()
{
	// ���̓f�o�C�X�̍X�V
	UpdateInputDevice();

	// �J�����̍X�V����
	UpdateCamera();

	// �����蔻��̃`�F�b�N
	AllCollisionCheck();

	/// �R���s���[�^�[�̍X�V
	UpdateComputer();

	// �v���C���[�̍X�V
	UpdateCharacter();

	// �_���[�W�̍X�V
	m_damageData.Update();

	// �I�u�W�F�N�g���X�V����
	m_objMgr->Update2D();
	m_objMgr->Update3D();

	// ������я�ԂɂȂ����牌�G�t�F�N�g�𐶐�����
	CreateCheckSmokeEffect();

	// �^�C�����[�h�̍X�V
	if (UpdateBattleModeTime()) return true;

	// �X�g�b�N���[�h�̍X�V
	if (UpdateBattleModeStock()) return true;

	// �������p������
	return false;
}

/**
* @brief �o�g���I���̍X�V
*/
void GameMain::UpdateGameFinish()
{
	// �t���[��
	static int frame = 0;

	switch (m_subWork)
	{
	case 0:
		SoundPlayer::GetInstance()->Play(FINISH_VOICE);
		m_subWork++;
		break;

		// �X�R�A�𒊏o���ă����L���O���Z�b�g����
	case 1:
	{
		// �X�R�A�A�\�[�g�p�X�R�A�A���j���A���S��
		vector<int> score, orderScore, kill, dead;
		// ���[�h�t���O
		vector<bool> loadFlag;

		for (int i = 0; i < m_playerNum; i++)
		{
			// �X�R�A�̎擾
			score.emplace_back(m_scoreMgr->GetScore(i));
			orderScore.emplace_back(score[i]);

			// ���j���̎擾
			kill.emplace_back(m_scoreMgr->GetKillCount(i));

			// ���S���̎擾
			dead.emplace_back(m_scoreMgr->GetDeadCount(i));

			// ���[�h�t���O�̐ݒ�
			loadFlag.push_back(false);

			// ���U���g�f�[�^�̓o�^
			UniqueData::GetInstance()->SetResultData(score[i], kill[i], dead[i], i);
		}

		// �X�R�A���~���Ń\�[�g
		sort(orderScore.begin(), orderScore.end(), greater<int>());

		// 1�ʂ��珇�ԂɃ��[�v������
		for (int i = 0; i != orderScore.size(); i++)
		{
			// �v���C���[�̃X�R�A�����[�v������
			for (int playerID = 0; playerID != score.size(); playerID++)
			{
				// �\�[�g�����X�R�A�ƈ�v�{�ǂݍ���ł��Ȃ��ꍇ
				if (orderScore[i] == score[playerID] && !loadFlag[playerID])
				{
					// �ǂݍ��񂾃t���O��ON�ɂ���
					loadFlag[playerID] = true;
					// 1�ʂ��珇�ԂɃv���C���[�ԍ����Z�b�g���Ă���
					UniqueData::GetInstance()->SetRankUnder(playerID);
				}
			}
		}

		// �x�N�^�[�̗v�f�폜
		score.clear();
		orderScore.clear();
		kill.clear();
		dead.clear();
		loadFlag.clear();

		m_subWork++;
	break;
	}

	case 2:
		// �t���[�����Z
		frame++;

		// ���U���g��ʂɔ�΂�
		if (frame > 60){
			frame = 0;
			SequenceChanger::GetInstance()->ChangeScene(make_shared<Result>());
		}
		break;
	}

	// �J�����̍X�V����
	UpdateCamera();

	// �����蔻��̃`�F�b�N
	AllCollisionCheck();

	// �I�u�W�F�N�g���X�V����
	m_objMgr->Update2D();
	m_objMgr->Update3D();
}

/**
* @brief �^�C�����[�h�̍X�V������
*/
bool GameMain::UpdateBattleModeTime()
{
	// �^�C�����[�h�Ȃ�
	if (UniqueData::GetInstance()->GetBattleMode() == BattleMode::Time)
	{
		// �^�C�}�[�X�V
		m_timer->Update();
		// �^�C�}�[��"0"�ɂȂ�����
		if (m_timer->GetTime() <= 0){
			return true;
		}
	}

	return false;
}

/**
* @brief �X�g�b�N���[�h�̍X�V������
*/
bool GameMain::UpdateBattleModeStock()
{
	UniqueData* Udata = UniqueData::GetInstance();

	// �X�g�b�N���[�h�Ȃ�
	if (Udata->GetBattleMode() == BattleMode::Stock)
	{
		// �X�g�b�N����0�̃v���C���[�̓��^�C�A�ɂ���
		for (int i = 0; i != m_player.size(); i++)
		{
			if (m_stock->GetStock(i) == 0)
			{
				// ���Ƀ��^�C�A���Ă�ԍ��͏��������Ȃ�
				if (m_player[i]->IsRetire()) continue;

				// ���^�C�A��Ԃɂ���
				m_player[i]->SetRetire(true);
				// ���ʂ̃Z�b�g(���[�ɑ}��)
				Udata->SetRankUnder(i);
				// ���ʂ̃J�E���g�����炷
				m_rank--;
			}

			// 1�ʂ����肵����
			if (m_rank <= 1){

				// ���Ƀ��^�C�A���Ă�ԍ��͏��������Ȃ�
				if (m_player[i]->IsRetire()) continue;

				// ���ʂ̃Z�b�g(��[�ɑ}��)
				Udata->SetRankOver(i);

				// �����̏I��
				return true;
			}
		}
	}

	// �����𑱂���
	return false;
}

/**
* @brief ����~�炷
*/
void GameMain::UpdateSnowParticle()
{
	static int frame = 0;
	
	// �������W��ݒ肷��
	Vector3 pos = Vector3(0.0f, 20.0f, 0.0f);
	pos.x = Utility::Random(-30.0f, 30.0f);
	pos.z = Utility::Random(0.0f, 11.0f);

	// �t���[�������Z
	frame++;

	// ���t���[�����Ƃɐ�𐶐�����
	if (frame < 3) return;

	// �t���[���̏�����
	frame = 0;

	// ��𐶐�����
	m_objMgr->AddBack(make_shared<SnowParticle>(pos, m_camera->GetView()));
}

/**
* @brief �A�C�e���̍X�V
*/
void GameMain::UpdateItem()
{
	// �A�C�e���t�@�N�g��
	ItemFactory IF;
	// �{���̍ő吔
	unsigned int bombMax = 1;

	// �X�y�V�����o�g���t���O��"ON"�Ȃ�
	if (UniqueData::GetInstance()->GetSpecialBattleFlag()){
		bombMax = 10;
		if (m_gameFrame >= 3600){
			bombMax = 20;
		}
	}
	// �t���O��"OFF"
	else{
		// 80�b�o��
		if (m_gameFrame >= 4800){
			bombMax = 4;
		}
		// 60�b�o��
		else if (m_gameFrame >= 3600){
			bombMax = 3;
		}
		// 30�b�o��
		else if (m_gameFrame >= 1800){
			bombMax = 2;
		}
	}

	// �{���A�C�e���𐶐�����
	if (m_objMgr->GetItemList().size() < bombMax)
	{
		// �����ō��W������
		float pos = Utility::Random(-8.0f, 8.0f);
		// ���X�g�ɓo�^
		m_objMgr->AddItem(IF.CreateItem(ItemType::Bomb, Vector3(pos, 10.0f, GetComPrm->InitPosZ)), m_lightBloomShader);
	}

	// �����O�̉��G�t�F�N�g�𐶐����� ------------------------------------------	
	for (auto it = m_objMgr->GetItemList().begin(); it != m_objMgr->GetItemList().end(); ++it)
	{
		if ((*it)->IsDamage())
		{
			// ���W�̎擾
			Vector3 pos = (*it)->GetPos();
			// �G�t�F�N�g
			m_objMgr->AddBack(make_shared<BombSmokeEffect>(pos, m_camera->GetView()));
			m_objMgr->AddBack(make_shared<BombSmokeEffect>(pos, m_camera->GetView()));
		}
	}
	// �����܂�-----------------------------------------------------------------
}

/**
* @brief �J�����̍X�V
*/
void GameMain::UpdateCamera()
{
	// �X�e�[�W�̍��W
	Vector3 stagePos = m_stage->GetPos();
	// �J�������W
	Vector3	cameraPos = m_camera->GetPos();

	// ��ԗ���Ă���v���C���[���m�̋������擾����
	float length = Utility::GetFarPlayerDistance(m_player);

	// �X�e�[�W���痣��Ă��邩���v������
	bool flag = false;
	for (int i = 0; i != m_player.size(); i++)
	{
		Vector3 pos = m_player[i]->GetPos();

		if (pos.x >= 12.0f || pos.x <= -12.0f){
			// �t���O�𗧂Ă��烋�[�v���甲����
			flag = true;
			break;
		}
	}

	// �v���C���[���m�̋��������ȏ㗣����
	// �܂��́A�N����l�ł��X�e�[�W�����苗������Ă�����J��������������
	if (length > 17.0f || flag) {
		// �J��������������
		cameraPos.z += 0.24f;
	}
	else{
		// �J�������߂Â���
		cameraPos.z -= 0.24f;
	}

	const float MaxDistZ = 40.0f;
	const float MinDistZ = 35.0f;
	// �ő�E�ŏ��l�𒴂�����J�������W��␳
	if (cameraPos.z > MaxDistZ){
		cameraPos.z = MaxDistZ;
	}
	if (cameraPos.z < MinDistZ){
		cameraPos.z = MinDistZ;
	}

	// �J�������W���Z�b�g
	m_camera->SetPos(cameraPos);
	// �J�����̍X�V(�X�e�[�W������)
	m_camera->Update(stagePos);
	// ���W�ۑ�
	m_oldCameraPos = m_camera->GetPos();
}

/**
* @brief ���̓f�o�C�X���X�V����
*/
void GameMain::UpdateInputDevice()
{
	for (int i = 0; i != m_dev.size(); i++)
	{
		// ���^�C�A���Ă���v���C���[�ԍ��̃f�o�C�X�͍X�V���Ȃ�
		if (m_player[i]->IsRetire()) continue;

		// ��O�̓��͂��L�^����
		m_deviceState[i].prevFlag = m_deviceState[i].flag;
		// ���͏������������Ă���
		m_deviceState[i].axis = NULL;
		m_deviceState[i].flag = NULL;

		// �f�o�C�X������͂��擾����
		m_dev[i]->GetInput();

		// ���͂Ȃ�
		if (m_dev[i]->GetInputState() == NULL){
			m_deviceState[i].flag = NULL;
		}

		// ���ړ�
		if (m_dev[i]->CheckInput(INPUT_LEFT)){
			m_deviceState[i].axis = INPUT_LEFT;
		}

		// �E�ړ�
		if (m_dev[i]->CheckInput(INPUT_RIGHT)){
			m_deviceState[i].axis = INPUT_RIGHT;
		}
		
		// �W�����v
		if (m_dev[i]->CheckInputMask(INPUT_UP)){
			m_deviceState[i].axis = INPUT_UP;
		}

		// �ʏ�U��
		if (m_dev[i]->CheckInputMask(INPUT_01)){
			m_deviceState[i].flag = INPUT_01;
		}
		// �X�}�b�V���U��
		if (m_dev[i]->CheckInputMask(INPUT_02)){
			m_deviceState[i].flag = INPUT_02;
		}
		// �K�E�U��(��)
		if (m_dev[i]->CheckInputMask(INPUT_03)){
			m_deviceState[i].flag = INPUT_03;
		}
		// �A�s�[��
		if (m_dev[i]->CheckInputMask(INPUT_04)){
			m_deviceState[i].flag = INPUT_04;
		}

		// �|�[�Y
		if (m_dev[i]->CheckInputMask(INPUT_RET)){
			m_isPause = true;
			SoundPlayer::GetInstance()->Play(DECISION_SE);
		}
	}
}

/**
* @brief �R���s���[�^�[���X�V����
*/
void GameMain::UpdateComputer()
{
	// �f�o�C�X�̌����擾
	int count = DeviceManager::GetInstance()->GetDeviceNum();

	// �R���s���[�^�[�̐�����
	for (int i = 0; i != m_computer.size(); i++)
	{
		// ���^�C�A���Ă����珈�������Ȃ�
		if (m_player[i + count]->IsRetire()){
			continue;
		}

		// �v�l�J�n
		m_deviceState[i + count] = m_computer[i].Think(m_player, m_objMgr->GetItemList(), m_stage, m_damageData, i + count);
	}
}

/**
* @brief �L�����N�^�[���X�V����
*/
void GameMain::UpdateCharacter()
{
	for (int i = 0; i != m_player.size(); i++)
	{
		// ����s�\��ԂȂ珈�������Ȃ�
		if (!m_player[i]->IsControl()) continue;

		// ���͂Ȃ�
		if (m_deviceState[i].flag == NULL && m_deviceState[i].axis == NULL){
			// �n�ʂɐڂ��Ă��鎞����
			if (m_player[i]->IsGround()){
				// �ҋ@
				m_player[i]->Idling();
			}
		}

		// ���ړ�
		if (m_deviceState[i].axis == INPUT_LEFT){
			m_player[i]->MoveLeft();
		}

		// �E�ړ�
		if (m_deviceState[i].axis == INPUT_RIGHT){
			m_player[i]->MoveRight();
		}

		// �W�����v
		if (m_deviceState[i].axis == INPUT_UP){
			m_player[i]->Jump();
		}

		// �ʏ�U��
		if (CheckDeviceState(INPUT_01, i)){
			m_player[i]->NormalAttack();
		}
		// �X�}�b�V���U��
		if (CheckDeviceState(INPUT_02, i)){
			m_player[i]->SmashAttack();
		}
		// �K�E�U��(��)
		if (CheckDeviceState(INPUT_03, i)){
			m_player[i]->SpecialAttackUP();
		}
		// �A�s�[��
		if (CheckDeviceState(INPUT_04, i)){
			m_player[i]->Appeal();
		}
	}
}

/**
* @brief ���ׂĂ̓����蔻����`�F�b�N����
*/
void GameMain::AllCollisionCheck()
{
	m_objMgr->CheckAllCollision(m_camera, m_stock, m_scoreMgr, m_damageData);
}

/**
* @brief ���G�t�F�N�g�𐶐����邩�ǂ������m�F����
*/
void GameMain::CreateCheckSmokeEffect()
{
	for (int i = 0; i != m_player.size(); i++)
	{
		if (m_player[i]->GetAction() == Action::Blow){
			Vector3 pos = m_player[i]->GetDamagePos();
			Vector3 pos1 = m_player[i]->GetDamagePos();
			Vector3 pos2 = m_player[i]->GetDamagePos();
			pos1.y += 0.2f;
			pos2.y -= 0.2f;
			// �����o��
			m_objMgr->AddFront(make_shared<SmokeEffect>(pos, m_camera->GetView()));
			m_objMgr->AddFront(make_shared<SmokeEffect>(pos1, m_camera->GetView()));
			m_objMgr->AddFront(make_shared<SmokeEffect>(pos2, m_camera->GetView()));
		}
	}
}

/**
* @brief �f�o�C�X�̓��͂��`�F�b�N����
* @param ���̓t���O
*/
bool GameMain::CheckDeviceState(unsigned int FLAG, int index)
{
	// ��i�K�ōs��
	// �܂����݂̓��͂Ƃ̔���
	if (m_deviceState[index].flag & FLAG)
	{
		// ���Ɍ��݂̓��͂ƈ�O�̓��͂̔r���I�_���a�Ƃ̔���
		if ((m_deviceState[index].flag ^ m_deviceState[index].prevFlag) & FLAG){
			return true;
		}
	}
	return false;
}

/**
* @brief �|�[�Y�̍X�V
* @return true - �|�[�Y���Ffalse - �|�[�Y����
*/
bool GameMain::UpdatePause()
{
	if (!m_isPause) return false;

	// ���̓f�o�C�X�����X�V����
	UpdateInputDevice();

	Vector3 stagePos = m_stage->GetPos();
	// �J�������W
	Vector3	cameraPos = m_camera->GetPos();

	// �|�[�Y�{�^��
	if (m_dev[0]->CheckInputMask(INPUT_RET)){
		m_isPause = false;
	}

	// �J�����ړ�
	if (m_dev[0]->CheckInput(INPUT_LEFT)){
		cameraPos.x--;
	}
	if (m_dev[0]->CheckInput(INPUT_RIGHT)){
		cameraPos.x++;
	}
	if (m_dev[0]->CheckInput(INPUT_UP)){
		cameraPos.z++;
	}
	if (m_dev[0]->CheckInput(INPUT_DOWN)){
		cameraPos.z--;
	}

	// �J�������W���Z�b�g
	if (m_isPause)
		m_camera->SetPos(cameraPos);
	// �|�[�Y�I���Ō��̃J�������W�ɖ߂�
	else
		m_camera->SetPos(m_oldCameraPos);
	
	// �J�����̍X�V(�X�e�[�W������)
	m_camera->Update(stagePos);

	// ��Ԃ�Ԃ�
	return m_isPause;
}

/**
* @brief UI�̕`��
*/
void GameMain::RenderUI(LPDIRECT3DTEXTURE9& bumpMap)
{
	// �e�N�X�`���}�l�[�W���[�̃C���X�^���X���擾
	TextureManager* m_2dTex = TextureManager::GetInstance();

	// �uStart�v������`��
	if (m_gameState == GameMainState::Start){
		m_2dTex->DrawTexture(m_startFinishTex[0], 640.0f, 360.0f);
	}
	// �uFinish�v������`��
	if (m_gameState == GameMainState::Finish){
		m_2dTex->DrawTexture(m_startFinishTex[1], 640.0f, 360.0f);
	}

	// �|�[�Y�������`�悷��
	if (m_isPause){
		D3DIns->GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

		// �|�[�Y�e�L�X�g
		m_2dTex->DrawTexture(PAUSE_TEXT_PNG, 640.0f, 100.0f, 0.0f, 1.0f);

		// ���C�g�̈ʒu
		m_2dTex->Draw3DTexture(m_camera->GetView(), LIGHTPOS_PNG, m_shaderMgr->GetLightPos(), Vector3(), 0.009f, 255, false, false);
		// ���C�g�̎���
		m_2dTex->Draw3DTexture(m_camera->GetView(), LIGHTEYE_PNG, m_shaderMgr->GetLightAt(), Vector3(), 0.009f, 255, false, false);

		// �t�H���g�̕`��
		char	str[4][128];
		sprintf_s(str[0], "�J�����̏ꏊ,X:%2.2f,Y:%2.2f,Z:%2.2f", m_camera->GetPos().x, m_camera->GetPos().y, m_camera->GetPos().z);
		sprintf_s(str[1], "�J�����̎���,X:%2.2f,Y:%2.2f,Z:%2.2f", m_stage->GetPos().x, m_stage->GetPos().y, m_stage->GetPos().z);
		sprintf_s(str[2], "���C�g�̏ꏊ,X:%2.2f,Y:%2.2f,Z:%2.2f", m_shaderMgr->GetLightPos().x, m_shaderMgr->GetLightPos().y, m_shaderMgr->GetLightPos().z);
		sprintf_s(str[3], "���C�g�̎���,X:%2.2f,Y:%2.2f,Z:%2.2f", m_shaderMgr->GetLightAt().x, m_shaderMgr->GetLightAt().y, m_shaderMgr->GetLightAt().z);
		for (int i = 0; i < 4; i++){
			m_font->DrawFont(str[i], 10, 10 + (100 * (i + 1)), 600, 250 + (100 * (i + 1)), D3DCOLOR_XRGB(255, 0, 0), FALSE);
		}

		D3DIns->GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

		// ���C�g�u���[���e�N�X�`��
		m_2dTex->DrawTextureEx(1130.0f, 0.0f, 1280.0f, 150.0f, m_shaderMgr->GetShaderTexture(ShaderManager::TexDataType::LightBloom));
		// �u���[���������e�N�X�`��
		m_2dTex->DrawTextureEx(1130.0f, 150.0f, 1280.0f, 300.0f, m_shaderMgr->GetShaderTexture(ShaderManager::TexDataType::Blur2));
		// �F���̃e�N�X�`��
		m_2dTex->DrawTextureEx(1130.0f, 300.0f, 1280.0f, 450.0f, m_shaderMgr->GetShaderTexture(ShaderManager::TexDataType::Color));
		// �[�x�e�N�X�`��
		m_2dTex->DrawTextureEx(1130.0f, 450.0f, 1280.0f, 600.0f, m_shaderMgr->GetShadowMapTexture());
		// �g�e�N�X�`��
		m_2dTex->DrawTextureEx(1130.0f, 600.0f, 1280.0f, 750.0f, bumpMap);

	}
	// �Q�[����UI
	else{
		m_gui->Render(m_timer, m_stock, m_player, m_camera, m_damageData);
	}
}