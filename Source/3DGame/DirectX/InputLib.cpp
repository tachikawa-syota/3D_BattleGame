#include "InputLib.h"
#include "..\ApplicationSystem\ApplicationData.h"

/**
* @brief コンストラクタ
*/
KeyBoard::KeyBoard(LPDIRECTINPUT8 pDI)
{
	// デバイス
	m_pDI = pDI;

	m_repelCount = 0;;
	m_repelControl = false;
	m_repelFrame = 0;;
}

/**
* @brief デストラクタ
*/
KeyBoard::~KeyBoard()
{
	SafeRelease(m_pKeyDevice);
}

/**
* @brief キーボードの初期化を行う
* @return 成功:S_OK 失敗:E_FAIL
*/
HRESULT KeyBoard::Initialize()
{
	// 「DirectInputデバイス」オブジェクトの作成
	if (FAILED(m_pDI->CreateDevice(GUID_SysKeyboard, &m_pKeyDevice, NULL))){
		return E_FAIL;
	}
	// デバイスをキーボードに設定
	if (FAILED(m_pKeyDevice->SetDataFormat(&c_dfDIKeyboard))){
		return E_FAIL;
	}
	// 協調レベルの設定
	if (FAILED(m_pKeyDevice->SetCooperativeLevel(AppIns->GetWindowHandle(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND))){
		return E_FAIL;
	}

	// デバイスを「取得」する
	m_pKeyDevice->Acquire();

	// 成功
	return S_OK;
}

/**
* @brief キーボードから入力を得る
* @param InputBase* pIB 入力を得る構造体　ここに入力が格納される
*/
void KeyBoard::GetInput(InputBase* pIB)
{
	// デバイスの取得
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

		// エンターキー
		if (m_keys[DIK_RETURN] & 0x80){
			pIB->flag |= INPUT_RET;
		}
		// エスケープキー
		if (m_keys[DIK_ESCAPE] & 0x80){
			pIB->flag |= INPUT_ESC;
		}
	}

	// はじき更新
	UpdateRepel(pIB);
}

/**
* @brief キーボードのはじき判定の更新
*/
void KeyBoard::UpdateRepel(InputBase* pIB)
{
	// 左右移動キーに入力がない状態
	if (pIB->flag != INPUT_LEFT && pIB->flag != INPUT_RIGHT)
	{
		// はじきフラグを消す
		pIB->isRepel = false;
		// 倒れていない
		pIB->isDownX = false;
		// 制御フラグを消す
		m_repelControl = false;
	}
	else
	{
		// スティック倒し状態
		pIB->isDownX = true;
		// 何回もカウントされないように制御
		if (!m_repelControl)
		{
			m_repelControl = true;
			m_repelCount++;
		}
	}

	// 弾き状態が有効かつスティックが倒されていたら
	// 状態を持続させる
	if (m_repelCount > 1 && pIB->isDownX)
	{
		m_repelFrame = 1;
		m_repelCount = 2;
		pIB->isRepel = true;
	}

	// リミットがきたら
	if (m_repelFrame > 10)
	{
		// すべて初期化
		m_repelFrame = 0;
		m_repelCount = 0;
		pIB->isRepel = false;
		if (!pIB->isDownX)
			m_repelControl = false;
	}

	// 猶予フレームを回す
	if (m_repelCount > 0){
		m_repelFrame++;
	}
}


// static変数
int JoyStick::m_joyCount;
int JoyStick::m_joyid;
LPDIRECTINPUT8 JoyStick::m_pDI;
LPDIRECTINPUTDEVICE8 JoyStick::m_pJoyDevice[JOY_MAX];

/**
* @brief デストラクタ
*/
JoyStick::~JoyStick()
{
}

