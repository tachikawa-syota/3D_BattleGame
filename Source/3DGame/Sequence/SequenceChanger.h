/**
* @file   SequenceChanger.h
* @brief  �V�[���̐؂�ւ����Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___SEQUENCECHANGER_H
#define ___SEQUENCECHANGER_H

#include "ISequence.h"

/**
* @brief �V�[���̐؂�ւ����Ǘ�����N���X
*/
class SequenceChanger
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	static SequenceChanger* GetInstance(){
		static SequenceChanger singleton;
		return &singleton;
	}

	/**
	* @brief ������
	*/
	void Initialize();

	/**
	* @brief �V�[���̐؂�ւ�
	* @param �؂�ւ������V�[��������
	*/
	void ChangeScene(const ISequencePtr& next);
	
	/**
	* @brief ���݂̃V�[�����擾����
	*/
	const ISequencePtr& GetNowScene() const;

private:
	/// ���݂̃V�[��
	ISequencePtr m_nowScene;
};

#endif