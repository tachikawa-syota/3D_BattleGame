/**
* @file   DeviceManager.h
* @brief  ���̓f�o�C�X�̊Ǘ�������
* @author Syota Tachikawa
*/

#ifndef ___DEVICEMANAGER_H
#define ___DEVICEMANAGER_H

#include "InputLib.h"

/**
* @brief ���̓f�o�C�X���Ǘ�����N���X
*/
class DeviceManager
{
public:
	/**
	* @brief �Q�b�g�C���X�^���X
	*/
	static DeviceManager* GetInstance() {
		static DeviceManager singleton;
		return &singleton;
	}

	/**
	* @brief ������
	*/
	HRESULT Initialize();

	/**
	* @brief �f�o�C�X�̍쐬�Ɛݒ������
	*/
	HRESULT SetUp();

	/**
	* @brief �f�o�C�X�̍쐬
	*/
	BOOL CreateDevice(DeviceType deviceType, int* id);
	
	/**
	* @brief �f�o�C�X������͂��擾
	*/
	bool GetInputFromDevice(InputBase* pIB ,int idx);

	/**
	* @brief �쐬���ꂽ�f�o�C�X�̌����擾
	*/
	int GetDeviceNum();

private:
	/// �f�o�C�X��ID
	int m_deviceId;

	/// �f�o�C�X�̐�
	int m_deviceNum;
	
	/// �f�o�C�X
	vector<IInputDevice*> m_pDevice;
	
	LPDIRECTINPUT8 m_pDI;
};

#endif