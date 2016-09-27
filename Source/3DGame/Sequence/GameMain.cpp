#pragma region ヘッダインクルード
// シーン
#include "GameMain.h"
#include "Result.h"
#include "SequenceChanger.h"
#include "ScreenEffect.h"
// オブジェクト
#include "..\Object\Camera.h"
#include "..\Object\Item\ItemFactory.h"
#include "..\Object\Particle\SnowParticle.h"
#include "..\Object\Effect\SmokeEffect.h"
#include "..\Object\Effect\BombSmokeEffect.h"
#include "..\Object\SkyDome\SkyDome.h"
#include "..\Object\Stage\Tree.h"
#include "..\Object\Stage\StageFactory.h"
#include "..\Character\CharacterFactory.h"
// システム
#include "..\Phyisics\Collision.h"
#include "..\Common\CommonParam.h"
#include "..\Common\STDXDefine.h"
// バトルシステム
#include "..\GameSystem\Timer.h"
#include "..\GameSystem\Stock.h"

#pragma endregion

using std::greater;

/**
* @brief 作成
*/
void GameMain::Create()
{
	// インスタンスを取得
	m_objMgr = ObjectManager::GetInstance();

	// オブジェクトリストを初期化しておく
	m_objMgr->Clear();

	// キャラクターを作成(人間とコンピューター分)
	auto charaterFactory = make_unique<CharacterFactory>();
	// 対戦人数を取得する
	m_playerNum = UniqueData::GetInstance()->GetBattleNum();
	for (int i = 0; i < m_playerNum; i++){
		m_player.emplace_back((charaterFactory->CreateCharacter(typeid(Rock), i)));
	}
	
	// 入力初期化
	DeviceState state = DeviceState();
	// 入力デバイスを作成
	int deviceCount = DeviceManager::GetInstance()->GetDeviceNum();
	// 参戦プレイヤー数よりデバイスの個数の方が多い場合
	if (m_playerNum < deviceCount) deviceCount = m_playerNum;
	// デバイスを作成する
	for (int i = 0; i < deviceCount; i++){
		m_dev.emplace_back(make_unique<InputMask>(i));
		m_deviceState.emplace_back(state);
	}

	// コンピューターの数を取得する
	int computerCount = UniqueData::GetInstance()->GetComputerCount();
	// コンピューターが存在したら作成する
	if (computerCount > 0)
	{
		// サイズ取得
		m_computer.resize(computerCount);
		// コンピューターの数だけ初期化
		for (int i = 0; i < computerCount; i++){
			// AIレベル1
			if (UniqueData::GetInstance()->GetComputerLevel()[i] == 1)
				m_computer[i].Initialize(AILevel::Level1);
			// AIレベル2
			if (UniqueData::GetInstance()->GetComputerLevel()[i] == 2)
				m_computer[i].Initialize(AILevel::Level2);
			// AIのデバイスステイト初期化
			m_deviceState.emplace_back(state);
		}
	}

	// フォントを作成
	m_font = make_unique<Font>();

	// カメラを作成
	m_camera = make_shared<Camera>(1280, 720);

	// ゲームモードの設定
	if (UniqueData::GetInstance()->GetBattleMode() == BattleMode::Time){
		m_timer = make_shared<Timer>();
	}
	else{
		m_stock = make_shared<Stock>(m_playerNum);
	}

	// ダメージデータの設定
	m_damageData.Initialize(m_playerNum);

	// スコアマネージャーの設定
	m_scoreMgr = make_shared<ScoreManager>(m_playerNum);
	
	// ステージを作成
	auto stageFactory = make_unique<StageFactory>();
	m_stage = stageFactory->CreateStage(StageType::Stage1);


	// スカイドームを作成
	m_skyDome = make_shared<SkyDome>();

	// UIの作成
	m_gui = make_shared<GUI>(m_playerNum);

	// テクスチャのコピー
	for (int i = 0; i < STARTFINISH_TEXTURE_MAX; i++){
		m_startFinishTex[i] = FileData::GetInstance()->GetStartFinishTexture(i);
	}

	// リストにオブジェクトを追加
	AddObject();

	// シェーダーの作成
	CreateShader();
}

