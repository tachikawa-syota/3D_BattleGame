/**
* @file   Stage1.h
* @brief �X�e�[�W1
* @author Syota Tachikawa
*/

#ifndef ___STAGE1_H
#define ___STAGE1_H

#include "Tree.h"

/**
* @brief �X�e�[�W�P�N���X
*/
class Stage1 : public IStage
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	Stage1();

	/**
	* @brief �f�X�g���N�^
	*/
	~Stage1();

	/**
	* @brief �X�V
	*/
	bool Update();

	/**
	* @brief �`��
	*/
	void Render(const Matrix& view, const Matrix& proj);

	/**
	* @brief ���W�̎擾
	*/
	const Vector3& GetPos() const;

	/**
	* @brief �X�e�[�W���b�V���̎擾
	*/
	const MeshPtr& GetMesh() const;

private:
	/// �X�e�[�W�I�u�W�F�N�g
	MeshPtr m_stage;

	/// �ʒu
	Vector3 m_pos;
};

#endif