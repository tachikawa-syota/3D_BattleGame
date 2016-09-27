#include "InputMask.h"

/**
* @brief �R���X�g���N�^
*/
InputMask::InputMask(int deviceID)
{
	Initialize(deviceID);
}

/**
* @brief �f�X�g���N�^
*/
InputMask::~InputMask()
{
}

/**
* @brief ������
*/
void InputMask::Initialize(int deviceID)
{
	// �f�o�C�X�̔ԍ��𓾂�
	m_InputDeviceID = deviceID;

	// �C���v�b�g�f�o�C�X�}�l�[�W���[�̎Q�Ƃ𓾂�
	m_idmInstace = DeviceManager::GetInstance();

	// ��x���͂��擾���Ă���
	m_idmInstace->GetInputFromDevice(&m_PrevInput, m_InputDeviceID);
	m_idmInstace->GetInputFromDevice(&m_Input, m_InputDeviceID);
}

/**
* @brief ���͂��󂯎��
* @return true:�����@false:���s
*/
bool InputMask::GetInput()
{
	// ��O�̓��͂�ۑ�
	m_PrevInput.flag = m_Input.flag;
	return m_idmInstace->GetInputFromDevice(&m_Input, m_InputDeviceID);
}

/**
* @brief �����ɓn���ꂽ���͂����邩�ǂ����𔻒肷��
* @param ���̓f�[�^ (INPUT_01 | INPUT_02)�݂����Ȋ����œn������
* @return true:���͂��� false:���͂Ȃ�
*/
bool InputMask::CheckInput(unsigned int FLAG)
{
	return (m_Input.flag & FLAG) ? true : false;
}

/**
* @brief �����ɓn���ꂽ���͂����邩�ǂ����𔻒肷��(���ׂĂ��}�X�N����B�A���œ��͂���Ă�����͓̂��͂Ƃ��Ȃ�)
* @param ���̓f�[�^ (INPUT_01 | INPUT_02)�݂����Ȋ����œn������
* @return true:���͂��� false:���͂Ȃ�
*/
bool InputMask::CheckInputMask(unsigned int FLAG)
{
	// ��i�K�ōs��
	// �܂����݂̓��͂Ƃ̔���
	if (m_Input.flag & FLAG)
	{
		// ���Ɍ��݂̓��͂ƈ�O�̓��͂̔r���I�_���a�Ƃ̔���
		if ((m_Input.flag ^ m_PrevInput.flag) & FLAG){
			return true;
		}
	}
	return false;
}

/**
* @brief ���̓f�[�^�����̂܂ܓ���
*/
unsigned int InputMask::GetInputState()
{
	return m_Input.flag;
}

/**
* @brief �X�e�B�b�N���e���ꂽ���ǂ���
*/
bool InputMask::GetJoyStickRepel()
{
	return m_Input.isRepel;
}

/**
* @brief �W���C�p�b�h���X�e�B�b�N�x���W�擾
*/
int InputMask::GetJoyStickLPosY()
{
	return m_Input.iJoy.lY;
}

/**
* @brief �W���C�p�b�h���X�e�B�b�N�w���W�擾
*/
int InputMask::GetJoyStickLPosX()
{
	return m_Input.iJoy.lX;
}

/**
* @brief �f�o�C�X�̓��͏�����������
*/
void InputMask::DataClear()
{
	m_Input.flag = NULL;
	m_Input.iJoy.lX = 0;
	m_Input.iJoy.lY = 0;
}

/**
* @brief �͂����f�[�^���N���A����
*/
void InputMask::RepelClear()
{
	m_Input.isRepel = false;
	m_Input.isDownX = false;
	m_Input.isDownY = false;
}