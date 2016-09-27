/**
* @file   AILevel1.h
* @brief  レベル1のAIの挙動を管理する
* @author Syota Tachikawa
*/

#ifndef ___AILEVEL1_H
#define ___AILEVEL1_H

#include "IComputer.h"

/**
* @brief レベル1のAIの挙動を管理するクラス
*/
class AILevel1 : public IComputer
{
public:
	/**
	* @brief コンストラクタ
	*/
	AILevel1();

	/**
	* @brief デストラクタ
	*/
	~AILevel1();

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
	* @brief ステージに復帰する行動をする
	* @param player - プレイヤークラス
	* @param stage - ステージクラス
	* @param index - プレイヤー番号
	*/
	DeviceState& StageReturnAction(vector<IPlayerPtr>& player, IStagePtr& stage, int index);

	/**
	* @brief 前回の行動を継続する
	*/
	DeviceState& ThinkContinue();

private:
	/// 行動
	Action m_action;

	/// 状態
	DeviceState m_deviceState;

	/// 行動フレーム
	unsigned int m_actionFrame;

	/// 次の行動を考えるフレーム
	unsigned int m_nextActionFrame;

	/// 復帰フレーム
	unsigned int returnFrame;

	/**
	* @brief 行動を設定する
	* @param action - 行動
	* @param flag - 入力フラグ
	*/
	void SetAction(Action action, unsigned int flag);

	/**
	* @brief 移動を設定する
	* @param action - 行動(移動系の行動を入れる)
	* @param flag - 入力フラグ
	*/
	void SetMoveAction(Action action, unsigned int flag);
};

#endif