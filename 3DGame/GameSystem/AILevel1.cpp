#include "AILevel1.h"
#include "DamageData.h"
#include "..\Common\Utility.h"
#include "..\DirectX\InputLib.h"

/// 次の行動を思考するまでのフレーム
const int intervalFrame = 60;
/// 復帰技を出すまでのフレーム
const int intervalReturnFrame = 60;

/**
* @brief コンストラクタ
*/
AILevel1::AILevel1()
{
	// 行動をアイドリングに
	SetAction(Action::Idle, NULL);

	m_actionFrame = 0;
	returnFrame = 0;
	m_nextActionFrame = 0;
}

/**
* @brief デストラクタ
*/
AILevel1::~AILevel1()
{

}

/**
* @brief 思考する
* @param player - プレイヤークラス
* @param stage - ステージクラス
* @param damage - ダメージデータクラス
* @param index - プレイヤー番号
*/
DeviceState& AILevel1::Think(vector<IPlayerPtr>& player, list<IItemPtr>& item, IStagePtr& stage, DamageData damage, int index)
{
	// 前回のデバイス情報を記録しておく
	m_deviceState.prevFlag = m_deviceState.flag;

	// 地面に付いていない 又は 場外だったら復帰行動を取る
	if (player[index]->GetPos().y < -0.5f ||
		player[index]->GetPos().x >= 10.0f || player[index]->GetPos().x <= -10.0f){
		return StageReturnAction(player, stage, index);
	}
	else{
		returnFrame = 0;
	}

	// アイテムの位置を探す
	for (auto it = item.begin(); it != item.end(); it++)
	{
		// 座標取得
		Vector3 pos = (*it)->GetPos();

		// 差分
		Vector3 diff = Utility::DiffVector(player[index]->GetPos(), pos);

		// 距離
		float length = diff.Length();

		// 距離が一定以内・50%の確率で回避行動を取る
		if (length <= 2.0f && Utility::Random(0,1) == 0){
			
			int rand = Utility::Random(0, 2);
			// ジャンプさせる
			if (rand == 0) SetMoveAction(Action::JumpStart, INPUT_UP);
			// スマッシュ攻撃をする
			if (rand == 1) SetAction(Action::SmashAttack, INPUT_02);
			// 弱攻撃をする
			if (rand == 2) SetAction(Action::NormalAttack, INPUT_01);

			return m_deviceState;
		}
	}

	// 行動フレームを加算する
	m_actionFrame++;

	// 一定フレームを超えたら
	if (m_actionFrame > m_nextActionFrame){
		m_nextActionFrame = intervalFrame + m_actionFrame;

		// 新しい行動を思考する
		return ThinkNewAction(player, stage, damage, index);
	}

	// 前回の行動を継続する
	return ThinkContinue();
}

/**
* @brief 新しい行動を思考する
* @param player - プレイヤークラス
* @param stage - ステージクラス
* @param damage - ダメージデータクラス
* @param index - プレイヤー番号
*/
DeviceState& AILevel1::ThinkNewAction(vector<IPlayerPtr>& player, IStagePtr& stage, DamageData damage, int index)
{
	// 初期化
	IPlayerPtr target = nullptr;

	// 一番近いプレイヤーを取得する
	target = Utility::GetNearPlayer(player, index);

	for (int i = 0; i != player.size(); i++)
	{
		// 場外にいる復帰中のプレイヤーは狙わない
		if (player[i]->GetPos().x >= 10.0f || player[i]->GetPos().x <= -10.0f) {
			continue;
		}
	}

	// ターゲットの取得に失敗したら待機状態にする
	if (target == nullptr){
		SetMoveAction(Action::Idle, NULL);
		return m_deviceState;
	}

	// 近い場合攻撃をする
	Vector3 diff = Utility::DiffVector(player[index]->GetPos(), target->GetPos());
	float length = diff.Length();

	// 距離を測る
	if (length < 2.5f)
	{
		int rand = Utility::Random(0, 4);
		// 通常攻撃を出す
		if (rand < 3){
			SetAction(Action::NormalAttack, INPUT_01);
		}
		// スマッシュ攻撃を出す
		else if (rand == 4){
			SetAction(Action::SmashAttack, INPUT_02);
		}

		return m_deviceState;
	}

	// 距離を測る
	if (length < 3.0f){
		// 攻撃
		SetAction(Action::SmashAttack, INPUT_02);
		return m_deviceState;
	}

	// 再度取得
	if (diff.x == 0){
		Vector3 diff = Utility::DiffVector(player[index]->GetPos(), target->GetPos());
	}

	// 右移動
	if (diff.x < 0.0f && m_deviceState.axis != INPUT_RIGHT){
		SetMoveAction(Action::RightWalk, INPUT_RIGHT);
	}
	// 左移動
	else if (diff.x > 0.0f && m_deviceState.axis != INPUT_LEFT){
		SetMoveAction(Action::LeftWalk, INPUT_LEFT);
	}

	// たまにジャンプしながら近づいてくる
	if (Utility::Random(0, 5) >= 4 && m_action != Action::JumpNow)
	{
		if (length > 5.0f){
			SetMoveAction(Action::JumpStart, INPUT_UP);
		}
	}

	// 結果
	return m_deviceState;
}