/**
* @brief オブジェクトを追加する
*/
void GameMain::AddObject()
{
	// ステージをリストに追加
	m_objMgr->Add(m_stage);
	
	// 木オブジェクトをリストに追加
	m_objMgr->Add(m_tree = make_shared<Tree>(Vector3(6.0f, 0.0f, 8.0f)));

	// スカイドームをリストに追加
	m_objMgr->Add(m_skyDome);
	
	// プレイヤーをオブジェクトリストに追加
	for (auto it = m_player.begin(); it != m_player.end(); it++){
		m_objMgr->AddPlayer((*it));
	}
}

/**
* @brief 初期化
*/
void GameMain::Initialize()
{
	// パラメータの初期化
	m_subWork = 0;
	m_gameFrame = 0;
	m_isPause = false;

	// 順位配列の作成
	UniqueData::GetInstance()->RankClear();

	// 順位のセット(プレイヤーの数分)
	m_rank = m_playerNum;

	// スタート状態にセット
	m_gameState = GameMainState::Start;

	// 作成されたプレイヤーの数だけ初期化
	for (int i = 0; i != m_player.size(); i++){
		m_player[i]->Initialize();
	}

	// ステージとの当たり判定
	Collision::GetInstance()->SetStageObj(m_stage->GetMesh());
	
	// BGMを再生する
	SoundPlayer::GetInstance()->LoopPlay(BATTLE_BGM);

	// カメラ座標
	m_oldCameraPos = m_camera->GetPos();
}

/**
* @brief 更新
*/
void GameMain::Update()
{
	switch (m_gameState)
	{
		// バトル開始
	case GameMainState::Start:
		if (UpdateGameStart()){
			m_gameState = GameMainState::Play;
		}
		break;

		// バトル中
	case GameMainState::Play:
		// ポーズ中ならゲームの更新をしない
		if (UpdatePause()) return;

		if (UpdateGamePlay()){
			m_gameState = GameMainState::Finish;
		}
		break;

		// バトルの終了
	case GameMainState::Finish:
		UpdateGameFinish();
		break;
	}

	// 以下の処理はポーズ中は適応されない ---------------------------------

	// ゲーム用のフレームを加算する
	m_gameFrame++;

	// 天球シェーダーのアンビエントを設定
	m_skyDomeShader->SetValue("AmbientMat", m_skyDome->GetAmbient());
	// ライトブルームシェーダーのアンビエントを設定
	m_lightBloomShader->SetValue("AmbientMat", m_skyDome->GetAmbient());
	m_lightBloomShader->SetValue("DiffuseMat", m_skyDome->GetAmbient());

	// 雪パーティクルの生成
	UpdateSnowParticle();

	// アイテムの更新
	UpdateItem();

	// ここまで-------------------------------------------------------------
}

/**
* @brief 描画
*/
void GameMain::Render()
{
	// カメラ
	m_camera->Active();

	// 波マップ(PC環境によっては反映されない※調査中)
	LPDIRECT3DTEXTURE9 bumpMap = m_waveShader->Render();
	
	// ノーマルマップをセット
	m_PM->SetValue("NormalMapTexture", bumpMap);

	// 深度バッファレンダリングの開始-------------
	m_shaderMgr->BeginDepth();
	
	// 影の描画
	m_objMgr->RenderShadow(m_camera);

	// 深度バッファレンダリングの終了-------------
	m_shaderMgr->EndDepth();

	// ライトブルームをシェーダー適用-------------
	m_shaderMgr->BeginBlur();

	// 3Dオブジェクト描画する(シェーダーを使用)
	m_objMgr->Render3D(m_camera);

	// ライトブルームシェーダーを切る-------------
	m_shaderMgr->EndBlur();

	// 2Dオブジェクトの描画
	m_objMgr->Render2D();

	// UIの描画
	RenderUI(bumpMap);
}

/**
* @brief 終了
*/
void GameMain::Finalize()
{
	// オブジェクトマネージャの中身をクリアする
	m_objMgr->Clear();

	// BGMを停止する
	SoundPlayer::GetInstance()->Stop(BATTLE_BGM);
}

