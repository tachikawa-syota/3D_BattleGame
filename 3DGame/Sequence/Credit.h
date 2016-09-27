/**
* @file   Credit.h
* @brief  �N���W�b�g��ʂ̊Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___CREDIT_H
#define ___CREDIT_H

#include "ISequence.h"
#include "..\DirectX\InputMask.h"

/**
* @brief �N���W�b�g��ʃN���X
*/
class Credit : public ISequence
{
public:
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

private:
	/// ���̓f�o�C�X
	InputPtr m_dev;
};

#endif