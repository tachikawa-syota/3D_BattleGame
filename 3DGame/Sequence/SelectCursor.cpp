#include "SelectCursor.h"
#include "Title.h"
#include "Option.h"

/**
* @brief コンストラクタ
*/
SelectCursor::SelectCursor(const type_info& type)
{
	// インスタンスを取得
	m_2dTex = TextureManager::GetInstance();

	// デバイスの作成
	m_dev = make_unique<InputMask>(0);

	// タイプによってパラメータを設定
	if (type == typeid(Title)){
		m_cursorPos = Vector2(750.0f, TitlePosY);
		m_cursorAdd = TitleCursorAdd;
		m_selectMax = 4;
	}
	else if (type == typeid(Option)){
		m_cursorPos = Vector2(100.0f, 200.0f);
		m_cursorAdd = 200.0f;
		m_selectMax = 3;
	}

	// 初期のカーソルの番号は"1"
	m_cursorNumber = 1;
	m_oldCursorNumber = 1;
}

/**
* @brief デストラクタ
*/
SelectCursor::~SelectCursor()
{

}

/**
* @brief 更新
*/
bool SelectCursor::Update()
{
	// 毎回フラグを初期化
	for (int i = 0; i < 2; i++){
		m_selectFlag[i] = false;
	}

	// デバイスから入力を取得する
	m_dev->GetInput();

	// 下移動
	if (m_dev->CheckInputMask(INPUT_DOWN)){
		m_cursorNumber++;
	}
	// 上移動
	if (m_dev->CheckInputMask(INPUT_UP)){
		m_cursorNumber--;
	}

	// 右入力
	if (m_dev->CheckInputMask(INPUT_RIGHT)){
		m_selectFlag[0] = true;
	}
	// 左入力
	if (m_dev->CheckInputMask(INPUT_LEFT)){
		m_selectFlag[1] = true;
	}

	// カーソルが下に移動したら
	if (m_cursorNumber > m_oldCursorNumber){
		m_cursorPos.y += m_cursorAdd;
	}
	// カーソルが上に移動したら
	else if (m_cursorNumber < m_oldCursorNumber){
		m_cursorPos.y -= m_cursorAdd;
	}

	// 上端を越えたら
	if (m_cursorNumber < 1){
		// 下端に移動
		m_cursorPos.y += (m_cursorAdd * m_selectMax);
		m_cursorNumber = m_selectMax;
	}
	// 下端を越えたら
	if (m_cursorNumber > m_selectMax){
		// 上端に移動
		m_cursorPos.y -= (m_cursorAdd * m_selectMax);
		m_cursorNumber = 1;
	}

	// 情報登録
	m_oldCursorNumber = m_cursorNumber;

	// 決定キー
	if (m_dev->CheckInputMask(INPUT_01))
	{
		// 効果音を再生
		SoundPlayer::GetInstance()->Play(DECISION_SE);
		return true;
	}

	return false;
}

/**
* @brief 描画
*/
void SelectCursor::Render(int alpha)
{
	m_2dTex->DrawTexture(CURSOL_PNG, m_cursorPos.x, m_cursorPos.y, 0.0f, 1.0f, alpha);
}

/**
* @brief カーソル座標を取得する
*/
const Vector2& SelectCursor::GetPos() const
{
	return m_cursorPos;
}

/**
* @brief カーソル番号の取得
*/
int SelectCursor::GetNumber() const
{
	return m_cursorNumber;
}

/**
* @brief 右の入力を取る(Optionでしか使わない)
*/
int SelectCursor::GetSelectRight() const
{
	return m_selectFlag[0];
}

/**
* @brief 左の入力を取る(Optionでしか使わない)
*/
int SelectCursor::GetSelectLeft() const
{
	return m_selectFlag[1];
}

/**
* @brief 入力デバイスを取得する
*/
const InputPtr& SelectCursor::GetInputDevice()
{
	return m_dev;
}