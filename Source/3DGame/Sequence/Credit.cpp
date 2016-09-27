#include "Credit.h"
#include "Title.h"
#include "SequenceChanger.h"
#include "..\Common\FileData.h"

/**
* @brief 作成
*/
void Credit::Create()
{
	// デバイスの所有権は"1P"のみ(キーボードが入る)
	m_dev = make_shared<InputMask>(0);
}

/**
* @brief 初期化
*/
void Credit::Initialize()
{

}

/**
* @brief 更新
*/
void Credit::Update()
{
	// デバイスから入力を得る
	m_dev->GetInput();

	// ボタン入力されたら
	if (m_dev->CheckInputMask(INPUT_01)){
		// 効果音を再生
		SoundPlayer::GetInstance()->Play(DECISION_SE);
		// タイトル画面に戻る
		SequenceChanger::GetInstance()->ChangeScene(make_shared<Title>());
	}
}

/**
* @brief 描画
*/
void Credit::Render()
{
	// インスタンス
	TextureManager* tex = TextureManager::GetInstance();

	// クレジット画面の描画
	tex->DrawTexture(CREDIT_BACK_PNG, 640.0f, 360.0f);

	// タイトルロゴの描画
	tex->DrawTexture(TITLE_LOGO_PNG, 640.0f, 50.0f, 0.0f, 0.5f);
}

/**
* @brief 終了
*/
void Credit::Finalize()
{

}