#include "Option.h"
#include "Title.h"
#include "SequenceChanger.h"

/// 対戦人数テキストの描画位置X
#define BATTLENUM_TEXT_POSX 450.0f
/// 対戦人数テキストの描画位置Y
#define BATTLENUM_TEXT_POSY 200.0f
/// 対戦モードの描画位置X
#define BATTLEMODE_TEXT_POSX 450.0f
/// 対戦モードの描画位置Y
#define BATTLEMODE_TEXT_POSY 400.0f
/// スペシャルバトルの描画位置
#define SPECIALBATTLE_TEXT_POSX 450.0f
/// スペシャルバトルの描画位置
#define SPECIALBATTLE_TEXT_POSY 600.0f

/**
* @brief 作成
*/
void Option::Create()
{
	// インスタンスの取得
	m_2dTex = TextureManager::GetInstance();
	m_fileData = FileData::GetInstance();

	// カーソルの作成
	m_cursor = make_unique<SelectCursor>(typeid(Option));

	// 数字テクスチャをコピー
	for (int i = 0; i < NUMBER_TEXTURE_MAX; i++){
		m_numTex[i] = m_fileData->GetNumberTexture(i);
	}
}

/**
* @brief 初期化
*/
void Option::Initialize()
{
	UniqueData* Udata = UniqueData::GetInstance();

	// 登録されているデータの取得
	m_battleNum = Udata->GetBattleNum();
	m_battleMode = Udata->GetBattleMode();
	m_isSpecialBattle = Udata->GetSpecialBattleFlag();

	// 表示フラグの初期化
	m_isLeft = false;
	m_isRight = false;
}

/**
* @brief 更新
*/
void Option::Update()
{
	// 右入力
	if (m_cursor->GetSelectRight())
	{
		// カーソルの番号
		switch (m_cursor->GetNumber())
		{
		// 対戦人数
		case SettingBattleNum:
			// カウントを増やす
			m_battleNum++;

			// 対戦人数は最高 4人
			if (m_battleNum > 4){
				m_battleNum = 4;
			}
			break;

		// バトルモード
		case SettingBattleMode:
			m_battleMode = BattleMode::Stock;
			break;

			// スペシャルバトル
		case SettingSpecialBattle:
			m_isSpecialBattle = true;
			break;
		}
	}

	// 左入力
	else if (m_cursor->GetSelectLeft())
	{
		switch (m_cursor->GetNumber())
		{
			// 対戦人数
		case SettingBattleNum:
			// カウントを減らす
			m_battleNum--;

			// 対戦人数は最低 2人
			if (m_battleNum < 2){
				m_battleNum = 2;
			}
			break;
	
			// バトルモード
		case SettingBattleMode:
			m_battleMode = BattleMode::Time;
			break;

			// スペシャルバトル
		case SettingSpecialBattle:
			m_isSpecialBattle = false;
			break;
		}
	}

	// 矢印アイコン表示フラグの更新
	UpdateDisplayArrowIcon();

	UniqueData* Udata = UniqueData::GetInstance();
	// コンピューターの数 = 対戦人数 - デバイスの個数
	int computerCount = m_battleNum - DeviceManager::GetInstance()->GetDeviceNum();
	// 0以下にはならない
	if (computerCount < 0) computerCount = 0;
	// コンピューターの数をセット
	Udata->SetComputerCount(computerCount);
	// 対戦人数のセット
	Udata->SetBattleNum(m_battleNum);
	// バトルモードのセット
	Udata->SetBattleMode(m_battleMode);
	// スペシャルバトルフラグのセット
	Udata->SetSpecialBattleFlag(m_isSpecialBattle);

	// タイトル画面に戻す
	if (m_cursor->Update()){
		SequenceChanger::GetInstance()->ChangeScene(make_shared<Title>());
	}
}

/**
* @brief 描画
*/
void Option::Render()
{
	// 背景はアルファ値を下げて描画する
	m_2dTex->DrawTexture(TITLE_BACK_PNG, 640.0f, 360.0f, 0.0f, 1.0f, 130);

	// カーソル
	m_cursor->Render();

	// 対戦人数テキスト
	m_2dTex->DrawTexture(BATTLENUM_TEXT_PNG, BATTLENUM_TEXT_POSX, BATTLENUM_TEXT_POSY);
	
	// 対戦人数数字
	m_2dTex->DrawTexture(m_numTex[m_battleNum], BATTLENUM_TEXT_POSX + 500.0f, BATTLENUM_TEXT_POSY);
	
	// バトルモード
	m_2dTex->DrawTexture(BATTLEMODE_TEXT_PNG, BATTLEMODE_TEXT_POSX, BATTLEMODE_TEXT_POSY);

	// タイム文字の描画
	if (m_battleMode == BattleMode::Time){
		m_2dTex->DrawTexture(TIME_TEXT_PNG, BATTLEMODE_TEXT_POSX + 500.0f, BATTLEMODE_TEXT_POSY);
	}
	// ストック文字の描画
	else if (m_battleMode == BattleMode::Stock){
		m_2dTex->DrawTexture(STOCK_TEXT_PNG, BATTLEMODE_TEXT_POSX + 500.0f, BATTLEMODE_TEXT_POSY);
	}

	// スペシャルバトル文字
	m_2dTex->DrawTexture(SPECIALBATTLE_TEXT_PNG, SPECIALBATTLE_TEXT_POSX , SPECIALBATTLE_TEXT_POSY);

	// "ON"OFF"の描画
	if (m_isSpecialBattle){
		m_2dTex->DrawTexture(ON_TEXT_PNG, SPECIALBATTLE_TEXT_POSX + 500.0f, SPECIALBATTLE_TEXT_POSY);
	}
	else{
		m_2dTex->DrawTexture(OFF_TEXT_PNG, SPECIALBATTLE_TEXT_POSX + 500.0f, SPECIALBATTLE_TEXT_POSY);
	}

	// 矢印をテキストの左側に描画
	if (m_isLeft){
		m_2dTex->DrawTexture(ARROW_PNG, BATTLEMODE_TEXT_POSX + 330.0f, m_cursorY, -90.0f, 0.8f);
	}
	// 矢印をテキストの右側に描画
	if (m_isRight){
		m_2dTex->DrawTexture(ARROW_PNG, BATTLEMODE_TEXT_POSX + 670.0f, m_cursorY, 90.0f, 0.8f);
	}
}

/**
* @brief 終了
*/
void Option::Finalize()
{

}

/**
* @brief 矢印アイコンの表示フラグを更新
*/
void Option::UpdateDisplayArrowIcon()
{
	// カーソルのY座標を取得する
	m_cursorY = m_cursor->GetPos().y;

	// 矢印アイコンの表示フラグを各種設定する
	switch (m_cursor->GetNumber())
	{
	case SettingBattleNum:
		if (m_battleNum == 2){
			m_isLeft = false;
			m_isRight = true;
		}
		if (m_battleNum == 3){
			m_isLeft = true;
			m_isRight = true;
		}
		if (m_battleNum == 4){
			m_isLeft = true;
			m_isRight = false;
		}
		break;

	case SettingBattleMode:
		if (m_battleMode == BattleMode::Time){
			m_isLeft = false;
			m_isRight = true;
		} 
		if (m_battleMode == BattleMode::Stock){
			m_isLeft = true;
			m_isRight = false;
		}
		break;

	case SettingSpecialBattle:
		if (m_isSpecialBattle){
			m_isLeft = true;
			m_isRight = false;
		}
		else{
			m_isLeft = false;
			m_isRight = true;
		}
		break;
	}
}