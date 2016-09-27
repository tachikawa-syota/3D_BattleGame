#include "InputLib.h"
#include "..\ApplicationSystem\ApplicationData.h"

/**
* @brief �R���X�g���N�^
*/
KeyBoard::KeyBoard(LPDIRECTINPUT8 pDI)
{
	// �f�o�C�X
	m_pDI = pDI;

	m_repelCount = 0;;
	m_repelControl = false;
	m_repelFrame = 0;;
}

/**
* @brief �f�X�g���N�^
*/
KeyBoard::~KeyBoard()
{
	SafeRelease(m_pKeyDevice);
}

/**
* @brief �L�[�{�[�h�̏��������s��
* @return ����:S_OK ���s:E_FAIL
*/
HRESULT KeyBoard::Initialize()
{
	// �uDirectInput�f�o�C�X�v�I�u�W�F�N�g�̍쐬
	if (FAILED(m_pDI->CreateDevice(GUID_SysKeyboard, &m_pKeyDevice, NULL))){
		return E_FAIL;
	}
	// �f�o�C�X���L�[�{�[�h�ɐݒ�
	if (FAILED(m_pKeyDevice->SetDataFormat(&c_dfDIKeyboard))){
		return E_FAIL;
	}
	// �������x���̐ݒ�
	if (FAILED(m_pKeyDevice->SetCooperativeLevel(AppIns->GetWindowHandle(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND))){
		return E_FAIL;
	}

	// �f�o�C�X���u�擾�v����
	m_pKeyDevice->Acquire();

	// ����
	return S_OK;
}

/**
* @brief �L�[�{�[�h������͂𓾂�
* @param InputBase* pIB ���͂𓾂�\���́@�����ɓ��͂��i�[�����
*/
void KeyBoard::GetInput(InputBase* pIB)
{
	// �f�o�C�X�̎擾
	HRESULT hr = m_pKeyDevice->Acquire();
	pIB->flag = NULL;

	if ((hr == DI_OK) || (hr == S_FALSE))
	{
		m_pKeyDevice->GetDeviceState(sizeof(m_keys), &m_keys);

		if (m_keys[DIK_LEFT] & 0x80){
			pIB->flag |= INPUT_LEFT;
		}
		if (m_keys[DIK_RIGHT] & 0x80){
			pIB->flag |= INPUT_RIGHT;
		}
		if (m_keys[DIK_UP] & 0x80){
			pIB->flag |= INPUT_UP;
		}
		if (m_keys[DIK_DOWN] & 0x80){
			pIB->flag |= INPUT_DOWN;
		}

		if (m_keys[DIK_Z] & 0x80){
			pIB->flag |= INPUT_01;
		}
		if (m_keys[DIK_X] & 0x80){
			pIB->flag |= INPUT_02;
		}
		if (m_keys[DIK_C] & 0x80){
			pIB->flag |= INPUT_03;
		}
		if (m_keys[DIK_A] & 0x80){
			pIB->flag |= INPUT_04;
		}
		if (m_keys[DIK_S] & 0x80){
			pIB->flag |= INPUT_05;
		}
		if (m_keys[DIK_D] & 0x80){
			pIB->flag |= INPUT_06;
		}
		if (m_keys[DIK_Q] & 0x80){
			pIB->flag |= INPUT_07;
		}
		if (m_keys[DIK_W] & 0x80){
			pIB->flag |= INPUT_08;
		}
		if (m_keys[DIK_E] & 0x80){
			pIB->flag |= INPUT_09;
		}

		// �G���^�[�L�[
		if (m_keys[DIK_RETURN] & 0x80){
			pIB->flag |= INPUT_RET;
		}
		// �G�X�P�[�v�L�[
		if (m_keys[DIK_ESCAPE] & 0x80){
			pIB->flag |= INPUT_ESC;
		}
	}

	// �͂����X�V
	UpdateRepel(pIB);
}

/**
* @brief �L�[�{�[�h�̂͂�������̍X�V
*/
void KeyBoard::UpdateRepel(InputBase* pIB)
{
	// ���E�ړ��L�[�ɓ��͂��Ȃ����
	if (pIB->flag != INPUT_LEFT && pIB->flag != INPUT_RIGHT)
	{
		// �͂����t���O������
		pIB->isRepel = false;
		// �|��Ă��Ȃ�
		pIB->isDownX = false;
		// ����t���O������
		m_repelControl = false;
	}
	else
	{
		// �X�e�B�b�N�|�����
		pIB->isDownX = true;
		// ������J�E���g����Ȃ��悤�ɐ���
		if (!m_repelControl)
		{
			m_repelControl = true;
			m_repelCount++;
		}
	}

	// �e����Ԃ��L�����X�e�B�b�N���|����Ă�����
	// ��Ԃ�����������
	if (m_repelCount > 1 && pIB->isDownX)
	{
		m_repelFrame = 1;
		m_repelCount = 2;
		pIB->isRepel = true;
	}

	// ���~�b�g��������
	if (m_repelFrame > 10)
	{
		// ���ׂď�����
		m_repelFrame = 0;
		m_repelCount = 0;
		pIB->isRepel = false;
		if (!pIB->isDownX)
			m_repelControl = false;
	}

	// �P�\�t���[������
	if (m_repelCount > 0){
		m_repelFrame++;
	}
}


// static�ϐ�
int JoyStick::m_joyCount;
int JoyStick::m_joyid;
LPDIRECTINPUT8 JoyStick::m_pDI;
LPDIRECTINPUTDEVICE8 JoyStick::m_pJoyDevice[JOY_MAX];

/**
* @brief �f�X�g���N�^
*/
JoyStick::~JoyStick()
{
}

/**
* @brief �R���X�g���N�^
*/
JoyStick::JoyStick()
{
	m_joyCount = 0;
	m_idx = m_joyid;

	m_repelCount = 0;;
	m_repelControl = false;
	m_repelFrame = 0;;
}

/**
* @brief �W���C�X�e�B�b�N�̏�����
*/
HRESULT JoyStick::Initialize()
{
	HRESULT hr;
	// �uDirectInput�v�I�u�W�F�N�g�̍쐬
	hr = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDI, NULL);
	if (FAILED(hr)){
		return E_FAIL;
	}

	//���p�\�ȃQ�[���R���g���[���[�̗񋓊֐������s
	m_pDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);

	// �f�o�C�X���쐬����Ă��邩
	if (!m_pJoyDevice[m_joyid]){
		return E_FAIL;
	}

	// �f�o�C�X���W���C�X�e�B�b�N�ɐݒ�
	hr = m_pJoyDevice[m_joyid]->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(hr)){
		return E_FAIL;
	}

	// �������x���̐ݒ�
	hr = m_pJoyDevice[m_joyid]->SetCooperativeLevel(AppIns->GetWindowHandle(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if (FAILED(hr)){
		return E_FAIL;
	}

	//�A�v���P�[�V�����Ŏg�p����R���g���[���[�̃v���p�e�B��񋓂��Đݒ肷��
	hr = m_pJoyDevice[m_joyid]->EnumObjects(EnumObjectsCallback, &m_joyid, DIDFT_ALL);
	if (FAILED(hr)){
		return E_FAIL;
	}

	// �f�o�C�X���u�擾�v����
	m_pJoyDevice[m_joyid]->Acquire();

	// �f�o�C�X�̔ԍ��𑝂₷
	m_joyid++;

	// ����
	return S_OK;
}

/**
* @brief �W���C�X�e�B�b�N�̓��͏����擾����
* @param InputBase* pIB ���͂𓾂�\���́@�����ɓ��͂��i�[�����
*/
void JoyStick::GetInput(InputBase* pIB)
{
	DIJOYSTATE2 js;
	// �f�o�C�X���u�擾�v����
	m_pJoyDevice[m_idx]->Acquire();

	m_pJoyDevice[m_idx]->Poll();
	m_pJoyDevice[m_idx]->GetDeviceState(sizeof(DIJOYSTATE2), &js);

	// ��x����������
	pIB->flag = NULL;

	// �X�e�B�b�N����
	pIB->iJoy.lX = js.lX;
	pIB->iJoy.lY = js.lY;
	pIB->iJoy.rX = js.lRz;
	pIB->iJoy.rY = js.rglSlider[0];

	// ���͔���
	if (pIB->iJoy.lX < -10){
		pIB->flag |= INPUT_LEFT;
	}
	else if (pIB->iJoy.lX > 10){
		pIB->flag |= INPUT_RIGHT;
	}

	if (pIB->iJoy.lY < -10){
		pIB->flag |= INPUT_UP;
	}
	else if (pIB->iJoy.lY > 10){
		pIB->flag |= INPUT_DOWN;
	}

	// �{�^������
	for (int i = 0; i < 27; ++i){
		pIB->flag |= (js.rgbButtons[i] & 0x80) ? 1 << (4 + i) : 0;
	}

	// �͂����`�F�b�N
	UpdateRepel(pIB);
}

/**
* @brief �W���C�X�e�B�b�N�̂͂�������̍X�V
*/
void JoyStick::UpdateRepel(InputBase* pIB)
{
	// �X�e�B�b�N�ɓ��͂��Ȃ����
	if (pIB->iJoy.lX <= 5 && pIB->iJoy.lX >= -5)
	{
		// �͂����t���O������
		pIB->isRepel = false;
		// �|��Ă��Ȃ�
		pIB->isDownX = false;
		// ����t���O������
		m_repelControl = false;
	}
	else
	{
		// �X�e�B�b�N�|�����
		pIB->isDownX = true;
		// ������J�E���g����Ȃ��悤�ɐ���
		if (!m_repelControl)
		{
			m_repelControl = true;
			m_repelCount++;
		}
	}

	// �e����Ԃ��L�����X�e�B�b�N���|����Ă�����
	// ��Ԃ�����������
	if (m_repelCount > 1 && pIB->isDownX)
	{
		m_repelFrame = 1;
		m_repelCount = 2;
		pIB->isRepel = true;
	}

	// ���~�b�g��������
	if (m_repelFrame > 10)
	{
		// ���ׂď�����
		m_repelFrame = 0;
		m_repelCount = 0;
		pIB->isRepel = false;
		if (!pIB->isDownX)
			m_repelControl = false;
	}

	// �P�\�t���[������
	if (m_repelCount > 0){
		m_repelFrame++;
	}
}

/**
* @brief �W���C�X�e�B�b�N�R�[���o�b�N�֐�
*/
BOOL CALLBACK JoyStick::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	for (int i = m_joyCount; i < m_joyid; ++i){
		m_joyCount++;
		return DIENUM_CONTINUE;
	}

	// DirectInput�f�o�C�X�I�u�W�F�N�g�̍쐬
	if (FAILED(m_pDI->CreateDevice(pdidInstance->guidInstance, &m_pJoyDevice[m_joyid], NULL)))
	{
		// �W���C�X�e�B�b�N�̌����𑱂���
		return DIENUM_CONTINUE;
	}

	// �W���C�X�e�B�b�N�̌������I��
	return DIENUM_STOP;
}

/**
* @brief �W���C�X�e�B�b�N�R�[���o�b�N�֐�
*/
BOOL CALLBACK JoyStick::EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext)
{
	if (pdidoi->dwType & DIDFT_AXIS){
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = pdidoi->dwType;
		diprg.lMin = -100;
		diprg.lMax = 100;

		if (FAILED(m_pJoyDevice[m_joyid]->SetProperty(DIPROP_RANGE, &diprg.diph)))
		{
			return DIENUM_STOP;
		}
	}
	return DIENUM_CONTINUE;
}