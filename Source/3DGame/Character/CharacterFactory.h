/**
* @file   CharacterFactory.h
* @brief  �L�����N�^�[�̃C���X�^���X�𐶐�����
* @author Syota Tachikawa
*/

#ifndef ___CHARACTERFACTORY_H
#define ___CHARACTERFACTORY_H

#include "Rock.h"

/**
* @brief �L�����N�^�[�����N���X
*/
class CharacterFactory
{
public:
	/**
	* @brief �L�����N�^�[�C���X�^���X����
	* @param type - �^���
	* @param index - �v���C���[�̔ԍ�
	*/
	IPlayerPtr CreateCharacter(const type_info& type, const int index);
};

/**
* @brief �L�����N�^�[�C���X�^���X����
* @param type - �^���
* @param index - �v���C���[�̔ԍ�
*/
IPlayerPtr CharacterFactory::CreateCharacter(const type_info& type, const int index)
{
	// ���͂P�L�����������Ȃ�
	// ���b�N
	if (type == typeid(Rock)){
		return make_shared<Rock>(index);
	}

	// �G���[
	return nullptr;
}

#endif