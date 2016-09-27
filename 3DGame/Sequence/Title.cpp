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

/// バトルテキストの描画位置
const float BattleTextPosY = TitlePosY;
/// オプションテキストの描画位置
const float OptionTextPosY = TitlePosY + TitleCursorAdd;
/// クレジットテキストの描画位置
const float CreditTextPosY = TitlePosY + TitleCursorAdd * 2;
/// 終了テキストの描画位置
const float EndTextPosY = TitlePosY + TitleCursorAdd * 3;

enum SubworkState
{
	/// フェード
	Fade = 0,
	/// ボイス再生
	PlayVoice,
	/// モード選択
	ModeSelect,
	/// モード決定
	ModeDecision,
	/// CPUのレベルを選択
	CPULevelSelect
};

/**
* @brief 作成
*/
void Title::Create()
{
	// インスタンスの取得
	m_2dTex = TextureManager::GetInstance();
	
	// カーソルの作成
	m_cursor = make_unique<SelectCursor>(typeid(Title));

	// プレイヤーテクスチャをコピー
	for (int i = 0; i < PLAYER_TEXTURE_MAX; i++){
		m_playerText[i] = FileData::GetInstance()->GetPlayerTexture(i);
	}

	// 数字テクスチャをコピー
	for (int i = 0; i < NUMBER_TEXTURE_MAX; i++){
		m_numTex[i] = FileData::GetInstance()->GetNumberTexture(i);
	}
}

