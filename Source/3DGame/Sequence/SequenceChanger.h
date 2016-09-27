/**
* @file   SequenceChanger.h
* @brief  シーンの切り替えを管理する
* @author Syota Tachikawa
*/

#ifndef ___SEQUENCECHANGER_H
#define ___SEQUENCECHANGER_H

#include "ISequence.h"

/**
* @brief シーンの切り替えを管理するクラス
*/
class SequenceChanger
{
public:
	/**
	* @brief コンストラクタ
	*/
	static SequenceChanger* GetInstance(){
		static SequenceChanger singleton;
		return &singleton;
	}

	/**
	* @brief 初期化
	*/
	void Initialize();

	/**
	* @brief シーンの切り替え
	* @param 切り替えたいシーンを入れる
	*/
	void ChangeScene(const ISequencePtr& next);
	
	/**
	* @brief 現在のシーンを取得する
	*/
	const ISequencePtr& GetNowScene() const;

private:
	/// 現在のシーン
	ISequencePtr m_nowScene;
};

#endif