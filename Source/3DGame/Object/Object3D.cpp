#include "Object3D.h"
#include "Object2D.h"
// 共通
#include "..\Common\UniqueData.h"
#include "..\Common\CommonLib.h"
#include "..\Common\Utility.h"
#include "..\Common\STDXDefine.h"
#include "..\Common\CommonParam.h"
// 当たり判定
#include "..\Phyisics\Collision.h"
// エフェクト系
#include "..\Object\Effect\DeadEffect.h"
#include "..\Object\Particle\HitParticle.h"
#include "..\Object\Effect\HitEffect.h"
#include "..\Object\Effect\BurstEffect.h"
#include "..\Object\Effect\ExplosionSmokeEffect.h"
// シェーダー
#include "..\Shader\ShaderManager.h"

/**
* @brief オブジェクトを追加する
* @param obj - オブジェクトインターフェイス
* @param mesh - メッシュデータ
*/
void Object3D::Add(const IMeshObjectPtr& obj)
{
	// オブジェクトリストに追加
	m_meshObjList.emplace_back(obj);
}

/**
* @brief オブジェクトを追加する
* @param obj - オブジェクトインターフェイス
* @param mesh - スキンメッシュデータ
*/
void Object3D::Add(const ISkinObjectPtr& obj)
{
	// オブジェクトリストに追加
	m_skinObjList.emplace_back(obj);
}

/**
* @brief プレイヤーオブジェクトを追加する
* @param obj - オブジェクトインターフェイス
*/
void Object3D::AddPlayer(const IPlayerPtr& obj)
{
	// オブジェクトリストに追加
	m_skinObjList.emplace_back(obj);
	// プレイヤーリストに追加
	m_players.emplace_back(obj);
}

/**
* @brief アイテムオブジェクトを追加する
* @param obj - オブジェクトインターフェイス
*/
void Object3D::AddItem(const IItemPtr& obj, ShaderPtr& shader)
{
	// シェーダーの設定
	obj->GetMesh()->SetShader(shader);
	// アイテムリストに追加
	m_itemList.emplace_back(obj);
}

/**
* @brief オブジェクトを更新する
*/
void Object3D::Update()
{
	// イテレータを取得
	auto mesh = m_meshObjList.begin();
	// リストに追加したオブジェクトを一括更新
	while (mesh != m_meshObjList.end())
	{
		// 更新
		if ((*mesh)->Update()){
			++mesh;
		}
		// 更新フラグがfalseなら要素を削除する
		else{
			mesh = m_meshObjList.erase(mesh);
		}
	}

	// アイテムの更新
	// イテレータを取得
	auto item = m_itemList.begin();
	// リストに追加したオブジェクトを一括更新
	while (item != m_itemList.end())
	{
		// 更新
		if ((*item)->Update()){
			++item;
		}
		// 更新フラグがfalseなら要素を削除する
		else{
			item = m_itemList.erase(item);
		}
	}

	// イテレータ取得
	auto skin = m_skinObjList.begin();
	// リストに追加したオブジェクトを一括更新
	while (skin != m_skinObjList.end())
	{
		// 更新
		if ((*skin)->Update()){
			++skin;
		}
		// 更新フラグがfalseなら要素を削除する
		else{
			skin = m_skinObjList.erase(skin);
		}
	}
}

/**
* @brief オブジェクトを描画する
*/
void Object3D::Render(CameraPtr& camera)
{
	// シェーダーマネージャーのインスタンスを取得する
	ShaderManager* mgr = ShaderManager::GetInstance();

	// リストに追加したスキンオブジェクトを一括描画
	// 範囲ベースループ
	for (auto& it : m_skinObjList)
	{
		
		// 描画する前に
		// シェーダーパラメータの設定を行なう
		mgr->Update(it->GetSkinMesh()->GetTransMatrix(), camera);
		// 描画
		it->Render(camera->GetView(), camera->GetProj());
	}

	// アイテムの描画
	for (auto& item : m_itemList)
	{
		// 描画する前に
		// シェーダーパラメータの設定を行なう
		mgr->Update(item->GetMesh()->GetTransMatrix(), camera);
		// 描画
		item->Render(camera->GetView(), camera->GetProj());
	}

	// リストに追加した剛体オブジェクトを一括描画
	// 範囲ベースループ
	for (auto& it : m_meshObjList)
	{
		// 描画する前に
		// シェーダーパラメータの設定を行なう
		mgr->Update(it->GetMesh()->GetTransMatrix(), camera);
		// 描画
		it->Render(camera->GetView(), camera->GetProj());
	}
}

