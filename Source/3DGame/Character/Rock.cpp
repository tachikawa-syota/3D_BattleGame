#include "Rock.h"
#include "Motion.h"
#include "CharacterParam.h"
#include "..\Common\CommonParam.h"
#include "..\Common\StringDefine.h"
#include "..\Common\UniqueData.h"
#include "..\Common\Utility.h"
#include "..\DirectX\Direct3DData.h"
#include "..\DirectX\SoundPlayer.h"
#include "..\DirectX\InputLib.h"

/**
* @brief コンストラクタ
*/
Rock::Rock(unsigned int index)
{
	// 番号のセット
	m_id = index;

	// プレイヤー番号に応じて座標と向きを設定する
	switch (index)
	{
		// 1P
	case 0:
		m_pos = Vector3(GetComPrm->InitPlayer1PosX, GetComPrm->InitPosY, GetComPrm->InitPosZ);
		m_angle = RAD(270);
		break;
		// 2P
	case 1:
		m_pos = Vector3(GetComPrm->InitPlayer2PosX, GetComPrm->InitPosY, GetComPrm->InitPosZ);
		m_angle = RAD(90);
		break;
		// 3P
	case 2:
		m_pos = Vector3(GetComPrm->InitPlayer3PosX, GetComPrm->InitPosY, GetComPrm->InitPosZ);
		m_angle = RAD(270);
		break;
		// 4P
	case 3:
		m_pos = Vector3(GetComPrm->InitPlayer4PosX, GetComPrm->InitPosY, GetComPrm->InitPosZ);
		m_angle = RAD(90);
		break;
		// エラー(何かしら値を入れておく)
	default:
		m_pos = Vector3(GetComPrm->InitPlayer2PosX, GetComPrm->InitPosY, GetComPrm->InitPosZ);
		m_angle = RAD(90);
		break;
	}

	// 座標を記録
	m_oldPos = m_pos;
	
	// プレイヤーの生成
	m_obj = make_shared<SkinMesh>();

	// スキンメッシュ読み込み
	m_obj->LoadAnimeXMesh("Data/Model/Player/", "player_1.X");
}

/**
* @brief デストラクタ
*/
Rock::~Rock()
{
	m_isAttack.clear();
}

/**
* @brief 初期化
*/
void Rock::Initialize()
{
	// 座標の初期化
	m_obj->SetPos(m_pos);

	// 速度の初期化
	m_spd = Vector3();
	m_oldSpd = Vector3();

	// 大きさの初期化
	m_scale = GetChrPrm->RockScale;
	m_obj->SetScale(m_scale);

	// 状態の初期化
	m_action = Action::Idle;
	m_nMotion = MIdle;
	m_oldAction = m_action;

	// モーションの初期化
	SetMotion(0, TRUE, TRUE);

	// フラグの設定
	m_isInvincible = false;
	// プレイヤーの数だけ攻撃判定フラグを初期化する
	for (int i = 0; i < UniqueData::GetInstance()->GetBattleNum(); i++){
		m_isAttack.push_back(false);
	}
	m_isGround = true;
	m_isControl = true;
	m_isRetire = false;
	m_isBlowRigidity = false;

	// ジャンプ回数を初期化
	m_jumpCount = 0;

	// 攻撃力
	m_power = 0;

	m_enemyID = -1;

	m_subWork = 0;
}

/**
* @brief 更新
*/
bool Rock::Update()
{
	// 地面判定
	if (m_isGround)
	{
		// ジャンプカウントのリセット
		if (m_action != Action::JumpStart){
			m_jumpCount = 0;
		}

		//被弾中以外なら
		if (m_action != Action::Damage){
			m_enemyID = -1;
		}
		// 地面に埋まるのを抑制する
		if (m_action != Action::JumpStart){
			m_spd.y = 0.0f;
		}

		m_gravity = 0.0f;
	}
	// 空中なら重量をかける
	else{
		m_gravity = GetComPrm->Gravity;
	}

	// 吹き飛び硬直
	if (m_isBlowRigidity){
		// 徐々に減速
		m_spd.x *= 0.97f;
	}

	// 重力を加算する
	m_spd.y += m_gravity;

	// 一つ前の行動と違う行動をしている且つ行動可能状態
	if (m_action != m_oldAction && m_isControl)
	{
		if (m_action != Action::JumpStart){
			ParamReset();
		}
	}

	// 行動更新
	UpdateAction();

	// 座標に速度を加算
	m_pos += m_spd;
	// 座標更新
	m_obj->SetPos(m_pos);
	// 角度更新
	m_obj->SetAngle(m_angle);
	// プレイヤー更新
	m_obj->Update();

	// 座標を記録する
	m_oldPos = m_pos;
	// 速度を記録する
	m_oldSpd = m_spd;
	// 状態を記録する
	m_oldAction = m_action;

	return true;
}

