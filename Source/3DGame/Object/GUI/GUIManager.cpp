#include "GUIManager.h"
#include "..\..\GameSystem\Timer.h"
#include "..\..\GameSystem\Stock.h"
#include "..\..\Object\Camera.h"
#include "..\..\Common\CommonParam.h"
#include "..\..\Common\UniqueData.h"

/**
* @brief コンストラクタ
*/
GUI::GUI(int playerCount)
{
	// インスタンスの取得
	m_2dTex = TextureManager::GetInstance();
	m_fileData = FileData::GetInstance();

	// プレイヤーの数を登録
	m_playerCount = playerCount;

	// 数字テクスチャをコピーする
	for (int i = 0; i < NUMBER_TEXTURE_MAX; i++){
		m_numberTex[i] = m_fileData->GetNumberTexture(i);
	}
	// プレイヤーテキストテクスチャをコピーする
	for (int i = 0; i < PLAYER_TEXTURE_MAX; i++){
		m_playerTextTex[i] = m_fileData->GetPlayerTexture(i);
	}

	// パラメータの初期化
	m_fightAnimation = 0;

	// プレイヤーテキスト描画位置のセット
	m_playerTextPos.clear();
	float addX = 0.0f;
	for (int i = 0; i < m_playerCount; i++){
		// 初期化する
		Vector2 pos = Vector2(0.0f, 650.0f);
		// ループ毎に250ずつ座標を加算
		addX += 250.0f;
		pos.x = addX;
		// 登録
		m_playerTextPos.emplace_back(pos);
	}

	// ストックの描画位置の初期化
	// 先に描画位置データをクリアする
	m_stockPos.clear();
	// ダメージ描画位置
	float add2X = 0.0f;
	for (int i = 0; i < m_playerCount; i++){
		Vector2 pos = Vector2(0.0f, 600.0f);
		add2X += 250.0f;
		pos.x = add2X;
		m_stockPos.emplace_back(pos);
	}

	// 先に描画位置データをクリアする
	m_damagePos.clear();
	// ダメージ描画位置
	float addX3 = 0.0f;
	for (int i = 0; i < m_playerCount; i++){
		Vector2 pos = Vector2(0.0f, 650.0f);
		addX3 += 250.0f;
		pos.x = addX3;
		m_damagePos.emplace_back(pos);
	}
}

/**
* @brief デストラクタ
*/
GUI::~GUI()
{

}

/**
* @brief 描画
*/
void GUI::Render(shared_ptr<Timer>& time, 
                 shared_ptr<Stock>& stock, 
	             vector<IPlayerPtr> player, 
				 CameraPtr camera,
	             DamageData damage)
{
	// タイマーの描画
	if (UniqueData::GetInstance()->GetBattleMode() == BattleMode::Time){
		m_2dTex->DrawTexture(m_numberTex[time->GetTenId()], 620.0f, 70.0f, 0.0f, 0.7f);
		m_2dTex->DrawTexture(m_numberTex[time->GetOneId()], 660.0f, 70.0f, 0.0f, 0.7f);
	}
	// ストックの描画
	else if (UniqueData::GetInstance()->GetBattleMode() == BattleMode::Stock){
		for (int i = 0; i < m_playerCount; i++){
			m_2dTex->DrawTexture(STOCK_PNG, m_stockPos[i].x, m_stockPos[i].y, 0.0f, 0.5f);
			m_2dTex->DrawTexture(m_numberTex[stock->GetStock(i)], m_stockPos[i].x + 30.0f, m_stockPos[i].y, 0.0f, 0.5f);
		}
	}

	// ダメージ率の描画
	for (int i = 0; i < m_playerCount; i++){
		m_2dTex->DrawTexture(m_numberTex[damage.GetHundredId(i)], m_damagePos[i].x + 50.0f, 650.0f, 0.0f, 0.4f);
		m_2dTex->DrawTexture(m_numberTex[damage.GetTenId(i)], m_damagePos[i].x + 80.0f, 650.0f, 0.0f, 0.4f);
		m_2dTex->DrawTexture(m_numberTex[damage.GetOneId(i)], m_damagePos[i].x + 110.0f, 650.0f, 0.0f, 0.4f);
	}

	// プレイヤーテキスト描画
	for (int i = 0; i != m_playerTextPos.size(); i++){
		m_2dTex->DrawTexture(m_playerTextTex[i], m_playerTextPos[i].x, m_playerTextPos[i].y, 0.0f, 0.5f);
	}

	// プレイヤーの頭上に番号を描画
	for (int i = 0; i != m_playerTextPos.size(); i++){
		// リタイアしてるプレイヤーの番号は描画しない
		if (player[i]->IsRetire()) continue;
		// 座標取得
		Vector3 pos = player[i]->GetPos();
		// 頭上に座標を調整
		pos.y = pos.y + 2.7f;
		// ビルボードで描画
		m_2dTex->Draw3DTexture(camera->GetInverseView(), m_playerTextTex[i], pos, Vector3(), 0.01f, ALPHA_MAX);
		m_2dTex->Draw3DTexture(camera->GetInverseView(), m_playerTextTex[i], pos, Vector3(), 0.01f, ALPHA_MAX);
	}
}