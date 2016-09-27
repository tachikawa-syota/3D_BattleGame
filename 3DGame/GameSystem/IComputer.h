/**
* @file   IComputer.h
* @brief  AIの管理する
* @author Syota Tachikawa
*/

#ifndef ___ICOMPUTER_H
#define ___ICOMPUTER_H

#include "..\Common\STDXStruct.h"
#include "..\Character\IPlayer.h"

// 前方宣言
class DamageData;

__interface IComputer
{
	/**
	* @brief 思考する
	* @param player - プレイヤークラス
	* @param stage - ステージクラス
	* @param damage - ダメージデータクラス
	* @param index - プレイヤー番号
	*/
	DeviceState& Think(vector<IPlayerPtr>& player, list<IItemPtr>& item, IStagePtr& stage, DamageData damage, int index);

	/**
	* @brief 新しい行動を思考する
	* @param player - プレイヤークラス
	* @param stage - ステージクラス
	* @param damage - ダメージデータクラス
	* @param index - プレイヤー番号
	*/
	DeviceState& ThinkNewAction(vector<IPlayerPtr>& player, IStagePtr& stage, DamageData damage, int index);

	/**
	* @brief 前回の行動を継続する
	*/
	DeviceState& ThinkContinue();

	/**
	* @brief ステージに復帰する行動をする
	* @param player - プレイヤークラス
	* @param stage - ステージクラス
	* @param index - プレイヤー番号
	*/
	DeviceState& StageReturnAction(vector<IPlayerPtr>& player, IStagePtr& stage, int index);
};

/// スマートポインタ
using IComputerPtr = shared_ptr<IComputer>;

#endif