/**
* @brief 描画(シェーダー)
*/
void Rock::Render(const Matrix& view, const Matrix& proj)
{
	// ZバッファON
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	// リタイア中のプレイヤーは描画しない
	if (!m_isRetire){
		m_obj->RenderAnimeXMesh(view, proj);
	}
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
}

/**
* @brief スキンメッシュデータを取得する
*/
const SkinMeshPtr& Rock::GetSkinMesh() const
{
	return m_obj;
}

/**
* @brief アイドリング
*/
void Rock::Idling()
{
	// フレーム更新
	m_obj->UpdateFrame();

	// モーションの設定とフレームの更新
	if (m_nMotion != MIdle){
		SetMotion(MIdle, true, true);
	}
	// アイドル状態
	m_action = Action::Idle;
	// 速度初期化
	m_spd = Vector3(0.0f, m_spd.y, 0.0f);
}

/**
* @brief アイドリング更新
*/
void Rock::UpdateIdling()
{
	m_obj->UpdateFrame();
}

/**
* @brief 右移動
*/
void Rock::MoveRight()
{
	if (m_action != Action::JumpNow && m_action != Action::JumpStart)
	{
		//ダッシュモーションループ
		if (m_nMotion != MRun) SetMotion(MRun, true, true);

		// 歩き
		m_action = Action::RightWalk;
	}

	// 左向き
	m_dir = Direction::Right;

	// 更新
	UpdateMoveRight();
}

/**
* @brief 左移動
*/
void Rock::MoveLeft()
{
	if (m_action != Action::JumpNow && m_action != Action::JumpStart)
	{
		//ダッシュモーションループ
		if (m_nMotion != MRun) SetMotion(MRun, true, true);

		// 歩き
		m_action = Action::LeftWalk;
	}

	// 左向き
	m_dir = Direction::Left;

	// 更新
	UpdateMoveLeft();
}

/**
* @brief 右移動更新
*/
void Rock::UpdateMoveRight()
{
	// 吹き飛び硬直フラグを消す
	m_isBlowRigidity = false;

	// フレームを加算する
	m_obj->UpdateFrame();

	// 速度セット
	m_spd.x = -0.1f;

	// 角度セット
	m_angle = RAD(90);
}

/**
* @brief 左移動更新
*/
void Rock::UpdateMoveLeft()
{
	// 吹き飛び硬直フラグを消す
	m_isBlowRigidity = false;

	// フレームを加算する
	m_obj->UpdateFrame();

	// 速度セット
	m_spd.x = 0.1f;

	// 角度セット
	m_angle = RAD(270);

}

/**
* @brief ジャンプ
*/
void Rock::Jump()
{
	// 最大数に達していたら何もしない
	if (m_jumpCount >= GetComPrm->JumpMax) return;

	// 瞬間的な連続ジャンプを抑制する
	if (m_jumpCount > 0){
		if (m_nMotion != MJump2 && m_nMotion != MJump3) return;
	}

	m_subWork = 0;

	// アニメーション切り替えフレーム
	m_obj->SetAnimShiftTime(0.0f);

	// ジャンプ状態
	m_action = Action::JumpStart;

	// ジャンプカウントを増やす
	m_jumpCount++;

	// 速度を設定する
	m_spd = Vector3(m_spd.x, 0.0f, m_spd.z);
	m_spd.y += 0.2f;

	// モーションの設定
	SetMotion(MJump1, false, true);
}

/**
* @brief ジャンプ更新
*/
void Rock::UpdateJump()
{
	switch (m_subWork)
	{
	case 0:
		if (m_obj->GetTime() > ROCK_MOTION_JUMP1){
			if (m_nMotion == MJump1){
				SetMotion(MJump2, false, true);
				m_subWork++;
			}
		}
		else{
			m_obj->UpdateFrame();
		}
		break;

	case 1:
		if (m_obj->GetTime() > ROCK_MOTION_JUMP2){
			if (m_nMotion == MJump2){
				SetMotion(MJump3, false, true);
				m_subWork++;
			}
		}
		else{
			m_obj->UpdateFrame();
		}
		break;

	case 2:
		if (m_obj->GetTime() > ROCK_MOTION_JUMP3){
		}
		else{
			m_obj->UpdateFrame();
		}
		break;
	}

	// 地面に付いたらパラメータをリセットする
	if (m_isGround){
		EndMotion();
		ParamReset();
	}
}

