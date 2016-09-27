/**
* @file   ItemFactory.h
* @brief  �A�C�e���̃C���X�^���X�𐶐�����
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
* @brief �L�����N�^�[�����N���X
*/
class ItemFactory
{
public:
	/**
	* @brief �L�����N�^�[�C���X�^���X����
	* @param type - �A�C�e���̎��
	* @param pos- ���W
	*/
	IItemPtr CreateItem(ItemType type, Vector3 pos)
	{
		switch (type)
		{
		// �{��
		case ItemType::Bomb:
			return make_shared<Bomb>(pos);

		// �G���[
		default:
			return nullptr;
		}
	}
};

#endif