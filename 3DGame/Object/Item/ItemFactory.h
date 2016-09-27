/**
* @file   ItemFactory.h
* @brief  アイテムのインスタンスを生成する
* @author Syota Tachikawa
*/

#ifndef ___ITEMFACTORY_H
#define ___ITEMFACTORY_H

#include "Bomb.h"

enum class ItemType : BYTE
{
	Bomb = 0,
};

/**
* @brief キャラクター生成クラス
*/
class ItemFactory
{
public:
	/**
	* @brief キャラクターインスタンス生成
	* @param type - アイテムの種類
	* @param pos- 座標
	*/
	IItemPtr CreateItem(ItemType type, Vector3 pos)
	{
		switch (type)
		{
		// ボム
		case ItemType::Bomb:
			return make_shared<Bomb>(pos);

		// エラー
		default:
			return nullptr;
		}
	}
};

#endif