/**
* @brief 被弾状態
* @param blownOff - 吹き飛ぶスピード
* @param attackerID - 攻撃者の番号
*/
void Rock::Damage(Vector3 blownOff, int attackerID)
{
	// パラメータをリセットする
	ParamReset();

	// 被弾状態
	m_action = Action::Damage;

	// モーションのセット
	SetMotion(MSmallDamage, false, true);

	// 操作不可
	m_isControl = false;

	// 無敵フラグを立てる
	m_isInvincible = true;

	// 吹き飛び速度の設定
	m_spd = blownOff;

	// 絶対値が一定値以上なら吹き飛び状態にする
	if (abs(m_spd.x) > 0.3f){
		m_action = Action::Blow;
	}

	// 相手の番号を登録
	m_enemyID = attackerID;
}

/**
* @brief 被弾状態の更新
*/
void Rock::UpdateDamage()
{
	// アニメーションが終了したら
	if (m_obj->IsAnimPeriod())
	{
		// 地面に付いていたら
		if (m_isGround){
			// アイドリングに戻す
			m_action = Action::Idle;
		}
		// 空中ならジャンプにする
		else{
			m_isBlowRigidity = true;
			m_action = Action::JumpNow;
			SetMotion(MJump3, false, true);
		}

		// 操作可能
		m_isControl = true;
		// 無敵フラグを消す
		m_isInvincible = false;
	}
	else{
		// フレーム更新
		m_obj->UpdateFrame(0.7f);
	}

	// 連続でヒットしないための制御用
	static int frame = 0;
	if (frame > 6){
		m_isInvincible = false;
		frame = 0;
	}
	frame++;
}

/**
* @brief 死亡
* @return 最後に攻撃された相手の番号を返す
*/
int Rock::Dead()
{
	// ジャンプカウントの初期化
	m_jumpCount = 0;

	// ボイスの再生
	SoundPlayer::GetInstance()->Play(ROCK_DEAD_VOICE);

	// モーションの初期化
	EndMotion();

	// パラメータの初期化
	ParamReset();

	// 速度を初期化する
	m_spd = Vector3();

	// 相手の番号
	return m_enemyID;
}

/**
* @brief 通常攻撃開始
*/
void Rock::NormalAttack()
{
	// 通常攻撃
	m_action = Action::NormalAttack;
	// ボイスの再生
	SoundPlayer::GetInstance()->Play(ROCK_NORMALATTACK_VOICE);
}

/**
* @brief スマッシュ攻撃
*/
void Rock::SmashAttack()
{
	// スマッシュ攻撃
	m_action = Action::SmashAttack;
	// ボイスの再生
	SoundPlayer::GetInstance()->Play(ROCK_SMASHATTACK_VOICE);
}

/**
* @brief 必殺攻撃(上)
*/
void Rock::SpecialAttackUP()
{
	// 通常攻撃
	m_action = Action::SpecialAttackUp;
	// ボイスの再生
	SoundPlayer::GetInstance()->Play(ROCK_SPECIALATTACKUP_VOICE);
}

/**
* @brief アピール
*/
void Rock::Appeal()
{
	// 通常攻撃
	m_action = Action::Appeal;
	// ボイスの再生
	SoundPlayer::GetInstance()->Play(ROCK_APPEAL_VOICE);
}

/**
* @brief 通常攻撃
*/
void Rock::UpdateNormalAttack()
{
	switch (m_subWork)
	{
	case 0:
		// モーションの設定
		if (m_nMotion != MVAttack1) SetMotion(MVAttack1, false, true);
		
		// 地面に付いていたら速度初期化
		if (m_isGround) m_spd = Vector3();

		// 攻撃力の設定
		m_power = GetChrPrm->RockNormalAttackPower;

		// 制御不可
		m_isControl = false;

		for (int i = 0; i != m_isAttack.size(); i++){
			m_isAttack[i] = true;
		}

		m_subWork++;
		break;

	case 1:
		// 一定フレーム経過で当たり判定を削除
		if (m_obj->GetTime() > 2400.0f / 4800.0f)
		{
			for (int i = 0; i != m_isAttack.size(); i++){
				m_isAttack[i] = false;
			}
		}
		// アニメーションが終了したらパラメータをリセットする
		if (m_obj->IsAnimPeriod()){
			EndMotion();
			ParamReset();
		}
		else{
			// アニメーション更新
			m_obj->UpdateFrame(1.6f);
		}
		break;
	}
}