/**
* @brief 影を描画(シャドウマップ)
*/
void Object3D::RenderShadow(CameraPtr& camera)
{
	// シェーダーマネージャーのインスタンスを取得する
	ShaderManager* mgr = ShaderManager::GetInstance();

	// シャドウマップシェーダー開始
	mgr->GetShadowMapShader()->Begine();

	//	for (auto& it : m_skinObjList)
	//	{
	//		// 描画する前に
	//		// シェーダーパラメータの設定
	//		mgr->Update(it->GetSkinMesh()->GetTransMatrix(), camera);
	//		// 影の描画
	//		it->GetSkinMesh()->DrawShadow();
	//	}

	// 剛体メッシュ
	for (auto& it : m_meshObjList)
	{
		// 描画する前に
		// シェーダーパラメータの設定
		mgr->Update(it->GetMesh()->GetTransMatrix(), camera);
		// 影の描画
		it->GetMesh()->DrawShadow();
	}

	// アイテムの描画
	for (auto& item : m_itemList)
	{
		// 描画する前に
		// シェーダーパラメータの設定を行なう
		mgr->Update(item->GetMesh()->GetTransMatrix(), camera);
		// 描画
		item->GetMesh()->DrawShadow();
	}

	//  シャドウマップシェーダー終了
	mgr->GetShadowMapShader()->End();
}

/**
* @brief 当たり判定をチェックする
*/
void Object3D::CheckAllCollision(CameraPtr& camera,
	StockPtr& stock,
	ScoreManagerPtr& score,
	DamageData& damage)
{
	// 当たり判定のインスタンス
	Collision* collision = Collision::GetInstance();

	// プレイヤーの数だけループ
	for (int i = 0; i != m_players.size(); i++)
	{
		// リタイアしてたらそのプレイヤー番号は処理をしない
		if (m_players[i]->IsRetire()) continue;

		// プレイヤーと床の当たり判定
		CheckCollisionPlayerAndFloor(i);

		// プレイヤーと攻撃の当たり判定
		CheckCollisionPlayerAndAttack(camera, damage, i);

		// プレイヤーとプレイヤーの当たり判定
		CheckCollisionPlayerAndPlayer(i);

		// アイテムの当たり判定
		CheckCollisionItem(camera, damage, i);

		// プレイヤーの死亡判定
		if (CheckDeadPlayer(stock, i))
		{
			// 死亡(誰に倒されたかを戻り値で取得)
			int killPlayer = m_players[i]->Dead();

			// 撃破したプレイヤーのスコアを加算する
			if (killPlayer != -1){
				score->AddScore(killPlayer);
			}

			// 撃破されたプレイヤーはスコアを減算する
			score->SubScore(i);

			// ダメージ初期化
			damage.SetReset(true, i);

			// 乱数を取る
			int rand = Utility::Random(0, 3);
			float returnPosX;
			// 結果に応じて復帰座標を変える
			if		(rand == 0) returnPosX = GetComPrm->Pattern1ReturnPosX;
			else if (rand == 1) returnPosX = GetComPrm->Pattern2ReturnPosX;
			else if (rand == 2) returnPosX = GetComPrm->Pattern3ReturnPosX;
			else if (rand == 3) returnPosX = GetComPrm->Pattern4ReturnPosX;
			Vector3 pos = Vector3(returnPosX, GetComPrm->ReturnPosY, GetComPrm->InitPosZ);
			// 座標のセット
			m_players[i]->SetPos(pos);
		}
	}

	// アイテムと床の当たり判定
	CheckCollisionItemAndFloor();
}