/**
* @brief 初期化
*/
void Title::Initialize()
{
	SoundPlayer* sound = SoundPlayer::GetInstance();

	// 音楽の再生(既に再生されていたら再生しない)
	if (!sound->IsPlay(TITLE_BGM)){
		sound->LoopPlay(TITLE_BGM);
	}

	// コンピューターの数だけ配列を確保する
	int count = UniqueData::GetInstance()->GetComputerCount();
	m_computerLevel.resize(count);
	for (int i = 0; i < count; i++){
		m_computerLevel[i] = 2;
	}

	// パラメータの初期化
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
* @brief 更新
*/
void Title::Update()
{
	// パーティクルの生成
	static DWORD frame = 0;
	float x = Utility::Random(0.0f, 1280.0f);
	if (frame % 15 == 0){
		ObjectManager::GetInstance()->AddBack(make_shared<TitleParticle>(Vector2(x, 720.0f)));
	}

	// フレーム加算
	frame++;

	// タイトル更新
	UpdateTitleWork();

	// リストに登録した2Dオブジェクトを更新
	ObjectManager::GetInstance()->Update2D();
}

/**
* @brief 描画
*/
void Title::Render()
{
	// リストに登録した2Dオブジェクトを描画
	ObjectManager::GetInstance()->Render2D();

	// 背景
	m_2dTex->DrawTexture(TITLE_BACK_PNG, 640.0f, 360.0f, 0.0f, 1.0f, m_alpha);
	// タイトルロゴ
	m_2dTex->DrawTexture(TITLE_LOGO_PNG, 640.0f, 150.0f, 0.0f, 1.0f, m_alpha);
	// バトル文字
	m_2dTex->DrawTexture(BATTLE_TEXT_PNG, TitlePosX, BattleTextPosY, 0.0f, m_battleFontScale, m_alpha);
	// オプション文字
	m_2dTex->DrawTexture(OPTION_TEXT_PNG, TitlePosX, OptionTextPosY, 0.0f, m_optionFontScale, m_alpha);
	// クレジット文字
	m_2dTex->DrawTexture(CREDIT_TEXT_PNG, TitlePosX, CreditTextPosY, 0.0f, m_creditTextScale, m_alpha);
	// クレジット文字
	m_2dTex->DrawTexture(END_TEXT_PNG, TitlePosX, EndTextPosY, 0.0f, m_endTextScale, m_alpha);
	// カーソル
	m_cursor->Render(m_alpha);

	// レベル選択時だけ描画
	if (m_levelWork == 0){
		// 「CPUレベルを選択して下さい」文字
		m_2dTex->DrawTexture(CPULEVELSELECT_TEXT_PNG, 640.0f, 200.0f);
		// Level文字
		m_2dTex->DrawTexture(LEVEL_TEXT_PNG, 500.0f, 450.0f);
		// プレイヤーテクスチャ描画
		// デバイスの数を取得する(人間の数)
		int count = DeviceManager::GetInstance()->GetDeviceNum();
		m_2dTex->DrawTexture(m_playerText[m_index + count], 640.0f, 330.0f);
		// CPUのレベルを描画
		m_2dTex->DrawTexture(m_numTex[m_computerLevel[m_index]], 750.0f, 450.0f);
	}
}

/**
* @brief 終了
*/
void Title::Finalize()
{
	// 2Dオブジェクトリストをクリア
	ObjectManager::GetInstance()->Clear();

	// バトル開始が選択された時だけBGMを停止する
	if (m_isEndBGM){
		SoundPlayer::GetInstance()->Stop(TITLE_BGM);
	}
}

/**
* @brief タイトルの更新
*/
void Title::UpdateTitleWork()
{
	SoundPlayer* sound = SoundPlayer::GetInstance();
	SequenceChanger* seq = SequenceChanger::GetInstance();

	switch (m_subWork)
	{
	case Fade: // フェード終了待ち
		if (!ScreenEffect::GetInstance()->isPlayFade()){
			m_subWork++;
		}
		break;

	case PlayVoice: // ボイスの再生
		sound->Play(TITLE_VOICE);
		m_subWork++;
		break;

	case ModeSelect: // モード選択
		if (m_cursor->Update()){
			m_subWork++;
		}
		break;

	case ModeDecision:
		switch (m_cursor->GetNumber())
		{
			// メインゲーム
		case 1:
			m_subWork++;
			break;

			// オプション
		case 2:
			sound->Play(OPTION_VOICE);
			seq->ChangeScene(make_shared<Option>());
			break;

			// クレジット
		case 3:
			seq->ChangeScene(make_shared<Credit>());
			break;

			// 終了
		case 4:
			// 効果音の再生が終了したら
			if (!sound->IsPlay(DECISION_SE)){
				exit(0);
			}
			break;
		}
		break;

	case CPULevelSelect:
		if (UpdateComputerLevelSelect()){
			// ボイスの再生
			sound->Play(TITLE_START_VOICE);
			seq->ChangeScene(make_shared<GameMain>());
		}
		break;
	}
	
	// スケール
	static float scale = 1.0f;
	// 拡大フラグを切る
	if (scale >= 1.3f){
		m_isAddScale = false;
	}
	// 拡大フラグを入れる
	else if (scale <= 1.0f){
		m_isAddScale = true;
	}

	// カーソル番号ごとに拡大するフォントが異なる
	switch (m_cursor->GetNumber())
	{
	case 1:
		// 選択番号以外は初期スケールに
		m_optionFontScale = 1.0f;
		m_creditTextScale = 1.0f;
		m_endTextScale = 1.0f;

		if (m_isAddScale){
			m_battleFontScale += 0.01f;
		}
		else {
			m_battleFontScale -= 0.01f;
		}
		// スケールの記録
		scale = m_battleFontScale;
		break;

	case 2:
		// 選択番号以外は初期スケールに
		m_battleFontScale = 1.0f;
		m_creditTextScale = 1.0f;
		m_endTextScale = 1.0f;

		if (m_isAddScale){
			m_optionFontScale += 0.01f;
		}
		else {
			m_optionFontScale -= 0.01f;
		}
		// スケールの記録
		scale = m_optionFontScale;
		break;

	case 3:
		// 選択番号以外は初期スケールに
		m_optionFontScale = 1.0f;
		m_battleFontScale = 1.0f;
		m_endTextScale = 1.0f;

		if (m_isAddScale){
			m_creditTextScale += 0.01f;
		}
		else {
			m_creditTextScale -= 0.01f;
		}
		// スケールの記録
		scale = m_creditTextScale;
		break;

	case 4:
		// 選択番号以外は初期スケールに
		m_optionFontScale = 1.0f;
		m_battleFontScale = 1.0f;
		m_creditTextScale = 1.0f;

		if (m_isAddScale){
			m_endTextScale += 0.01f;
		}
		else {
			m_endTextScale -= 0.01f;
		}
		// スケールの記録
		scale = m_endTextScale;
		break;
	}
}

/**
* @brief コンピューターのレベル選択の更新
*/
bool Title::UpdateComputerLevelSelect()
{
	// インスタンスの取得
	UniqueData* data = UniqueData::GetInstance();

	// コンピューターの数を取得する
	int count = data->GetComputerCount();

	// コンピューターが存在しなかったらスルー
	if (count == 0){
		// BGM停止フラグをセットする
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

		// 左キーでレベルを減らす
		if (m_cursor->GetSelectLeft()){
			m_computerLevel[m_index]--;
		}
		// 右キーでレベルを増やす
		if (m_cursor->GetSelectRight()){
			m_computerLevel[m_index]++;
		}

		// 補正
		if (m_computerLevel[m_index] < 1){
			m_computerLevel[m_index] = 1;
		}
		if (m_computerLevel[m_index] > 2){
			m_computerLevel[m_index] = 2;
		}

		// キー入力を確認したら番号を増やす
		if (m_cursor->Update()){
			m_index++;
			
		}
	
		// 全コンピューターのレベルがセットできたら終了
		if (count <= m_index){
			m_levelWork++;
		}
		break;

	case 1:
		// BGM停止フラグをセットする
		m_isEndBGM = true;
		// コンピューターレベルをセットする
		data->SetComputerLevel(m_computerLevel);
		return true;
		break;
	}

	return false;
}
