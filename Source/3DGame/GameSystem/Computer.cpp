#include "Computer.h"
#include "..\Common\Utility.h"
#include "..\DirectX\InputLib.h"
#include "..\GameSystem\DamageData.h"

/**
* @brief 初期化
* @param レベル
*/
void Computer::Initialize(AILevel level)
{
	// 指定されたレベルのAIを作成する
	AILevelFactory factory;
	m_computer = factory.CreateAI(level);
}

/**
* @brief 思考する
* @param player - プレイヤークラス
* @param stage - ステージクラス
* @param damage - ダメージデータクラス
* @param index - プレイヤー番号
*/
DeviceState& Computer::Think(vector<IPlayerPtr>& player, list<IItemPtr>& item, IStagePtr& stage, DamageData damage, int index)
{
	return m_computer->Think(player, item, stage, damage, index);
}

/**
* @brief 前回の行動を継続する
*/
DeviceState& Computer::ThinkContinue()
{
	return m_computer->ThinkContinue();
}

/**
* @brief 新しい行動を思考する
* @param player - プレイヤークラス
* @param stage - ステージクラス
* @param damage - ダメージデータクラス
* @param index - プレイヤー番号
*/
DeviceState& Computer::ThinkNewAction(vector<IPlayerPtr>& player, IStagePtr& stage, DamageData damage, int index)
{
	return m_computer->ThinkNewAction(player, stage, damage, index);
}

/**
* @brief ステージに復帰する行動をする
* @param player - プレイヤークラス
* @param stage - ステージクラス
* @param index - プレイヤー番号
*/
DeviceState& Computer::StageReturnAction(vector<IPlayerPtr>& player, IStagePtr& stage, int index)
{
	return m_computer->StageReturnAction(player, stage, index);
}