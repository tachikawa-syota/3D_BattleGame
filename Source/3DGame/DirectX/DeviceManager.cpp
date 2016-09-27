#include "DeviceManager.h"

/**
* @brief ������
*/
HRESULT DeviceManager::Initialize()
{
	HRESULT hr;
	// �uDirectInput�v�I�u�W�F�N�g�̍쐬
	hr = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDI, NULL);
	
	if (FAILED(hr)){
		// �쐬���s
		return E_FAIL;
	}

	// �ŏ��͌��O
	m_deviceNum = 0;

	// ����
	return S_OK;
}

/**
* @brief �f�o�C�X�̍쐬�Ɛݒ������
*/
HRESULT DeviceManager::SetUp()
{
	// ���̓f�o�C�X�̍쐬
	int temp = 0;
	int joyNum = 0;

	// �W���C�X�e�B�b�N���쐬����
	for (int i = 0; i < JOY_MAX; i++){
		if (FAILED(CreateDevice(DeviceType::JoyStick, &temp))){
			return E_FAIL;
		}
		// �W���C�X�e�B�b�N�̃J�E���g�𑝂₷
		joyNum++;
	}


	// �L�[�{�[�h���쐬����(�L�[�{�[�h�͈�����쐬���Ȃ�)
	if (FAILED(CreateDevice(DeviceType::KeyBoard, &temp))){
		return E_FAIL;
	}

	// ����
	return S_OK;
}

/**
* @brief �f�o�C�X�̍쐬
* @param deviceType - �f�o�C�X�̎��
* @param id - �f�o�C�X�̃n���h��ID
* @return true - �쐬�����@: false - ���s
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

	// �쐬���ꂽ�f�o�C�X�̏��������s��
	if (temp->Initialize() == E_FAIL){
		*id = -1;
		SafeDelete(temp);
		// ���s
		return FALSE;
	}

	// �������ɐ���(�v�f��ǉ�����)
	// �W���C�X�e�B�b�N�̏ꍇ�͏I�[�ɒǉ�
	if (deviceType == DeviceType::JoyStick){
		m_pDevice.emplace_back(temp);
	}
	// �L�[�{�[�h�̏ꍇ�͐擪�ɒǉ�(1P���L�[�{�[�h�ɐݒ肷�邽�߂�)
	else if (deviceType == DeviceType::KeyBoard){
		auto it = m_pDevice.begin();
		m_pDevice.insert(it, temp);
	}

	// �J�E���g�𑝂₷
	*id = m_deviceNum++;

	// ����
	return TRUE;
}

/**
* @brief �f�o�C�X������͂��擾
*/
bool DeviceManager::GetInputFromDevice(InputBase* pIB ,int idx)
{
	// �G���[
	if (idx < 0 || idx >= m_deviceNum) return false;

	// ���͂𓾂�
	m_pDevice[idx]->GetInput(pIB);

	// ����
	return true;
}

/**
* @brief �쐬���ꂽ�f�o�C�X�̌����擾
*/
int DeviceManager::GetDeviceNum()
{
	return m_deviceNum;
}