/**
* @brief プレイヤーと床の当たり判定をチェックする
*/
void Object3D::CheckCollisionPlayerAndFloor(int index)
{
	// 当たり判定のインスタンス
	Collision* collision = Collision::GetInstance();

	// プレイヤーの数を取得
	int plNum = m_players.size();

	// 当たり判定
	vector<bool> isGround;
	if (isGround.size() == NULL){
		isGround.resize(plNum);
	}
	// 当たっていない
	isGround[index] = false;

	// プレイヤーの座標を取得する
	Vector3 playerPos = m_players[index]->GetPos();

	// 当たり判定
	if (playerPos.y < collision->GetHeight(playerPos))
	{
		// 当たっている
		isGround[index] = true;

		// 座標補正
		playerPos.y = collision->GetHeight(playerPos);

		// 座標セット
		m_players[index]->SetPos(playerPos);
	}

	// 地面に付いてるかセット
	m_players[index]->SetGround(isGround[index]);
}

/**
* @brief プレイヤーと攻撃の当たり判定をチェックする
*/
void Object3D::CheckCollisionPlayerAndAttack(
	CameraPtr& camera,
	DamageData& damage,
	int index)
{
	// 当たり判定のインスタンス
	Collision* collision = Collision::GetInstance();
	Object2D* mgr2d = Object2D::GetInstance();

	for (int i = 0; i != m_players.size(); i++)
	{
		// 自分は除く
		if (i == index){
			continue;
		}
		// 対象プレイヤーに攻撃判定が無かったら当たらない
		if (!m_players[index]->IsAttack(i)) {
			continue;
		}
		// 無敵フラグが立っているキャラクターも除く(当たらない)
		if (m_players[i]->IsInvincible()){
			continue;
		}

		// 被弾範囲を取得
		Vector3 damagePos = m_players[i]->GetDamagePos();
		// 攻撃範囲を取得
		Vector3 hitPos = m_players[index]->GetHitPos();

		// 当たり判定(半径1.0fの球)
		if (collision->CheckShpere(damagePos, 1.0f, hitPos, 1.0f))
		{
			// 吹き飛び速度
			Vector3 blown = Utility::BlownOffSpeed(typeid(Rock), m_players, index, i, damage.GetDamageRate(i));

			// 被弾状態にする
			m_players[i]->Damage(blown, index);

			// 吹き飛び率を加算
			damage.AddDamageRate(m_players[index]->GetPower(), i);

			// 効果音再生
			SoundPlayer::GetInstance()->Play(HIT_SE);

			// エフェクト追加
			Vector3 effectPos = m_players[i]->GetDamagePos();
			mgr2d->AddFront(make_shared<HitEffect>(effectPos, camera->GetView()));
			for (int effect = 0; effect < 5; effect++){
				mgr2d->AddFront(make_shared<HitParticle>(effectPos, camera->GetView()));
			}

			// 攻撃が当たったプレイヤーには連続して攻撃が当たらない
			m_players[index]->SetAttack(i, false);
		}
	}
}

/**
* @brief プレイヤーとプレイヤーの当たり判定をチェックする
*/
void Object3D::CheckCollisionPlayerAndPlayer(int index)
{
	// 当たり判定のインスタンス
	Collision* collision = Collision::GetInstance();

	for (int i = 0; i != m_players.size(); i++)
	{
		// 自分は除く
		if (i == index){
			continue;
		}

		// プレイヤーの座標を取得する
		Vector3 obj1 = m_players[index]->GetPos();
		Vector3 obj2 = m_players[i]->GetPos();

		// 差分取得
		Vector3 diffVec = Utility::DiffVector(obj2, obj1);

		// 当たり判定
		if (collision->CheckShpere(obj1, 1.0f, obj2, 1.0f))
		{
			// 座標の修正
			m_players[index]->SetPos(m_players[index]->GetPos() + diffVec * 0.07f);
		}
	}
}

