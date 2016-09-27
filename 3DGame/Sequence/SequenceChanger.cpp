#include "SequenceChanger.h"
#include "ScreenEffect.h"

/// フェードをかけるフレーム
#define FADE_FRAME 90

/**
* @brief 初期化
*/
void SequenceChanger::Initialize()
{
	m_nowScene = nullptr;
}

/**
* @brief シーンの切り替え
* @param 切り替えたいシーンを入れる
*/
void SequenceChanger::ChangeScene(const ISequencePtr& next)
{
	// 現在のシーンの終了処理
	if (m_nowScene != nullptr){
		m_nowScene->Finalize();
	}

	// シーンの切り替え
	m_nowScene = next;
	
	// フェードセット
	ScreenEffect::GetInstance()->SetFade(Fade::In, FadeColor::Black, FADE_FRAME);

	// シーンの作成
	m_nowScene->Create();

	// シーンの初期化
	m_nowScene->Initialize();
}

/**
* @brief 現在のシーンの取得
*/
const ISequencePtr& SequenceChanger::GetNowScene() const
{
	return m_nowScene;
}