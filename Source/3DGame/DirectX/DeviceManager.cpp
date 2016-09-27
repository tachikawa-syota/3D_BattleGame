#include "DeviceManager.h"

/**
* @brief 初期化
*/
HRESULT DeviceManager::Initialize()
{
	HRESULT hr;
	// 「DirectInput」オブジェクトの作成
	hr = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDI, NULL);
	
	if (FAILED(hr)){
		// 作成失敗
		return E_FAIL;
	}

	// 最初は個数０
	m_deviceNum = 0;

	// 成功
	return S_OK;
}

/**
* @brief デバイスの作成と設定をする
*/
HRESULT DeviceManager::SetUp()
{
	// 入力デバイスの作成
	int temp = 0;
	int joyNum = 0;

	// ジョイスティックを作成する
	for (int i = 0; i < JOY_MAX; i++){
		if (FAILED(CreateDevice(DeviceType::JoyStick, &temp))){
			return E_FAIL;
		}
		// ジョイスティックのカウントを増やす
		joyNum++;
	}


	// キーボードを作成する(キーボードは一つしか作成しない)
	if (FAILED(CreateDevice(DeviceType::KeyBoard, &temp))){
		return E_FAIL;
	}

	// 成功
	return S_OK;
}

/**
* @brief デバイスの作成
* @param deviceType - デバイスの種類
* @param id - デバイスのハンドルID
* @return true - 作成成功　: false - 失敗
*/
BOOL DeviceManager::CreateDevice(DeviceType deviceType, int* id)
{
	IInputDevice* temp = nullptr;

	switch (deviceType)
	{
	case DeviceType::JoyStick:
		temp = new JoyStick();
		break;

	case DeviceType::KeyBoard:
		temp = new KeyBoard(m_pDI);
		break;
	}

	// 作成されたデバイスの初期化を行う
	if (temp->Initialize() == E_FAIL){
		*id = -1;
		SafeDelete(temp);
		// 失敗
		return FALSE;
	}

	// 初期化に成功(要素を追加する)
	// ジョイスティックの場合は終端に追加
	if (deviceType == DeviceType::JoyStick){
		m_pDevice.emplace_back(temp);
	}
	// キーボードの場合は先頭に追加(1Pをキーボードに設定するために)
	else if (deviceType == DeviceType::KeyBoard){
		auto it = m_pDevice.begin();
		m_pDevice.insert(it, temp);
	}

	// カウントを増やす
	*id = m_deviceNum++;

	// 成功
	return TRUE;
}

/**
* @brief デバイスから入力を取得
*/
bool DeviceManager::GetInputFromDevice(InputBase* pIB ,int idx)
{
	// エラー
	if (idx < 0 || idx >= m_deviceNum) return false;

	// 入力を得る
	m_pDevice[idx]->GetInput(pIB);

	// 成功
	return true;
}

/**
* @brief 作成されたデバイスの個数を取得
*/
int DeviceManager::GetDeviceNum()
{
	return m_deviceNum;
}