#include "ObjectManager.h"

/**
* @brief コンストラクタ
*/
ObjectManager::ObjectManager()
{
	m_obj2D = Object2D::GetInstance();
	m_obj3D = Object3D::GetInstance();
}

/**
* @brief 2Dオブジェクト更新
*/
void ObjectManager::Update2D()
{
	m_obj2D->Update();
}

/**
* @brief 2Dオブジェクト更新
*/
void ObjectManager::Update3D()
{
	m_obj3D->Update();
}

/**
* @brief 描画
*/
void ObjectManager::Render2D()
{
	m_obj2D->Render();
}

/**
* @brief 描画
*/
void ObjectManager::Render3D(CameraPtr& camera)
{
	m_obj3D->Render(camera);
}

/**
* @brief 影を描画(シャドウマップ)
*/
void ObjectManager::RenderShadow(CameraPtr& camera)
{
	m_obj3D->RenderShadow(camera);
}

/**
* @brief オブジェクトを追加する
* @param obj - オブジェクトインターフェイス
*/
void ObjectManager::Add(const IMeshObjectPtr& obj)
{
	m_obj3D->Add(obj);
}

/**
* @brief 前に追加
*/
void ObjectManager::AddFront(const I2DObjectPtr& obj)
{
	m_obj2D->AddFront(obj);
}

/**
* @brief 後ろに追加
*/
void ObjectManager::AddBack(const I2DObjectPtr& obj)
{
	m_obj2D->AddBack(obj);
}

/**
* @brief プレイヤーオブジェクトを追加する
* @param obj - オブジェクトインターフェイス
*/
void ObjectManager::AddPlayer(const IPlayerPtr& obj)
{
	m_obj3D->AddPlayer(obj);
}

/**
* @brief アイテムオブジェクトを追加する
* @param obj - オブジェクトインターフェイス
*/
void ObjectManager::AddItem(const IItemPtr& obj, ShaderPtr& shader)
{
	m_obj3D->AddItem(obj, shader);
}

/**
* @brief 全てのオブジェクトの当たり判定をチェックする
*/
void ObjectManager::CheckAllCollision(CameraPtr& camera, StockPtr& stock, ScoreManagerPtr& score, DamageData& damage)
{
	m_obj3D->CheckAllCollision(camera, stock, score, damage);
}

/**
* @brief アイテムリストを取得する
*/
list<IItemPtr>& ObjectManager::GetItemList()
{
	return m_obj3D->GetItemList();
}

/**
* @brief 削除
*/
void ObjectManager::Clear()
{
	m_obj2D->Clear();
	m_obj3D->Clear();
}

