/**
* @file   Computer.h
* @brief  AIの管理する
* @author Syota Tachikawa
*/

#ifndef ___COMPUTER_H
#define ___COMPUTER_H

#include "..\Character\IPlayer.h"
#include "IComputer.h"
#include "AILevelFactory.h"

// 前方宣言
class DamageData;

/**
* @brief コンピューター(人工知能)クラス
*/
class Computer
{
public:
	/**
	* @brief 初期化
	* @param レベル
	*/
	void Initialize(AILevel level);

	/**
	* @brief 思考する
	* @param player - プレイヤークラス
	* @param stage - ステージクラス
	* @param damage - ダメージデータクラス
	* @param index - プレイヤー番号
	*/
	DeviceState& Think(vector<IPlayerPtr>& player, list<IItemPtr>& item, IStagePtr& stage, DamageData damage, int index);

private:
	/// コンピューターインターフェイス
	IComputerPtr m_computer;

	/**
	* @brief 前回の行動を継続する
	*/
	DeviceState& ThinkContinue();

	/**
	* @brief 新しい行動を思考する
	* @param player - プレイヤークラス
	* @param stage - ステージクラス
	* @param damage - ダメージデータクラス
	* @param index - プレイヤー番号
	*/
	DeviceState& ThinkNewAction(vector<IPlayerPtr>& player, IStagePtr& stage, DamageData damage, int index);

	/**
	* @brief ステージに復帰する行動をする
	* @param player - プレイヤークラス
	* @param stage - ステージクラス
	* @param index - プレイヤー番号
	*/
	DeviceState& StageReturnAction(vector<IPlayerPtr>& player, IStagePtr& stage, int index);

};

#endif