/**
* @brief シェーダーを作成する
*/
void GameMain::CreateShader()
{
	// シェーダーマネージャーのインスタンスを取得
	m_shaderMgr = ShaderManager::GetInstance();

	// 初期化する
	m_shaderMgr->Initialize();

	// パララックスマップを作成する(調整中)
	m_PM = make_shared<Shader>("Data/Shader/ParallaxMapping.fx");

	// スキンメッシュテスト用のシェーダー
	m_skinTest = make_shared<Shader>("Data/Shader/SkinMeshTest.fx");

	// スカイボックスシェーダー
	m_skyDomeShader = make_shared<Shader>("Data/Shader/SkyDome.fx");

	// ライトブルームシェーダー
	m_lightBloomShader = make_shared<Shader>("Data/Shader/LightBloom.fx");

	// 波シェーダー
	m_waveShader = make_shared<WaveShader>(1024, 1024);

	// シェーダーリストに追加(リストに追加しないと更新されないので注意)
	m_shaderMgr->Add(m_PM, "PM");
	m_shaderMgr->Add(m_lightBloomShader, "LightBloom");
	m_shaderMgr->Add(m_skyDomeShader, "SkyDome");
	m_shaderMgr->Add(m_skinTest, "SkinMeshShader");
	m_shaderMgr->Add(m_waveShader->GetShader(), "Wave");

	// ステージのシェーダー設定
	m_stage->GetMesh()->SetShader(m_lightBloomShader);

	// 木のシェーダーを設定
	m_tree->GetMesh()->SetShader(m_PM);

	// スカイドームにシェーダーを設定
	m_skyDome->GetMesh()->SetShader(m_skyDomeShader);

	// プレイヤーにシェーダーを設定
	for (int i = 0; i != m_player.size(); i++){
		m_player[i]->GetSkinMesh()->SetShader(m_skinTest);
	}

}

/**
* @brief フェード状態の更新
* @return true - 次の状態に移行：false - 現在の処理を継続
*/
bool GameMain::UpdateGameStart()
{
	static int frame = 0;

	switch (m_subWork)
	{
	case 0:
		// ボイスを再生する
		SoundPlayer::GetInstance()->Play(FIGHT_VOICE);
		m_subWork++;
		break;

	case 1:
		// フレームチェック
		if (frame > 60){
			m_subWork = 0;
			frame = 0;
			return true;
		}

		// フレーム加算
		frame++;
		break;
	}

	// カメラの更新処理
	UpdateCamera();

	// 当たり判定のチェック
	AllCollisionCheck();

	// オブジェクトを更新する
	m_objMgr->Update2D();
	m_objMgr->Update3D();

	return false;
}

/**
* @brief バトル状態の更新
* @return true - 次の状態に移行：false - 現在の処理を継続
*/
bool GameMain::UpdateGamePlay()
{
	// 入力デバイスの更新
	UpdateInputDevice();

	// カメラの更新処理
	UpdateCamera();

	// 当たり判定のチェック
	AllCollisionCheck();

	/// コンピューターの更新
	UpdateComputer();

	// プレイヤーの更新
	UpdateCharacter();

	// ダメージの更新
	m_damageData.Update();

	// オブジェクトを更新する
	m_objMgr->Update2D();
	m_objMgr->Update3D();

	// 吹き飛び状態になったら煙エフェクトを生成する
	CreateCheckSmokeEffect();

	// タイムモードの更新
	if (UpdateBattleModeTime()) return true;

	// ストックモードの更新
	if (UpdateBattleModeStock()) return true;

	// 処理を継続する
	return false;
}

