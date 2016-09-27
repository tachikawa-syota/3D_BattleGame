#include "Object2D.h"

/**
* @brief 前に追加
*/
void Object2D::AddFront(const I2DObjectPtr& obj)
{
	m_objList.emplace_front(obj);
}

/**
* @brief 後ろに追加
*/
void Object2D::AddBack(const I2DObjectPtr& obj)
{
	m_objList.emplace_back(obj);
}

/**
* @brief オブジェクトを更新する
*/
void Object2D::Update()
{
	// イテレータを取得
	auto it = m_objList.begin();

	while (it != m_objList.end())
	{
		// 更新
		if ((*it)->Update()){
			++it;
		}
		// 更新フラグがfalseなら要素を削除する
		else{
			it = m_objList.erase(it);
		}
	}
}

/**
* @brief オブジェクトを描画する
*/
void Object2D::Render()
{
	// 範囲ベースループ
	for (auto& obj : m_objList)
	{
		obj->Render();
	}
}

/**
* @brief リスト内の要素削除
*/
void Object2D::Clear()
{
	m_objList.clear();
}
