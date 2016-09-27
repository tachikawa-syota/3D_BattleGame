/**
* @file   StageFactory.h
* @brief  ステージのインスタンスを生成する
* @author Syota Tachikawa
*/

#ifndef ___STAGEFACTORY_H
#define ___STAGEFACTORY_H

#include "Stage1.h"

/**
* @enum StageType
* @brief ステージの種類
*/
enum class StageType : BYTE
{
	Stage1
};

/**
* @brief ステージのインスタンスを作成 (ファクトリメソッド)
*/
class StageFactory
{
public:
	/**
	* @brief ステージの作成
	* @param ステージの種類
	*/
	IStagePtr CreateStage(StageType type);
};

/**
* @brief ステージのインスタンス作成(ファクトリメソッド)
* @param ステージの種類
*/
IStagePtr StageFactory::CreateStage(StageType type)
{
	// 今はステージが一つしかない
	switch (type)
	{
		// ステージ１
	case StageType::Stage1:
		return make_unique<Stage1>();
		break;

		// あり得ない
	default:
		return nullptr;
		break;
	}
}

#endif