/**
* @brief バトル終了の更新
*/
void GameMain::UpdateGameFinish()
{
	// フレーム
	static int frame = 0;

	switch (m_subWork)
	{
	case 0:
		SoundPlayer::GetInstance()->Play(FINISH_VOICE);
		m_subWork++;
		break;

		// スコアを抽出してランキングをセットする
	case 1:
	{
		// スコア、ソート用スコア、撃破数、死亡数
		vector<int> score, orderScore, kill, dead;
		// ロードフラグ
		vector<bool> loadFlag;

		for (int i = 0; i < m_playerNum; i++)
		{
			// スコアの取得
			score.emplace_back(m_scoreMgr->GetScore(i));
			orderScore.emplace_back(score[i]);

			// 撃破数の取得
			kill.emplace_back(m_scoreMgr->GetKillCount(i));

			// 死亡数の取得
			dead.emplace_back(m_scoreMgr->GetDeadCount(i));

			// ロードフラグの設定
			loadFlag.push_back(false);

			// リザルトデータの登録
			UniqueData::GetInstance()->SetResultData(score[i], kill[i], dead[i], i);
		}

		// スコアを降順でソート
		sort(orderScore.begin(), orderScore.end(), greater<int>());

		// 1位から順番にループさせる
		for (int i = 0; i != orderScore.size(); i++)
		{
			// プレイヤーのスコアをループさせる
			for (int playerID = 0; playerID != score.size(); playerID++)
			{
				// ソートしたスコアと一致＋読み込んでいない場合
				if (orderScore[i] == score[playerID] && !loadFlag[playerID])
				{
					// 読み込んだフラグをONにする
					loadFlag[playerID] = true;
					// 1位から順番にプレイヤー番号をセットしていく
					UniqueData::GetInstance()->SetRankUnder(playerID);
				}
			}
		}

		// ベクターの要素削除
		score.clear();
		orderScore.clear();
		kill.clear();
		dead.clear();
		loadFlag.clear();

		m_subWork++;
	break;
	}

	case 2:
		// フレーム加算
		frame++;

		// リザルト画面に飛ばす
		if (frame > 60){
			frame = 0;
			SequenceChanger::GetInstance()->ChangeScene(make_shared<Result>());
		}
		break;
	}

	// カメラの更新処理
	UpdateCamera();

	// 当たり判定のチェック
	AllCollisionCheck();

	// オブジェクトを更新する
	m_objMgr->Update2D();
	m_objMgr->Update3D();
}

/**
* @brief タイムモードの更新をする
*/
bool GameMain::UpdateBattleModeTime()
{
	// タイムモードなら
	if (UniqueData::GetInstance()->GetBattleMode() == BattleMode::Time)
	{
		// タイマー更新
		m_timer->Update();
		// タイマーが"0"になったら
		if (m_timer->GetTime() <= 0){
			return true;
		}
	}

	return false;
}

/**
* @brief ストックモードの更新をする
*/
bool GameMain::UpdateBattleModeStock()
{
	UniqueData* Udata = UniqueData::GetInstance();

	// ストックモードなら
	if (Udata->GetBattleMode() == BattleMode::Stock)
	{
		// ストック数が0のプレイヤーはリタイアにする
		for (int i = 0; i != m_player.size(); i++)
		{
			if (m_stock->GetStock(i) == 0)
			{
				// 既にリタイアしてる番号は処理をしない
				if (m_player[i]->IsRetire()) continue;

				// リタイア状態にする
				m_player[i]->SetRetire(true);
				// 順位のセット(末端に挿入)
				Udata->SetRankUnder(i);
				// 順位のカウントを減らす
				m_rank--;
			}

			// 1位が決定したら
			if (m_rank <= 1){

				// 既にリタイアしてる番号は処理をしない
				if (m_player[i]->IsRetire()) continue;

				// 順位のセット(先端に挿入)
				Udata->SetRankOver(i);

				// 処理の終了
				return true;
			}
		}
	}

	// 処理を続ける
	return false;
}

/**
* @brief 雪を降らす
*/
void GameMain::UpdateSnowParticle()
{
	static int frame = 0;
	
	// 生成座標を設定する
	Vector3 pos = Vector3(0.0f, 20.0f, 0.0f);
	pos.x = Utility::Random(-30.0f, 30.0f);
	pos.z = Utility::Random(0.0f, 11.0f);

	// フレームを加算
	frame++;

	// 数フレームごとに雪を生成する
	if (frame < 3) return;

	// フレームの初期化
	frame = 0;

	// 雪を生成する
	m_objMgr->AddBack(make_shared<SnowParticle>(pos, m_camera->GetView()));
}

