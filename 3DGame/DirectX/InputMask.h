/**
* @file   InputMask.h
* @brief  �f�o�C�X�̓��͏����Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___INPUTMASK_H
#define ___INPUTMASK_H

#include "DeviceManager.h"

/**
* @class InputMask
* @brief ���͂𐧌䂷��N���X
*/
class InputMask
{
public:
	//�R���X�g���N�^
	InputMask(){}
	/**
	* @param ���̓f�o�C�X��ID
	*/
	InputMask(int deviceID);
	/**
	* @param �f�X�g���N�^
	*/
	~InputMask();

	/**
	* @brief ���������s��
	*/
	void Initialize(int deviceID);

	/**
	* @brief ���͂��󂯎��
	* @return true:�����@false:���s
	*/
	bool GetInput();

	/**
	* @brief �����ɓn���ꂽ���͂����邩�ǂ����𔻒肷��
	* @param ���̓f�[�^ (INPUT_01 | INPUT_02)�݂����Ȋ����œn������
	* @return true:���͂��� false:���͂Ȃ�
	*/
	bool CheckInput(unsigned int FLAG);

	/**
	* @brief �����ɓn���ꂽ���͂����邩�ǂ����𔻒肷��(���ׂĂ��}�X�N����B�A���œ��͂���Ă�����͓̂��͂Ƃ��Ȃ�)
	* @param ���̓f�[�^ (INPUT_01 | INPUT_02)�݂����Ȋ����œn������
	* @return true:���͂��� false:���͂Ȃ�
	*/
	bool CheckInputMask(unsigned int FLAG);

	/**
	* @brief ���̓f�[�^�����̂܂ܓ���
	*/
	unsigned int GetInputState();

	/**
	* @brief �W���C�p�b�h���X�e�B�b�N�w���W�擾����
	*/
	int GetJoyStickLPosX();

	/**
	* @brief �W���C�p�b�h���X�e�B�b�N�x���W�擾����
	*/
	int GetJoyStickLPosY();

	/**
	* @brief �X�e�B�b�N���e���ꂽ���ǂ���
	*/
	bool GetJoyStickRepel();

	/**
	* @brief ���̓f�o�C�X�̎�ނ��擾����
	* @return ���̓f�o�C�X�̎��(�L�[�{�[�h or �W���C�X�e�B�b�N)
	*/
	DeviceType GetDeviceType();

	/**
	* @brief �p�b�h�f�[�^���N���A����
	*/
	void DataClear();

	/**
	* @brief �͂����f�[�^���N���A����
	*/
	void RepelClear();

protected:
	/// ���̓f�o�C�X��ݒ肷��
	DeviceManager* m_idmInstace;

	/// ��O�̓��͂�ۑ�����
	InputBase m_PrevInput;

	/// ���݂̓��͂�ۑ�����
	InputBase m_Input;

	/// ���̓f�o�C�X��ID
	int	m_InputDeviceID;
};

/// ���̓f�o�C�X�̃X�}�[�g�|�C���^
using InputPtr = shared_ptr <InputMask>;

#endif
