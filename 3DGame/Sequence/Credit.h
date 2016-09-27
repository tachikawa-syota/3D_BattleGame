/**
* @file   Credit.h
* @brief  クレジット画面の管理する
* @author Syota Tachikawa
*/

#ifndef ___CREDIT_H
#define ___CREDIT_H

#include "ISequence.h"
#include "..\DirectX\InputMask.h"

/**
* @brief クレジット画面クラス
*/
class Credit : public ISequence
{
public:
	/**
	* @brief 作成
	*/
	void Create();

	/**
	* @brief 初期化
	*/
	void Initialize();

	/**
	* @brief 更新
	*/
	void Update();

	/**
	* @brief 描画
	*/
	void Render();

	/**
	* @brief 終了
	*/
	void Finalize();

private:
	/// 入力デバイス
	InputPtr m_dev;
};

#endif