/**
* @brief アイテムの更新
*/
void GameMain::UpdateItem()
{
	// アイテムファクトリ
	ItemFactory IF;
	// ボムの最大数
	unsigned int bombMax = 1;

	// スペシャルバトルフラグが"ON"なら
	if (UniqueData::GetInstance()->GetSpecialBattleFlag()){
		bombMax = 10;
		if (m_gameFrame >= 3600){
			bombMax = 20;
		}
	}
	// フラグが"OFF"
	else{
		// 80秒経過
		if (m_gameFrame >= 4800){
			bombMax = 4;
		}
		// 60秒経過
		else if (m_gameFrame >= 3600){
			bombMax = 3;
		}
		// 30秒経過
		else if (m_gameFrame >= 1800){
			bombMax = 2;
		}
	}

	// ボムアイテムを生成する
	if (m_objMgr->GetItemList().size() < bombMax)
	{
		// 乱数で座標を決定
		float pos = Utility::Random(-8.0f, 8.0f);
		// リストに登録
		m_objMgr->AddItem(IF.CreateItem(ItemType::Bomb, Vector3(pos, 10.0f, GetComPrm->InitPosZ)), m_lightBloomShader);
	}

	// 爆発前の煙エフェクトを生成する ------------------------------------------	
	for (auto it = m_objMgr->GetItemList().begin(); it != m_objMgr->GetItemList().end(); ++it)
	{
		if ((*it)->IsDamage())
		{
			// 座標の取得
			Vector3 pos = (*it)->GetPos();
			// エフェクト
			m_objMgr->AddBack(make_shared<BombSmokeEffect>(pos, m_camera->GetView()));
			m_objMgr->AddBack(make_shared<BombSmokeEffect>(pos, m_camera->GetView()));
		}
	}
	// ここまで-----------------------------------------------------------------
}

/**
* @brief カメラの更新
*/
void GameMain::UpdateCamera()
{
	// ステージの座標
	Vector3 stagePos = m_stage->GetPos();
	// カメラ座標
	Vector3	cameraPos = m_camera->GetPos();

	// 一番離れているプレイヤー同士の距離を取得する
	float length = Utility::GetFarPlayerDistance(m_player);

	// ステージから離れているかを計測する
	bool flag = false;
	for (int i = 0; i != m_player.size(); i++)
	{
		Vector3 pos = m_player[i]->GetPos();

		if (pos.x >= 12.0f || pos.x <= -12.0f){
			// フラグを立てたらループから抜ける
			flag = true;
			break;
		}
	}

	// プレイヤー同士の距離が一定以上離れると
	// または、誰か一人でもステージから一定距離離れていたらカメラを遠ざける
	if (length > 17.0f || flag) {
		// カメラを遠ざける
		cameraPos.z += 0.24f;
	}
	else{
		// カメラを近づける
		cameraPos.z -= 0.24f;
	}

	const float MaxDistZ = 40.0f;
	const float MinDistZ = 35.0f;
	// 最大・最小値を超えたらカメラ座標を補正
	if (cameraPos.z > MaxDistZ){
		cameraPos.z = MaxDistZ;
	}
	if (cameraPos.z < MinDistZ){
		cameraPos.z = MinDistZ;
	}

	// カメラ座標をセット
	m_camera->SetPos(cameraPos);
	// カメラの更新(ステージを見る)
	m_camera->Update(stagePos);
	// 座標保存
	m_oldCameraPos = m_camera->GetPos();
}

/**
* @brief 入力デバイスを更新する
*/
void GameMain::UpdateInputDevice()
{
	for (int i = 0; i != m_dev.size(); i++)
	{
		// リタイアしているプレイヤー番号のデバイスは更新しない
		if (m_player[i]->IsRetire()) continue;

		// 一つ前の入力を記録する
		m_deviceState[i].prevFlag = m_deviceState[i].flag;
		// 入力情報を初期化しておく
		m_deviceState[i].axis = NULL;
		m_deviceState[i].flag = NULL;

		// デバイスから入力を取得する
		m_dev[i]->GetInput();

		// 入力なし
		if (m_dev[i]->GetInputState() == NULL){
			m_deviceState[i].flag = NULL;
		}

		// 左移動
		if (m_dev[i]->CheckInput(INPUT_LEFT)){
			m_deviceState[i].axis = INPUT_LEFT;
		}

		// 右移動
		if (m_dev[i]->CheckInput(INPUT_RIGHT)){
			m_deviceState[i].axis = INPUT_RIGHT;
		}
		
		// ジャンプ
		if (m_dev[i]->CheckInputMask(INPUT_UP)){
			m_deviceState[i].axis = INPUT_UP;
		}

		// 通常攻撃
		if (m_dev[i]->CheckInputMask(INPUT_01)){
			m_deviceState[i].flag = INPUT_01;
		}
		// スマッシュ攻撃
		if (m_dev[i]->CheckInputMask(INPUT_02)){
			m_deviceState[i].flag = INPUT_02;
		}
		// 必殺攻撃(上)
		if (m_dev[i]->CheckInputMask(INPUT_03)){
			m_deviceState[i].flag = INPUT_03;
		}
		// アピール
		if (m_dev[i]->CheckInputMask(INPUT_04)){
			m_deviceState[i].flag = INPUT_04;
		}

		// ポーズ
		if (m_dev[i]->CheckInputMask(INPUT_RET)){
			m_isPause = true;
			SoundPlayer::GetInstance()->Play(DECISION_SE);
		}
	}
}

