/**
* @file   InputMask.h
* @brief  デバイスの入力情報を管理する
* @author Syota Tachikawa
*/

#ifndef ___INPUTMASK_H
#define ___INPUTMASK_H

#include "DeviceManager.h"

/**
* @class InputMask
* @brief 入力を制御するクラス
*/
class InputMask
{
public:
	//コンストラクタ
	InputMask(){}
	/**
	* @param 入力デバイスのID
	*/
	InputMask(int deviceID);
	/**
	* @param デストラクタ
	*/
	~InputMask();

	/**
	* @brief 初期化を行う
	*/
	void Initialize(int deviceID);

	/**
	* @brief 入力を受け取る
	* @return true:成功　false:失敗
	*/
	bool GetInput();

	/**
	* @brief 引数に渡された入力があるかどうかを判定する
	* @param 入力データ (INPUT_01 | INPUT_02)みたいな感じで渡すこと
	* @return true:入力あり false:入力なし
	*/
	bool CheckInput(unsigned int FLAG);

	/**
	* @brief 引数に渡された入力があるかどうかを判定する(すべてをマスクする。連続で入力されているものは入力としない)
	* @param 入力データ (INPUT_01 | INPUT_02)みたいな感じで渡すこと
	* @return true:入力あり false:入力なし
	*/
	bool CheckInputMask(unsigned int FLAG);

	/**
	* @brief 入力データをそのまま得る
	*/
	unsigned int GetInputState();

	/**
	* @brief ジョイパッド左スティックＸ座標取得する
	*/
	int GetJoyStickLPosX();

	/**
	* @brief ジョイパッド左スティックＹ座標取得する
	*/
	int GetJoyStickLPosY();

	/**
	* @brief スティックが弾かれたかどうか
	*/
	bool GetJoyStickRepel();

	/**
	* @brief 入力デバイスの種類を取得する
	* @return 入力デバイスの種類(キーボード or ジョイスティック)
	*/
	DeviceType GetDeviceType();

	/**
	* @brief パッドデータをクリアする
	*/
	void DataClear();

	/**
	* @brief はじきデータをクリアする
	*/
	void RepelClear();

protected:
	/// 入力デバイスを設定する
	DeviceManager* m_idmInstace;

	/// 一つ前の入力を保存する
	InputBase m_PrevInput;

	/// 現在の入力を保存する
	InputBase m_Input;

	/// 入力デバイスのID
	int	m_InputDeviceID;
};

/// 入力デバイスのスマートポインタ
using InputPtr = shared_ptr <InputMask>;

#endif
