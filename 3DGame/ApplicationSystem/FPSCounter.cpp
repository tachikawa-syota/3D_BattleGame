#include "FPSCounter.h"

/**
* @brief コンストラクタ
* @param FPSモードの指定
*/
FPSCounter::FPSCounter(FPSMode fpsMode)
{
	// FPSモードの設定
	m_FPSMode = fpsMode;
	
	// ゲームフレームの初期化
	m_dwGameFrame = 0;

	// フレームタイムの初期化
	m_dwFrameTime = clock();
}

/**
* @brief デストラクタ
*/
FPSCounter::~FPSCounter()
{

}

BOOL FPSCounter::Update()
{
	static DWORD dwSec = 0;
	DWORD CurrentTime = clock() * 10;

	// フレーム制限
	if (CurrentTime < m_dwFrameTime + 167) {
		return FALSE;
	}

	// 経過時間
	DWORD dTime = CurrentTime - m_dwFrameTime;
	if (dTime > 2000) m_dwFrameTime = CurrentTime;

	// スキップタイプ 
	switch (m_FPSMode){
	case FPSMode::FPS_60:	bRender = TRUE;	break;
	case FPSMode::FPS_30:	if (m_dwGameFrame & 0x01) bRender = TRUE; else bRender = FALSE;
		break;
	case FPSMode::FPS_FLEX:	if (dTime > 167 * 2) bRender = FALSE; else bRender = TRUE;
		break;
	}

	// フレーム時間更新
	if (GetKeyState(VK_LCONTROL) < 0) m_dwFrameTime += 300;
	m_dwFrameTime += 167;

	// 秒間フレーム数保存
	if (dwSec < CurrentTime){
		m_dwFPS = m_dwCurFrame;
		m_dwRenderFPS = m_dwRCurFrame;
		m_dwCurFrame = m_dwRCurFrame = 0;
		dwSec += 10000;
	}
	// フレーム数更新
	m_dwCurFrame++;

	// ゲームフレーム数更新
	m_dwGameFrame++;

	// 描画フレーム数更新
	if (bRender) m_dwRCurFrame++;

	return TRUE;
}


/**
* @brief 描画フラグの取得
*/
BOOL FPSCounter::IsRender()
{
	return bRender;
}

/**
* @brief フレームレートの取得
*/
DWORD FPSCounter::GetFPS()
{
	return m_dwFPS;
}

/**
* @brief 描画フレームレートの取得
*/
DWORD FPSCounter::GetRenderFPS()
{
	return m_dwRenderFPS;
}