/**
* @brief コンピューターを更新する
*/
void GameMain::UpdateComputer()
{
	// デバイスの個数を取得
	int count = DeviceManager::GetInstance()->GetDeviceNum();

	// コンピューターの数だけ
	for (int i = 0; i != m_computer.size(); i++)
	{
		// リタイアしていたら処理をしない
		if (m_player[i + count]->IsRetire()){
			continue;
		}

		// 思考開始
		m_deviceState[i + count] = m_computer[i].Think(m_player, m_objMgr->GetItemList(), m_stage, m_damageData, i + count);
	}
}

/**
* @brief キャラクターを更新する
*/
void GameMain::UpdateCharacter()
{
	for (int i = 0; i != m_player.size(); i++)
	{
		// 操作不能状態なら処理をしない
		if (!m_player[i]->IsControl()) continue;

		// 入力なし
		if (m_deviceState[i].flag == NULL && m_deviceState[i].axis == NULL){
			// 地面に接している時だけ
			if (m_player[i]->IsGround()){
				// 待機
				m_player[i]->Idling();
			}
		}

		// 左移動
		if (m_deviceState[i].axis == INPUT_LEFT){
			m_player[i]->MoveLeft();
		}

		// 右移動
		if (m_deviceState[i].axis == INPUT_RIGHT){
			m_player[i]->MoveRight();
		}

		// ジャンプ
		if (m_deviceState[i].axis == INPUT_UP){
			m_player[i]->Jump();
		}

		// 通常攻撃
		if (CheckDeviceState(INPUT_01, i)){
			m_player[i]->NormalAttack();
		}
		// スマッシュ攻撃
		if (CheckDeviceState(INPUT_02, i)){
			m_player[i]->SmashAttack();
		}
		// 必殺攻撃(上)
		if (CheckDeviceState(INPUT_03, i)){
			m_player[i]->SpecialAttackUP();
		}
		// アピール
		if (CheckDeviceState(INPUT_04, i)){
			m_player[i]->Appeal();
		}
	}
}

/**
* @brief すべての当たり判定をチェックする
*/
void GameMain::AllCollisionCheck()
{
	m_objMgr->CheckAllCollision(m_camera, m_stock, m_scoreMgr, m_damageData);
}

/**
* @brief 煙エフェクトを生成するかどうかを確認する
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
			// 複数出す
			m_objMgr->AddFront(make_shared<SmokeEffect>(pos, m_camera->GetView()));
			m_objMgr->AddFront(make_shared<SmokeEffect>(pos1, m_camera->GetView()));
			m_objMgr->AddFront(make_shared<SmokeEffect>(pos2, m_camera->GetView()));
		}
	}
}

/**
* @brief デバイスの入力をチェックする
* @param 入力フラグ
*/
bool GameMain::CheckDeviceState(unsigned int FLAG, int index)
{
	// 二段階で行う
	// まず現在の入力との判定
	if (m_deviceState[index].flag & FLAG)
	{
		// 次に現在の入力と一つ前の入力の排他的論理和との判定
		if ((m_deviceState[index].flag ^ m_deviceState[index].prevFlag) & FLAG){
			return true;
		}
	}
	return false;
}

