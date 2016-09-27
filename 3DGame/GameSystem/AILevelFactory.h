/**
* @file   AILevelFactory.h
* @brief  AI�̃��x���𐶐�����N���X
* @author Syota Tachikawa
*/

#ifndef ___AILEVELFACTORY_H
#define ___AILEVELFACTORY_H

#include "AILevel1.h"
#include "AILevel2.h"
#include "..\Common\Utility.h"

/**
* @enum AILevel
* @brief AI�̃��x��
*/
enum class AILevel : BYTE
{
	/// ���x��1
	Level1,

	/// ���x��2
	Level2
};

/**
* @brief AI�̃��x���𐶐�����N���X
*/
class AILevelFactory
{
public:
	/**
	* @brief �L�����N�^�[�C���X�^���X����
	* @brief ���x��
	*/
	IComputerPtr CreateAI(AILevel level)
	{
		switch (level)
		{
			// ���x��1
		case AILevel::Level1:
			return make_shared<AILevel1>();

			// ���x��2
		case AILevel::Level2:
			return make_shared<AILevel2>();

			// �G���[
		default:
			// �����I�ɃQ�[�����I������
			Utility::abortWithMsg("���݂��Ȃ�AI���x�����I������܂����B�I�����܂�");
			return nullptr;
		}
	}
};

#endif