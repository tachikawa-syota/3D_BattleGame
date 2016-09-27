/**
* @file   InputLib.h
* @brief  入力デバイスの管理をする
* @author Syota Tachikawa
*/

#ifndef ___INPUTLIB_H
#define ___INPUTLIB_H

#include <dInput.h>
#include "..\Common\CommonLib.h"

// ジョイスティックを認識させる最大数
#define JOY_MAX 3

// フラグ用
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

// デバイスの種類
enum class DeviceType
{
	JoyStick,
	KeyBoard
};

/**
* @struct InputJoyStick
* @brief ジョイスティックの入力構造体
*/
typedef struct InputJoyStick
{
	// 左スティックの入力が入る
	int lX, lY;
	
	// 右スティックの入力が入る
	int rX, rY;
}InputJoyStick;

/**
* @struct InputBase
* @brief 入力をまとめる
*/
struct InputBase
{
	// 入力の情報を管理するフラグ
	unsigned int flag;

	// スティック入力情報のポインタ
	InputJoyStick iJoy;

	// はじかれたかどうか
	bool isRepel;

	// 倒されている
	bool isDownY, isDownX;

	InputBase(){
		flag = NULL;
	}
	~InputBase(){

	}
};


/**
* @interface IInputDevice
* @brief 入力デバイスのインターフェイス
*/
__interface IInputDevice
{
	/**
	* @brief 初期化
	*/
	HRESULT Initialize();

	/**
	* @brief 入力情報を取得する
	*/
	void GetInput(InputBase* pIB);
};


/**
* @brief キーボードデバイス作成及び入力を得る
*/
class KeyBoard : public IInputDevice
{
public:
	/**
	* @brief コンストラクタ
	*/
	KeyBoard(LPDIRECTINPUT8 pDI);
	/**
	* @brief デストラクタ
	*/
	~KeyBoard();

	/**
	* @brief キーボードを初期化
	*/
	HRESULT Initialize();

	/**
	* @brief キーボードから入力を得る
	* @param InputBase* pIB 入力を得る構造体　ここに入力が格納される
	*/
	void GetInput(InputBase* pIB);

private:
	/// デバイス
	LPDIRECTINPUTDEVICE8 m_pKeyDevice;

	LPDIRECTINPUT8 m_pDI;

	/// キー入力を格納する
	unsigned char m_keys[256];

	/// 入力を管理する
	InputBase mIB;

	/// はじきカウント
	int m_repelCount;

	/// はじき制御
	bool m_repelControl;

	/// はじきフレーム
	int m_repelFrame;

	/**
	* @brief キーボードのはじき判定の更新
	*/
	void UpdateRepel(InputBase* pIB);
};

/**
* @brief ジョイスティッククラス
*/
class JoyStick : public IInputDevice
{
public:
	/**
	* @brief コンストラクタ
	*/
	JoyStick();
	/**
	* @brief デストラクタ
	*/
	~JoyStick();

	/**
	* @brief ジョイスティックを初期化
	*/
	HRESULT Initialize();

	/**
	* @brief 入力情報を取得する
	* @param InputBase* pIB 入力を得る構造体　ここに入力が格納される
	*/
	void GetInput(InputBase* pIB);

private:
	/// ジョイスティック数
	static int m_joyCount;

	/// ジョイスティックの番号
	static int m_joyid;

	/// 番号
	int m_idx;

	/// はじきカウント
	int m_repelCount;

	/// はじき制御
	bool m_repelControl;

	/// はじきフレーム
	int m_repelFrame;

	static LPDIRECTINPUT8 m_pDI;

	static LPDIRECTINPUTDEVICE8 m_pJoyDevice[JOY_MAX];

	/**
	* @brief ジョイスティックのはじき判定の更新
	*/
	void UpdateRepel(InputBase* pIB);

	/**
	* @brief ジョイスティックコールバック関数
	*/
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);

	/**
	* @brief ジョイスティックコールバック関数
	*/
	static BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);
};

#endif