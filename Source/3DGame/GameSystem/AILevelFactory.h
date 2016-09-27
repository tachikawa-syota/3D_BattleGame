/**
* @file   AILevelFactory.h
* @brief  AIのレベルを生成するクラス
* @author Syota Tachikawa
*/

#ifndef ___AILEVELFACTORY_H
#define ___AILEVELFACTORY_H

#include "AILevel1.h"
#include "AILevel2.h"
#include "..\Common\Utility.h"

/**
* @enum AILevel
* @brief AIのレベル
*/
enum class AILevel : BYTE
{
	/// レベル1
	Level1,

	/// レベル2
	Level2
};

/**
* @brief AIのレベルを生成するクラス
*/
class AILevelFactory
{
public:
	/**
	* @brief キャラクターインスタンス生成
	* @brief レベル
	*/
	IComputerPtr CreateAI(AILevel level)
	{
		switch (level)
		{
			// レベル1
		case AILevel::Level1:
			return make_shared<AILevel1>();

			// レベル2
		case AILevel::Level2:
			return make_shared<AILevel2>();

			// エラー
		default:
			// 強制的にゲームを終了する
			Utility::abortWithMsg("存在しないAIレベルが選択されました。終了します");
			return nullptr;
		}
	}
};

#endif