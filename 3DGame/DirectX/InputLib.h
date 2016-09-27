/**
* @file   InputLib.h
* @brief  ���̓f�o�C�X�̊Ǘ�������
* @author Syota Tachikawa
*/

#ifndef ___INPUTLIB_H
#define ___INPUTLIB_H

#include <dInput.h>
#include "..\Common\CommonLib.h"

// �W���C�X�e�B�b�N��F��������ő吔
#define JOY_MAX 3

// �t���O�p
#define INPUT_UP		(1 << 0)
#define INPUT_DOWN		(1 << 1)
#define INPUT_LEFT		(1 << 2)
#define INPUT_RIGHT		(1 << 3)
#define INPUT_01		(1 << 4)
#define INPUT_02		(1 << 5)
#define INPUT_03		(1 << 6)
#define INPUT_04		(1 << 7)
#define INPUT_05		(1 << 8)
#define INPUT_06		(1 << 9)
#define INPUT_07		(1 << 10)
#define INPUT_08		(1 << 11)
#define INPUT_09		(1 << 12)
#define INPUT_10		(1 << 13)
#define INPUT_11		(1 << 14)
#define INPUT_12		(1 << 15)
#define INPUT_13		(1 << 16)
#define INPUT_14		(1 << 17)
#define INPUT_15		(1 << 18)
#define INPUT_16		(1 << 19)
#define INPUT_17		(1 << 20)
#define INPUT_18		(1 << 21)
#define INPUT_19		(1 << 22)
#define INPUT_20		(1 << 23)
#define INPUT_21		(1 << 24)
#define INPUT_22		(1 << 25)
#define INPUT_23		(1 << 26)
#define INPUT_24		(1 << 27)
#define INPUT_25		(1 << 28)
#define INPUT_26		(1 << 29)
#define INPUT_RET		(1 << 30)
#define INPUT_ESC		(1 << 31)

// �f�o�C�X�̎��
enum class DeviceType
{
	JoyStick,
	KeyBoard
};

/**
* @struct InputJoyStick
* @brief �W���C�X�e�B�b�N�̓��͍\����
*/
typedef struct InputJoyStick
{
	// ���X�e�B�b�N�̓��͂�����
	int lX, lY;
	
	// �E�X�e�B�b�N�̓��͂�����
	int rX, rY;
}InputJoyStick;

/**
* @struct InputBase
* @brief ���͂��܂Ƃ߂�
*/
struct InputBase
{
	// ���͂̏����Ǘ�����t���O
	unsigned int flag;

	// �X�e�B�b�N���͏��̃|�C���^
	InputJoyStick iJoy;

	// �͂����ꂽ���ǂ���
	bool isRepel;

	// �|����Ă���
	bool isDownY, isDownX;

	InputBase(){
		flag = NULL;
	}
	~InputBase(){

	}
};


/**
* @interface IInputDevice
* @brief ���̓f�o�C�X�̃C���^�[�t�F�C�X
*/
__interface IInputDevice
{
	/**
	* @brief ������
	*/
	HRESULT Initialize();

	/**
	* @brief ���͏����擾����
	*/
	void GetInput(InputBase* pIB);
};


/**
* @brief �L�[�{�[�h�f�o�C�X�쐬�y�ѓ��͂𓾂�
*/
class KeyBoard : public IInputDevice
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	KeyBoard(LPDIRECTINPUT8 pDI);
	/**
	* @brief �f�X�g���N�^
	*/
	~KeyBoard();

	/**
	* @brief �L�[�{�[�h��������
	*/
	HRESULT Initialize();

	/**
	* @brief �L�[�{�[�h������͂𓾂�
	* @param InputBase* pIB ���͂𓾂�\���́@�����ɓ��͂��i�[�����
	*/
	void GetInput(InputBase* pIB);

private:
	/// �f�o�C�X
	LPDIRECTINPUTDEVICE8 m_pKeyDevice;

	LPDIRECTINPUT8 m_pDI;

	/// �L�[���͂��i�[����
	unsigned char m_keys[256];

	/// ���͂��Ǘ�����
	InputBase mIB;

	/// �͂����J�E���g
	int m_repelCount;

	/// �͂�������
	bool m_repelControl;

	/// �͂����t���[��
	int m_repelFrame;

	/**
	* @brief �L�[�{�[�h�̂͂�������̍X�V
	*/
	void UpdateRepel(InputBase* pIB);
};

/**
* @brief �W���C�X�e�B�b�N�N���X
*/
class JoyStick : public IInputDevice
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	JoyStick();
	/**
	* @brief �f�X�g���N�^
	*/
	~JoyStick();

	/**
	* @brief �W���C�X�e�B�b�N��������
	*/
	HRESULT Initialize();

	/**
	* @brief ���͏����擾����
	* @param InputBase* pIB ���͂𓾂�\���́@�����ɓ��͂��i�[�����
	*/
	void GetInput(InputBase* pIB);

private:
	/// �W���C�X�e�B�b�N��
	static int m_joyCount;

	/// �W���C�X�e�B�b�N�̔ԍ�
	static int m_joyid;

	/// �ԍ�
	int m_idx;

	/// �͂����J�E���g
	int m_repelCount;

	/// �͂�������
	bool m_repelControl;

	/// �͂����t���[��
	int m_repelFrame;

	static LPDIRECTINPUT8 m_pDI;

	static LPDIRECTINPUTDEVICE8 m_pJoyDevice[JOY_MAX];

	/**
	* @brief �W���C�X�e�B�b�N�̂͂�������̍X�V
	*/
	void UpdateRepel(InputBase* pIB);

	/**
	* @brief �W���C�X�e�B�b�N�R�[���o�b�N�֐�
	*/
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);

	/**
	* @brief �W���C�X�e�B�b�N�R�[���o�b�N�֐�
	*/
	static BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);
};

#endif