// 落下判定の座標
#define FALLING_DECISION_POS -30.0f
// 左側の死亡判定
#define DEAD_DECISION_LEFT_POS 40.0f
// 右側の死亡判定
#define DEAD_DECISION_RIGHT_POS -40.0f

/**
* @brief プレイヤーの死亡判定をチェックする
*/
bool Object3D::CheckDeadPlayer(StockPtr& stock, int index)
{
	Object2D* mgr2d = Object2D::GetInstance();

	// 判定
	bool result = false;

	// 落下判定
	if (m_players[index]->GetPos().y < FALLING_DECISION_POS)
	{
		// エフェクト登録
		mgr2d->AddFront(make_shared<DeadEffect>(0));

		// 落下判定をＯＮにする
		result = true;
	}

	// 左側の判定
	if (m_players[index]->GetPos().x > DEAD_DECISION_LEFT_POS)
	{
		// エフェクト登録
		mgr2d->AddFront(make_shared<DeadEffect>(1));

		// 落下判定をＯＮにする
		result = true;
	}

	// 右側の判定
	if (m_players[index]->GetPos().x < DEAD_DECISION_RIGHT_POS)
	{
		// エフェクト登録
		mgr2d->AddFront(make_shared<DeadEffect>(2));

		// 落下判定をＯＮにする
		result = true;
	}

	// 死亡判定が立っていたら
	if (result)
	{
		// ストックモードだったらストック数を減らす
		if (UniqueData::GetInstance()->GetBattleMode() == BattleMode::Stock){
			stock->MinusStock(index);
		}
	}

	// 結果
	return result;
}

/**
* @brief アイテムと全てのオブジェクトの当たり判定
*/
void Object3D::CheckCollisionItem(CameraPtr& camera, DamageData& damage, int index)
{
	Object2D* mgr2d = Object2D::GetInstance();
	// 当たり判定のインスタンス
	Collision* collision = Collision::GetInstance();

	// プレイヤーの座標を取得する
	Vector3 playerPos = m_players[index]->GetPos();

	// イテレータを取得
	auto item = m_itemList.begin();

	while (item != m_itemList.end())
	{
		// アイテムの座標を取得する
		Vector3 itemPos = (*item)->GetPos();

		// アイテム同士の当たり判定
		CheckCollisionItemAndItem(item);

		// 攻撃範囲を取得
		Vector3 hitPos = m_players[index]->GetHitPos();

		bool flag = false;

		// プレイヤーの攻撃とアイテムの当たり判定
		if (collision->CheckShpere(hitPos, 1.0f, itemPos, 1.0f))
		{
			// 攻撃判定が出ているときだけ
			if (m_players[index]->GetAction() == Action::NormalAttack ||
				m_players[index]->GetAction() == Action::SpecialAttackUp ||
				m_players[index]->GetAction() == Action::SmashAttack){

				// 吹き飛び速度
				Vector3 blown = Utility::BlownOffSpeed(m_players, index, itemPos, 1.0f);
				blown.z = 0.0f;
				// 効果音再生
				SoundPlayer::GetInstance()->Play(HIT_SE);

				// エフェクト追加
				Vector3 effectPos = (*item)->GetPos();
				mgr2d->AddFront(make_shared<HitEffect>(effectPos, camera->GetView()));
				for (int effect = 0; effect < 15; effect++){
					mgr2d->AddBack(make_shared<HitParticle>(effectPos, camera->GetView()));
				}

				// ダメージフラグをＯＮ
				(*item)->SetDamage(true, index);

				// 速度の設定
				(*item)->SetSpeed(blown);
			}
			else{
				flag = true;
			}
		}

		// 落下判定を取ったら削除する
		if ((*item)->GetPos().y < FALLING_DECISION_POS){
			item = m_itemList.erase(item);
			continue;
		}
		// 左側の判定
		if ((*item)->GetPos().x > DEAD_DECISION_LEFT_POS)
		{
			item = m_itemList.erase(item);
			continue;
		}

		// 右側の判定
		if ((*item)->GetPos().x < DEAD_DECISION_RIGHT_POS)
		{
			item = m_itemList.erase(item);
			continue;
		}

		// ダメージフラグが立っていなかったら終了
		if (!(*item)->IsDamage()) {
			++item;
			continue;
		}

		// 削除フラグ
		bool deadFlag = false;

		for (int i = 0; i != m_players.size(); i++)
		{
			// 座標を取得
			Vector3 pos = m_players[i]->GetPos();
			// ダメージフラグＯＮのアイテムとプレイヤー自身の当たり判定
			if (collision->CheckShpere(itemPos, 1.3f, pos, 1.3f) && flag)
			{
				// 吹き飛び速度
				Vector3 blown = Utility::BlownOffSpeed(m_players, i, itemPos, 50.0f);

				// 被弾状態にする
				m_players[i]->Damage(blown, (*item)->GetID());

				// 吹き飛び率を加算(とりあえず一律で設定しておく)
				damage.AddDamageRate(60, i);

				// 効果音再生
				SoundPlayer::GetInstance()->Play(BOMB_HIT_SE);

				// エフェクト追加
				Vector3 effectPos = itemPos;
				// 煙エフェクト
				mgr2d->AddFront(make_shared<ExplosionSmokeEffect>(effectPos, camera->GetView()));
				for (int e = 0; e < 3; e++){
				// 衝撃波エフェクト
					mgr2d->AddFront(make_shared<BurstEffect>(effectPos, camera->GetView()));
				}
				// 被弾エフェクト
				for (int effect = 0; effect < 15; effect++){
					mgr2d->AddFront(make_shared<HitParticle>(effectPos, camera->GetView()));
				}

				// 攻撃が当たったプレイヤーには連続して攻撃が当たらない
				m_players[i]->SetAttack(i, false);

				// 削除する
				deadFlag = true;
			}
		}

		// 削除フラグが立っていたら消す
		if (deadFlag){
			item = m_itemList.erase(item);
		}
		else{
			++item;
		}
	}
}

