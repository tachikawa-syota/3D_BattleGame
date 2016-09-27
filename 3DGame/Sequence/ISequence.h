/**
* @file   ISequence.h
* @brief  シークエンスインターフェイス
* @author Syota Tachikawa
*/

#ifndef ___ISEQUENCE_H
#define ___ISEQUENCE_H

#include "..\Common\CommonLib.h"

/**
* @interface ISequence
* @brief シークエンスインターフェイスクラス
*
* @note：全てのシーンクラスはこのインターフェイスを継承させる
*/
__interface ISequence
{
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
};

/// シークエンスのスマートポインタ
using ISequencePtr = shared_ptr<ISequence>;

#endif