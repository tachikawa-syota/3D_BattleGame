/**
* @file   DeviceManager.h
* @brief  入力デバイスの管理をする
* @author Syota Tachikawa
*/

#ifndef ___DEVICEMANAGER_H
#define ___DEVICEMANAGER_H

#include "InputLib.h"

/**
* @brief 入力デバイスを管理するクラス
*/
class DeviceManager
{
public:
	/**
	* @brief ゲットインスタンス
	*/
	static DeviceManager* GetInstance() {
		static DeviceManager singleton;
		return &singleton;
	}

	/**
	* @brief 初期化
	*/
	HRESULT Initialize();

	/**
	* @brief デバイスの作成と設定をする
	*/
	HRESULT SetUp();

	/**
	* @brief デバイスの作成
	*/
	BOOL CreateDevice(DeviceType deviceType, int* id);
	
	/**
	* @brief デバイスから入力を取得
	*/
	bool GetInputFromDevice(InputBase* pIB ,int idx);

	/**
	* @brief 作成されたデバイスの個数を取得
	*/
	int GetDeviceNum();

private:
	/// デバイスのID
	int m_deviceId;

	/// デバイスの数
	int m_deviceNum;
	
	/// デバイス
	vector<IInputDevice*> m_pDevice;
	
	LPDIRECTINPUT8 m_pDI;
};

#endif