/**
* @brief アイテム同士の当たり判定
*/
void Object3D::CheckCollisionItemAndItem(list<IItemPtr>::iterator& item)
{
	// 当たり判定のインスタンス
	Collision* collision = Collision::GetInstance();

	// イテレータを取得
	auto it = m_itemList.begin();
	while (it != m_itemList.end())
	{
		// 自分は除く
		if (it == item){
			++it;
			continue;
		}

		// アイテムの座標を取得する
		Vector3 itemPos = (*item)->GetPos();
		Vector3 itemPos2 = (*it)->GetPos();

		// 差分取得
		Vector3 diffVec = Utility::DiffVector(itemPos, itemPos2);

		// 当たり判定
		if (collision->CheckShpere(itemPos, 1.0f, itemPos2, 1.0f))
		{
			// 座標の修正
			(*it)->SetPos(itemPos2 + diffVec * 0.07f);
		}
		++it;
	}
}

/**
* @brief アイテムと床の当たり判定
*/
void Object3D::CheckCollisionItemAndFloor()
{
	// 当たり判定のインスタンス
	Collision* collision = Collision::GetInstance();

	auto item = m_itemList.begin();

	while (item != m_itemList.end())
	{
		// アイテムの座標を取得する
		Vector3 itemPos = (*item)->GetPos();

		// アイテムと床の当たり判定
		if (itemPos.y < collision->GetHeight(itemPos))
		{
			// 座標補正
			itemPos.y = collision->GetHeight(itemPos);

			// 接地フラグＯＮ
			(*item)->SetGround(true);

			// 座標セット
			(*item)->SetPos(itemPos);
		}
		else{
			// 接地フラグＯＦＦ
			(*item)->SetGround(false);
		}

		++item;
	}
}

/**
* @brief アイテムリストを取得する
*/
list<IItemPtr>& Object3D::GetItemList()
{
	return m_itemList;
}

/**
* @brief リスト内の要素削除
*/
void Object3D::Clear()
{
	m_meshObjList.clear();
	m_skinObjList.clear();
	m_players.clear();
	m_itemList.clear();
}