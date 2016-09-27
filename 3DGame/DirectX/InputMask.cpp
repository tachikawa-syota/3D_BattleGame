#include "InputMask.h"

/**
* @brief コンストラクタ
*/
InputMask::InputMask(int deviceID)
{
	Initialize(deviceID);
}

/**
* @brief デストラクタ
*/
InputMask::~InputMask()
{
}

/**
* @brief 初期化
*/
void InputMask::Initialize(int deviceID)
{
	// デバイスの番号を得る
	m_InputDeviceID = deviceID;

	// インプットデバイスマネージャーの参照を得る
	m_idmInstace = DeviceManager::GetInstance();

	// 一度入力を取得しておく
	m_idmInstace->GetInputFromDevice(&m_PrevInput, m_InputDeviceID);
	m_idmInstace->GetInputFromDevice(&m_Input, m_InputDeviceID);
}

/**
* @brief 入力を受け取る
* @return true:成功　false:失敗
*/
bool InputMask::GetInput()
{
	// 一つ前の入力を保存
	m_PrevInput.flag = m_Input.flag;
	return m_idmInstace->GetInputFromDevice(&m_Input, m_InputDeviceID);
}

/**
* @brief 引数に渡された入力があるかどうかを判定する
* @param 入力データ (INPUT_01 | INPUT_02)みたいな感じで渡すこと
* @return true:入力あり false:入力なし
*/
bool InputMask::CheckInput(unsigned int FLAG)
{
	return (m_Input.flag & FLAG) ? true : false;
}

/**
* @brief 引数に渡された入力があるかどうかを判定する(すべてをマスクする。連続で入力されているものは入力としない)
* @param 入力データ (INPUT_01 | INPUT_02)みたいな感じで渡すこと
* @return true:入力あり false:入力なし
*/
bool InputMask::CheckInputMask(unsigned int FLAG)
{
	// 二段階で行う
	// まず現在の入力との判定
	if (m_Input.flag & FLAG)
	{
		// 次に現在の入力と一つ前の入力の排他的論理和との判定
		if ((m_Input.flag ^ m_PrevInput.flag) & FLAG){
			return true;
		}
	}
	return false;
}

/**
* @brief 入力データをそのまま得る
*/
unsigned int InputMask::GetInputState()
{
	return m_Input.flag;
}

/**
* @brief スティックが弾かれたかどうか
*/
bool InputMask::GetJoyStickRepel()
{
	return m_Input.isRepel;
}

/**
* @brief ジョイパッド左スティックＹ座標取得
*/
int InputMask::GetJoyStickLPosY()
{
	return m_Input.iJoy.lY;
}

/**
* @brief ジョイパッド左スティックＸ座標取得
*/
int InputMask::GetJoyStickLPosX()
{
	return m_Input.iJoy.lX;
}

/**
* @brief デバイスの入力情報を消去する
*/
void InputMask::DataClear()
{
	m_Input.flag = NULL;
	m_Input.iJoy.lX = 0;
	m_Input.iJoy.lY = 0;
}

/**
* @brief はじきデータをクリアする
*/
void InputMask::RepelClear()
{
	m_Input.isRepel = false;
	m_Input.isDownX = false;
	m_Input.isDownY = false;
}