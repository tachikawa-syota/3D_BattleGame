/**
* @file   StageFactory.h
* @brief  �X�e�[�W�̃C���X�^���X�𐶐�����
* @author Syota Tachikawa
*/

#ifndef ___STAGEFACTORY_H
#define ___STAGEFACTORY_H

#include "Stage1.h"

/**
* @enum StageType
* @brief �X�e�[�W�̎��
*/
enum class StageType : BYTE
{
	Stage1
};

/**
* @brief �X�e�[�W�̃C���X�^���X���쐬 (�t�@�N�g�����\�b�h)
*/
class StageFactory
{
public:
	/**
	* @brief �X�e�[�W�̍쐬
	* @param �X�e�[�W�̎��
	*/
	IStagePtr CreateStage(StageType type);
};

/**
* @brief �X�e�[�W�̃C���X�^���X�쐬(�t�@�N�g�����\�b�h)
* @param �X�e�[�W�̎��
*/
IStagePtr StageFactory::CreateStage(StageType type)
{
	// ���̓X�e�[�W��������Ȃ�
	switch (type)
	{
		// �X�e�[�W�P
	case StageType::Stage1:
		return make_unique<Stage1>();
		break;

		// ���蓾�Ȃ�
	default:
		return nullptr;
		break;
	}
}

#endif