#include "ScreenEffect.h"
#include "..\DirectX\Direct3DData.h"

/**
* @brief 初期化
*/
void ScreenEffect::Initialize()
{
	m_add = 0;
	m_color = FadeColor::Black;
	m_frame = 0;
	m_fade_cnt = 0;
	m_state = Fade::None;
}

/**
* @brief フェードの設定
* @param type - フェードの種類(In または Out)
* @param color - フェード色
* @param frame - フェートしてるフレーム
*/
bool ScreenEffect::SetFade(Fade type, FadeColor color, int frame)
{
	// フェードの種類
	switch (type)
	{
		//フェードイン
	case Fade::In:
		m_fade_cnt = frame;
		m_add = -1;
		break;
		//フェードアウト
	case Fade::Out:
		m_fade_cnt = 0;
		m_add = 1;
		break;
		//フェードなし
	default:
		m_add = 0;
		break;
	}
	m_color = color;
	m_frame = frame;
	m_state = type;

	// フェードが掛かっていなかったら
	if (m_state == Fade::None){
		return false; 
	}

	return true;
}

/**
* @brief フェード実行中かをチェック
* @return None以外だとtrueが返る
*/
bool ScreenEffect::isPlayFade()
{
	if (m_state == Fade::None){
		return false;
	}
	else{
		return true;
	}
}

/**
* @brief 更新
*/
void ScreenEffect::Update()
{
	// フェードが掛かっていなかったら何もしない
	if (m_state == Fade::None) return;

	//フレームの加算
	m_fade_cnt += m_add;
	//フェードアウト終了
	if (m_fade_cnt > m_frame){
		m_fade_cnt = m_frame;
		m_state = Fade::None;
	}
	//フェードイン終了
	if (m_fade_cnt < 0){
		m_state = Fade::None;
		m_fade_cnt = 0;
		m_add = 0;
	}
}

/**
* @brief 描画
*/
void ScreenEffect::Draw()
{
	// フェードが掛かっていなかったら何もしない
	if (m_state == Fade::None) return;

	// 透過処理をするよう変更
	D3DIns->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	// 透過前の部分(下地)の設定 重ねる画像の透明度が高いほど薄くする
	D3DIns->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// 重ねる画像はその透過色を適用
	D3DIns->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	// ボックスの描画
	if (m_fade_cnt > 0)
	{
		float alpha = (float)(m_fade_cnt) / m_frame;
		// 黒アルファブレンド
		if (m_color == FadeColor::Black){
			TextureManager::GetInstance()->DrawBox(0, 0, 1280.0f, 720.0f, (D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, alpha)));
		}
		// 白アルファブレンド
		if (m_color == FadeColor::White){
			TextureManager::GetInstance()->DrawBox(0, 0, 1280.0f, 720.0f, (D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, alpha)));
		}
	}

	D3DIns->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}
