/**
* @file   ObjectManager.h
* @brief  全オブジェクトを管理する
* @author Syota Tachikawa
*/

#ifndef ___OBJECTMANAGER_H
#define ___OBJECTMANAGER_H

#include "Object2D.h"
#include "Object3D.h"

/**
* @brief オブジェクトを管理するクラス
*/
class ObjectManager
{
public:
	/**
	* @brief インスタンスを取得する（シングルトン）
	*/
	static ObjectManager* GetInstance(){
		static ObjectManager singleton;
		return &singleton;
	}

	/**
	* @brief コンストラクタ
	*/
	ObjectManager();

	/**
	* @brief 2Dオブジェクト更新
	*/
	void Update2D();

	/**
	* @brief 2Dオブジェクト更新
	*/
	void Update3D();

	/**
	* @brief 描画
	*/
	void Render2D();

	/**
	* @brief 描画(シェーダーを使用する)
	*/
	void Render3D(CameraPtr& camera);

	/**
	* @brief 影を描画(シャドウマップ)
	*/
	void RenderShadow(CameraPtr& camera);

	/**
	* @brief オブジェクトを追加する
	* @param obj - オブジェクトインターフェイス
	*/
	void Add(const IMeshObjectPtr& obj);

	/**
	* @brief 前に追加
	*/
	void AddFront(const I2DObjectPtr& obj);

	/**
	* @brief 後ろに追加
	*/
	void AddBack(const I2DObjectPtr& obj);

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
	/// 2Dオブジェクト
	Object2D* m_obj2D;

	/// 3Dオブジェクト
	Object3D* m_obj3D;
};

#endif