/**
* @file   CharacterFactory.h
* @brief  キャラクターのインスタンスを生成する
* @author Syota Tachikawa
*/

#ifndef ___CHARACTERFACTORY_H
#define ___CHARACTERFACTORY_H

#include "Rock.h"

/**
* @brief キャラクター生成クラス
*/
class CharacterFactory
{
public:
	/**
	* @brief キャラクターインスタンス生成
	* @param type - 型情報
	* @param index - プレイヤーの番号
	*/
	IPlayerPtr CreateCharacter(const type_info& type, const int index);
};

/**
* @brief キャラクターインスタンス生成
* @param type - 型情報
* @param index - プレイヤーの番号
*/
IPlayerPtr CharacterFactory::CreateCharacter(const type_info& type, const int index)
{
	// 今は１キャラしかいない
	// ロック
	if (type == typeid(Rock)){
		return make_shared<Rock>(index);
	}

	// エラー
	return nullptr;
}

#endif