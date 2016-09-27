/**
* @file   Object3D.h
* @brief  3Dオブジェクトを管理する
* @author Syota Tachikawa
*/

#ifndef ___OBJECT3D_H
#define ___OBJECT3D_H

#include "..\Character\IPlayer.h"
#include "..\GameSystem\Stock.h"
#include "..\GameSystem\ScoreManager.h"

class DamageData;

/**
* @brief 3Dオブジェクトを管理するクラス
*
* @note：シェーダーを使用するオブジェクトだけをリストに登録する
*/
class Object3D
{
public:
	/**
	* @brief インスタンスを取得する（シングルトン）
	*/
	static Object3D* GetInstance(){
		static Object3D singleton;
		return &singleton;
	}

	/**
	* @brief オブジェクトを追加する
	* @param obj - オブジェクトインターフェイス
	*/
	void Add(const IMeshObjectPtr& obj);

	/**
	* @brief オブジェクトを追加する
	* @param obj - オブジェクトインターフェイス
	*/
	void Add(const ISkinObjectPtr& obj);

	/**
	* @brief プレイヤーオブジェクトを追加する
	* @param obj - オブジェクトインターフェイス
	*/
	void AddPlayer(const IPlayerPtr& obj);

	/**
	* @brief アイテムオブジェクトを追加する
	* @param obj - オブジェクトインターフェイス
	*/
	void AddItem(const IItemPtr& obj, ShaderPtr& shader);

	/**
	* @brief 更新
	*/
	void Update();

	/**
	* @brief 描画(シェーダーを使用する)
	*/
	void Render(CameraPtr& camera);

	/**
	* @brief 影を描画(シャドウマップ)
	*/
	void RenderShadow(CameraPtr& camera);

	/**
	* @brief 全てのオブジェクトの当たり判定をチェックする
	*/
	void CheckAllCollision(CameraPtr& camera, StockPtr& stock, ScoreManagerPtr& score, DamageData& damage);

	/**
	* @brief アイテムリストを取得する
	*/
	list<IItemPtr>& GetItemList();

	/**
	* @brief 削除
	*/
	void Clear();

private:
	/// 3Dオブジェクトリスト
	list<IMeshObjectPtr> m_meshObjList;

	/// 3Dオブジェクトリスト
	list<ISkinObjectPtr> m_skinObjList;

	/// プレイヤーリスト
	vector<IPlayerPtr> m_players;

	/// アイテムリスト
	list<IItemPtr> m_itemList;

	/// メッシュリスト(シャドウマップで使用)
	list<MeshPtr> m_meshList;

	/// メッシュリスト(シャドウマップで使用)
	list<SkinMeshPtr> m_skinList;

	/**
	* @brief プレイヤーと床の当たり判定をチェックする
	*/
	void CheckCollisionPlayerAndFloor(int index);

	/**
	* @brief プレイヤーと攻撃の当たり判定をチェックする
	*/
	void CheckCollisionPlayerAndAttack(CameraPtr& camera, DamageData& damage, int index);

	/**
	* @brief プレイヤーとプレイヤーの当たり判定をチェックする
	*/
	void CheckCollisionPlayerAndPlayer(int index);

	/**
	* @brief アイテムと全てのオブジェクトの当たり判定
	*/
	void CheckCollisionItem(CameraPtr& camera, DamageData& damage, int index);

	/**
	* @brief アイテム同士の当たり判定
	*/
	void CheckCollisionItemAndItem(list<IItemPtr>::iterator& item);

	/**
	* @brief アイテムと床の当たり判定
	*/
	void CheckCollisionItemAndFloor();

	/**
	* @brief プレイヤーの死亡判定をチェックする
	*/
	bool CheckDeadPlayer(StockPtr& stock, int index);
};

#endif