/**
* @brief コンストラクタ
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
* @brief ジョイスティックの初期化
*/
HRESULT JoyStick::Initialize()
{
	HRESULT hr;
	// 「DirectInput」オブジェクトの作成
	hr = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDI, NULL);
	if (FAILED(hr)){
		return E_FAIL;
	}

	//利用可能なゲームコントローラーの列挙関数を実行
	m_pDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);

	// デバイスが作成されているか
	if (!m_pJoyDevice[m_joyid]){
		return E_FAIL;
	}

	// デバイスをジョイスティックに設定
	hr = m_pJoyDevice[m_joyid]->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(hr)){
		return E_FAIL;
	}

	// 協調レベルの設定
	hr = m_pJoyDevice[m_joyid]->SetCooperativeLevel(AppIns->GetWindowHandle(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if (FAILED(hr)){
		return E_FAIL;
	}

	//アプリケーションで使用するコントローラーのプロパティを列挙して設定する
	hr = m_pJoyDevice[m_joyid]->EnumObjects(EnumObjectsCallback, &m_joyid, DIDFT_ALL);
	if (FAILED(hr)){
		return E_FAIL;
	}

	// デバイスを「取得」する
	m_pJoyDevice[m_joyid]->Acquire();

	// デバイスの番号を増やす
	m_joyid++;

	// 成功
	return S_OK;
}

/**
* @brief ジョイスティックの入力情報を取得する
* @param InputBase* pIB 入力を得る構造体　ここに入力が格納される
*/
void JoyStick::GetInput(InputBase* pIB)
{
	DIJOYSTATE2 js;
	// デバイスを「取得」する
	m_pJoyDevice[m_idx]->Acquire();

	m_pJoyDevice[m_idx]->Poll();
	m_pJoyDevice[m_idx]->GetDeviceState(sizeof(DIJOYSTATE2), &js);

	// 一度初期化する
	pIB->flag = NULL;

	// スティック入力
	pIB->iJoy.lX = js.lX;
	pIB->iJoy.lY = js.lY;
	pIB->iJoy.rX = js.lRz;
	pIB->iJoy.rY = js.rglSlider[0];

	// 入力判定
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

	// ボタン入力
	for (int i = 0; i < 27; ++i){
		pIB->flag |= (js.rgbButtons[i] & 0x80) ? 1 << (4 + i) : 0;
	}

	// はじきチェック
	UpdateRepel(pIB);
}

/**
* @brief ジョイスティックのはじき判定の更新
*/
void JoyStick::UpdateRepel(InputBase* pIB)
{
	// スティックに入力がない状態
	if (pIB->iJoy.lX <= 5 && pIB->iJoy.lX >= -5)
	{
		// はじきフラグを消す
		pIB->isRepel = false;
		// 倒れていない
		pIB->isDownX = false;
		// 制御フラグを消す
		m_repelControl = false;
	}
	else
	{
		// スティック倒し状態
		pIB->isDownX = true;
		// 何回もカウントされないように制御
		if (!m_repelControl)
		{
			m_repelControl = true;
			m_repelCount++;
		}
	}

	// 弾き状態が有効かつスティックが倒されていたら
	// 状態を持続させる
	if (m_repelCount > 1 && pIB->isDownX)
	{
		m_repelFrame = 1;
		m_repelCount = 2;
		pIB->isRepel = true;
	}

	// リミットがきたら
	if (m_repelFrame > 10)
	{
		// すべて初期化
		m_repelFrame = 0;
		m_repelCount = 0;
		pIB->isRepel = false;
		if (!pIB->isDownX)
			m_repelControl = false;
	}

	// 猶予フレームを回す
	if (m_repelCount > 0){
		m_repelFrame++;
	}
}

/**
* @brief ジョイスティックコールバック関数
*/
BOOL CALLBACK JoyStick::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	for (int i = m_joyCount; i < m_joyid; ++i){
		m_joyCount++;
		return DIENUM_CONTINUE;
	}

	// DirectInputデバイスオブジェクトの作成
	if (FAILED(m_pDI->CreateDevice(pdidInstance->guidInstance, &m_pJoyDevice[m_joyid], NULL)))
	{
		// ジョイスティックの検索を続ける
		return DIENUM_CONTINUE;
	}

	// ジョイスティックの検索を終了
	return DIENUM_STOP;
}

/**
* @brief ジョイスティックコールバック関数
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