/**
* @brief ポーズの更新
* @return true - ポーズ中：false - ポーズ解除
*/
bool GameMain::UpdatePause()
{
	if (!m_isPause) return false;

	// 入力デバイスだけ更新する
	UpdateInputDevice();

	Vector3 stagePos = m_stage->GetPos();
	// カメラ座標
	Vector3	cameraPos = m_camera->GetPos();

	// ポーズボタン
	if (m_dev[0]->CheckInputMask(INPUT_RET)){
		m_isPause = false;
	}

	// カメラ移動
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

	// カメラ座標をセット
	if (m_isPause)
		m_camera->SetPos(cameraPos);
	// ポーズ終了で元のカメラ座標に戻す
	else
		m_camera->SetPos(m_oldCameraPos);
	
	// カメラの更新(ステージを見る)
	m_camera->Update(stagePos);

	// 状態を返す
	return m_isPause;
}

/**
* @brief UIの描画
*/
void GameMain::RenderUI(LPDIRECT3DTEXTURE9& bumpMap)
{
	// テクスチャマネージャーのインスタンスを取得
	TextureManager* m_2dTex = TextureManager::GetInstance();

	// 「Start」文字を描画
	if (m_gameState == GameMainState::Start){
		m_2dTex->DrawTexture(m_startFinishTex[0], 640.0f, 360.0f);
	}
	// 「Finish」文字を描画
	if (m_gameState == GameMainState::Finish){
		m_2dTex->DrawTexture(m_startFinishTex[1], 640.0f, 360.0f);
	}

	// ポーズ中だけ描画する
	if (m_isPause){
		D3DIns->GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

		// ポーズテキスト
		m_2dTex->DrawTexture(PAUSE_TEXT_PNG, 640.0f, 100.0f, 0.0f, 1.0f);

		// ライトの位置
		m_2dTex->Draw3DTexture(m_camera->GetView(), LIGHTPOS_PNG, m_shaderMgr->GetLightPos(), Vector3(), 0.009f, 255, false, false);
		// ライトの視線
		m_2dTex->Draw3DTexture(m_camera->GetView(), LIGHTEYE_PNG, m_shaderMgr->GetLightAt(), Vector3(), 0.009f, 255, false, false);

		// フォントの描画
		char	str[4][128];
		sprintf_s(str[0], "カメラの場所,X:%2.2f,Y:%2.2f,Z:%2.2f", m_camera->GetPos().x, m_camera->GetPos().y, m_camera->GetPos().z);
		sprintf_s(str[1], "カメラの視線,X:%2.2f,Y:%2.2f,Z:%2.2f", m_stage->GetPos().x, m_stage->GetPos().y, m_stage->GetPos().z);
		sprintf_s(str[2], "ライトの場所,X:%2.2f,Y:%2.2f,Z:%2.2f", m_shaderMgr->GetLightPos().x, m_shaderMgr->GetLightPos().y, m_shaderMgr->GetLightPos().z);
		sprintf_s(str[3], "ライトの視線,X:%2.2f,Y:%2.2f,Z:%2.2f", m_shaderMgr->GetLightAt().x, m_shaderMgr->GetLightAt().y, m_shaderMgr->GetLightAt().z);
		for (int i = 0; i < 4; i++){
			m_font->DrawFont(str[i], 10, 10 + (100 * (i + 1)), 600, 250 + (100 * (i + 1)), D3DCOLOR_XRGB(255, 0, 0), FALSE);
		}

		D3DIns->GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

		// ライトブルームテクスチャ
		m_2dTex->DrawTextureEx(1130.0f, 0.0f, 1280.0f, 150.0f, m_shaderMgr->GetShaderTexture(ShaderManager::TexDataType::LightBloom));
		// ブラーをかけたテクスチャ
		m_2dTex->DrawTextureEx(1130.0f, 150.0f, 1280.0f, 300.0f, m_shaderMgr->GetShaderTexture(ShaderManager::TexDataType::Blur2));
		// 色情報のテクスチャ
		m_2dTex->DrawTextureEx(1130.0f, 300.0f, 1280.0f, 450.0f, m_shaderMgr->GetShaderTexture(ShaderManager::TexDataType::Color));
		// 深度テクスチャ
		m_2dTex->DrawTextureEx(1130.0f, 450.0f, 1280.0f, 600.0f, m_shaderMgr->GetShadowMapTexture());
		// 波テクスチャ
		m_2dTex->DrawTextureEx(1130.0f, 600.0f, 1280.0f, 750.0f, bumpMap);

	}
	// ゲームのUI
	else{
		m_gui->Render(m_timer, m_stock, m_player, m_camera, m_damageData);
	}
}