#include <algorithm>
#include "Result.h"
#include "Title.h"
#include "SequenceChanger.h"
#include "..\DirectX\SoundPlayer.h"
#include "..\Common\StringDefine.h"

/**
* @brief 作成
*/
void Result::Create()
{
	// インスタンスの取得
	m_2dTex = TextureManager::GetInstance();
	m_fileData = FileData::GetInstance();

	// デバイスの作成
	m_dev = make_unique<InputMask>(0);

	// テクスチャをコピーする
	for (int i = 0; i < NUMBER_TEXTURE_MAX; i++){
		m_numberTex[i] = m_fileData->GetNumberTexture(i);
	}
	for (int i = 0; i < PLAYER_TEXTURE_MAX; i++){
		m_playerTextTex[i] = m_fileData->GetPlayerTexture(i);
	}
}

/**
* @brief 初期化
*/
void Result::Initialize()
{
	m_lightAngle = 0.0f;

	// 音楽の再生
	SoundPlayer::GetInstance()->LoopPlay(TITLE_BGM);
	SoundPlayer::GetInstance()->Play(CHEER_SE);

	// 対戦人数の数を取得
	int count = UniqueData::GetInstance()->GetBattleNum();

	// 順位のセット
	for (int i = 0; i < count; i++)
	{
		m_rank.emplace_back(UniqueData::GetInstance()->GetRank(i));
		m_data.emplace_back(UniqueData::GetInstance()->GetResultData(i));
	}
	
}

/**
* @brief 更新
*/
void Result::Update()
{
	// デバイスから入力を得る
	m_dev->GetInput();

	// タイトル画面に戻す
	if (UpdateResultWork()){
		SequenceChanger::GetInstance()->ChangeScene(make_shared<Title>());
	}

	// 光の角度を加算する
	m_lightAngle++;
}

/**
* @brief 描画
*/
void Result::Render()
{
	// 背景の描画
	m_2dTex->DrawTexture(TITLE_BACK_PNG, 640.0f, 360.0f, 0.0f, 1.0f, 150);

	// 光の輪の描画
	m_2dTex->DrawTexture(COLORRING_PNG, 640.0f, 100.0f);

	// 光の描画
	m_2dTex->DrawTexture(LIGHT_EFFECT_PNG, 640.0f, 100.0f, m_lightAngle, 1.25f, 255, true);

	// Winner文字の描画
	m_2dTex->DrawTexture(RESULT_WINNER_PNG, 640.0f, 360.0f, 0.0f, 1.0f, 255);

	// プレイヤーテキストの描画
	m_2dTex->DrawTexture(m_playerTextTex[m_rank[0]], 640.0f, 300.0f, 0.0f, 1.5f);
}

/**
* @brief 終了
*/
void Result::Finalize()
{
	m_rank.clear();
}

/**
* @brief リザルトの更新
*/
bool Result::UpdateResultWork()
{
	// 入力を確認したら
	if (m_dev->CheckInputMask(INPUT_01)){
		return true;
	}

	return false;
}