/**
* @brief ステージに復帰する行動をする
* @param player - プレイヤークラス
* @param stage - ステージクラス
* @param index - プレイヤー番号
*/
DeviceState& AILevel1::StageReturnAction(vector<IPlayerPtr>& player, IStagePtr& stage, int index)
{
	// ステージ内の床下にいたら即復帰技を出す
	if (player[index]->GetPos().y < -0.5f)
	{
		if (player[index]->GetPos().x <= 10.0f && player[index]->GetPos().x >= -10.0f){
			SetAction(Action::SpecialAttackUp, INPUT_03);
			return m_deviceState;
		}
	}

	// 差分取得
	Vector3 diff = Utility::DiffVector(player[index]->GetPos(), stage->GetPos());

	// 右移動
	if (diff.x < 0.0f && m_deviceState.axis != INPUT_RIGHT){
		SetMoveAction(Action::RightWalk, INPUT_RIGHT);
	}
	// 左移動
	else if (diff.x > 0.0f && m_deviceState.axis != INPUT_LEFT){
		SetMoveAction(Action::LeftWalk, INPUT_LEFT);
	}

	// ジャンプさせる(ステージの方向に移動させてからジャンプを行なうために最初のフレームは入らない)
	if (returnFrame > 3){
		SetMoveAction(Action::JumpStart, INPUT_UP);
	}

	// 一定フレーム経過で復帰技を使う
	if (returnFrame > intervalReturnFrame){
		returnFrame = 0;
		SetAction(Action::SpecialAttackUp, INPUT_03);
	}

	// フレーム加算(動けない被弾中は加算しない)
	if (player[index]->IsControl()){
		returnFrame++;
	}

	return m_deviceState;
}

/**
* @brief 前回の行動を継続する
*/
DeviceState& AILevel1::ThinkContinue()
{
	// 入力を初期化しておく
	m_deviceState.axis = NULL;
	m_deviceState.flag = NULL;

	switch (m_action)
	{
		// アイドリング
	case Action::Idle:
		m_deviceState.axis = NULL;
		break;

		// 右移動
	case Action::RightWalk:
		m_deviceState.axis = INPUT_RIGHT;
		break;

		// 左移動
	case Action::LeftWalk:
		m_deviceState.axis = INPUT_LEFT;
		break;

		// ジャンプ
	case Action::JumpStart:
		m_deviceState.axis = INPUT_UP;
		break;

		// 通常攻撃
	case Action::NormalAttack:
		m_deviceState.flag = INPUT_01;
		break;

		// スマッシュ攻撃
	case Action::SmashAttack:
		m_deviceState.flag = INPUT_02;
		break;

		// 必殺技(上)
	case Action::SpecialAttackUp:
		m_deviceState.flag = INPUT_03;
		break;

		// アピール
	case Action::Appeal:
		m_deviceState.flag = INPUT_04;
		break;
	}

	// 前回と入力が同じなら現在の入力NULLに設定し直す
	// (連続で入力され続けていると判定しないから)
	if (m_deviceState.flag == m_deviceState.prevFlag){
		m_deviceState.flag = NULL;
	}

	// 結果
	return m_deviceState;
}

/**
* @brief 行動を設定する
* @param action - 行動
* @param flag - 入力フラグ
*/
void AILevel1::SetAction(Action action, unsigned int flag)
{
	m_action = action;
	m_deviceState.flag = flag;
}

/**
* @brief 移動を設定する
* @param action - 行動(移動系の行動を入れる)
* @param flag - 入力フラグ
*/
void AILevel1::SetMoveAction(Action action, unsigned int flag)
{
	m_action = action;
	m_deviceState.axis = flag;
}