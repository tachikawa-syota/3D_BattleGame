/**
* @file   ISequence.h
* @brief  �V�[�N�G���X�C���^�[�t�F�C�X
* @author Syota Tachikawa
*/

#ifndef ___ISEQUENCE_H
#define ___ISEQUENCE_H

#include "..\Common\CommonLib.h"

/**
* @interface ISequence
* @brief �V�[�N�G���X�C���^�[�t�F�C�X�N���X
*
* @note�F�S�ẴV�[���N���X�͂��̃C���^�[�t�F�C�X���p��������
*/
__interface ISequence
{
	/**
	* @brief �쐬
	*/
	void Create();

	/**
	* @brief ������
	*/
	void Initialize();
	
	/**
	* @brief �X�V
	*/
	void Update();

	/**
	* @brief �`��
	*/
	void Render();

	/**
	* @brief �I��
	*/
	void Finalize();
};

/// �V�[�N�G���X�̃X�}�[�g�|�C���^
using ISequencePtr = shared_ptr<ISequence>;

#endif