/**
* @brief スマッシュ攻撃
*/
void Rock::UpdateSmashAttack()
{
	switch (m_subWork)
	{
	case 0:
		// モーションの設定
		if (m_nMotion != MBAttack1) SetMotion(MBAttack1, false, true);

		// 地面に付いていたら速度初期化
		if (m_isGround) m_spd = Vector3();

		// 攻撃力の設定
		m_power = GetChrPrm->RockSmashAttackPower;

		// 制御不可
		m_isControl = false;

		m_subWork++;
		break;

	case 1:
		// 一定フレーム経過で当たり判定を生成する
		if (m_obj->GetTime() > 1700.0f / 4800.0f)
		{
			for (int i = 0; i != m_isAttack.size(); i++){
				m_isAttack[i] = true;
			}
		}

		// アニメーションの再生が終了したら
		if (m_obj->IsAnimPeriod()){
			// リセットする
			EndMotion();
			ParamReset();
		}
		else{
			// アニメーションを更新
			m_obj->UpdateFrame();
		}
		break;
	}
}

/**
* @brief 必殺攻撃
*/
void Rock::SpecialAttack()
{
	switch (m_subWork)
	{
	case 0:
		// モーションの設定
		if (m_nMotion != MSpecial) SetMotion(MSpecial, false, true);

		// 地面に付いていたら速度初期化
		if (m_isGround) m_spd = Vector3();

		// 攻撃力の設定
		m_power = GetChrPrm->RockSmashAttackPower;

		// 制御不可
		m_isControl = false;

		// 無敵フラグを立てる
		m_isInvincible = true;

		m_subWork++;
		break;

	case 1:
		// アニメーションの再生が終了したら
		if (m_obj->IsAnimPeriod()){
			// 無敵フラグを消す
			m_isInvincible = false;
			// リセットする
			EndMotion();
			ParamReset();
		}
		else{
			// アニメーションを更新
			m_obj->UpdateFrame();
		}
		break;
	}
}

/**
* @brief 必殺攻撃(上)
*/
void Rock::UpdateSpecialAttackUP()
{
	switch (m_subWork)
	{
	case 0:
		// モーションの設定
		if (m_nMotion != MVAttack3)	SetMotion(MVAttack3, false, true);

		// 先に速度を初期化しておく
		m_spd = Vector3();

		// 新たに速度をセットする
		m_spd = Vector3(m_oldSpd.x, 0.4f, m_oldSpd.z);

		// 攻撃力の設定
		m_power = GetChrPrm->RockSpecialAttackUpPower;

		// 制御不可
		m_isControl = false;

		for (int i = 0; i != m_isAttack.size(); i++){
			m_isAttack[i] = true;
		}

		m_subWork++;
		break;

	case 1:
		// 一定時間までアニメーションを更新する
		if (m_obj->GetTime() > 3300.0f / 4800.0f){
		}
		else{
			m_obj->UpdateFrame();
		}

		if (m_isGround){
			EndMotion();
			ParamReset();
		}
		break;
	}
}

/**
* @brief アピール
*/
void Rock::UpdateAppeal()
{
	switch (m_subWork)
	{
	case 0:
		// モーションの設定
		if (m_nMotion != MAppeal1)	SetMotion(MAppeal1, false, true);

		// 速度を初期化
		m_spd = Vector3();

		// 制御不可
		m_isControl = false;
		m_subWork++;
		break;

	case 1:
		// アニメーションの再生が終了したら
		if (m_obj->IsAnimPeriod()){
			// リセットする
			EndMotion();
			ParamReset();
		}
		else{
			// アニメーションを更新
			m_obj->UpdateFrame();
		}
		break;
	}

	// 動かないように調整する
	m_spd.x = 0.0f;
	m_spd.z = 0.0f;
}

/**
* @brief 座標取得
*/
const Vector3& Rock::GetPos() const
{
	return m_pos;
}

/**
* @brief 前フレームの座標を取得する
*/
const Vector3& Rock::GetOldPos() const
{
	return m_oldPos;
}

/**
* @brief 状態を取得する
*/
const Action Rock::GetAction() const
{
	return m_action;
}

/**
* @brief 攻撃力を取得する
*/
const DWORD Rock::GetPower() const
{
	return m_power;
}

/**
* @brief 剣先ボーンを取得する
*/
const Vector3 Rock::GetHitPos() const
{
	Matrix mat;
	
	// 武器の位置
	m_obj->GetPartsMatrixByName(&mat, "Bone002");
	
	Vector3 p(mat._41, mat._42, mat._43);
	
	return p;
}

/**
* @brief ダメージ座標を取得する
*/
const Vector3 Rock::GetDamagePos() const
{
	Matrix mat;
	//プレイヤーヘッドボーン
	m_obj->GetPartsMatrixByName(&mat, "Bip001");
	return Vector3(mat._41, mat._42, mat._43);
}

/**
* @brief スキンメッシュ情報を取得する
*/
const SkinMeshPtr& Rock::GetSkinMesh()
{
	return m_obj;
}

/**
* @brief 座標を設定する
*/
void Rock::SetPos(const Vector3& pos)
{
	m_pos = pos;
}

/**
* @brief 攻撃判定フラグをセットする
*/
void Rock::SetAttack(int enemyID, bool bAttack)
{
	m_isAttack[enemyID] = bAttack;
}

/**
* @brief 地面に付いているかどうかを設定する
*/
void Rock::SetGround(bool bGround)
{
	m_isGround = bGround;
}

/**
* @brief リタイアフラグをセットする
*/
void Rock::SetRetire(bool bRetire)
{
	m_isRetire = bRetire;
}

/**
* @brief モーションを設定する
*/
void Rock::SetMotion(int nMotion, bool bLoop, bool bForce)
{
	//現在と次のモーションが異なる場合
	if (bForce || m_nMotion != nMotion){
		//名前よりモーション番号を取得
		m_obj->SetAnimName(s_szMotionNameArray[nMotion], bLoop);
		//モーションを設定
		m_nMotion = nMotion;
	}
}

/**
* @brief 行動更新
*/
void Rock::UpdateAction()
{
	// 行動に応じた更新処理をする
	switch (m_action)
	{
	case Action::Idle:
		UpdateIdling();
		break;

	case Action::JumpStart:
		m_action = Action::JumpNow;
		break;

	case Action::JumpNow:
		UpdateJump();
		break;

	case Action::NormalAttack:
		UpdateNormalAttack();
		break;

	case Action::SmashAttack:
		UpdateSmashAttack();
		break;

	case Action::SpecialAttackUp:
		UpdateSpecialAttackUP();
		break;

	case Action::Damage:
	case Action::Blow:
		UpdateDamage();
		break;

	case Action::Appeal:
		UpdateAppeal();
		break;
	}
}

/**
* @brief モーションの終了
*/
void Rock::EndMotion()
{
	// アニメーションのシフトタイム
	m_obj->SetAnimShiftTime(0.25f);

	// 地上
	if (m_isGround){
		Idling();
	}
	// 空中
	else {
		m_action = Action::JumpNow;
		SetMotion(MJump3, false, true);
	}
}

/**
* @brief パラメータをリセットする
*/
void Rock::ParamReset()
{
	// サブワークリセット
	m_subWork = 0;
	// 攻撃力のリセット
	m_power = 0;
	// 攻撃判定フラグを消す
	for (int i = 0; i != m_isAttack.size(); i++){
		m_isAttack[i] = false;
	}
	// 制御可能
	m_isControl = true;
}

/**
* @brief 無敵判定があるかどうか
*/
bool Rock::IsInvincible() const
{
	return m_isInvincible;
}

/**
* @brief 攻撃判定があるかどうか
*/
bool Rock::IsAttack(int enemyID) const
{
	return m_isAttack[enemyID];
}

/**
* @brief 地面に着いているかどうか
*/
bool Rock::IsGround() const
{
	return m_isGround;
}

/**
* @brief 操作可能状態かどうか
*/
bool Rock::IsControl() const
{
	return m_isControl;
}

/**
* @brief リタイアかどうかをチェック(ストック数が"0"を意味する)
*/
bool Rock::IsRetire() const
{
	return m_isRetire;
}