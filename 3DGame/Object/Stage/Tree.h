/**
* @file  Tree.h
* @brief �X�e�[�W�P�ɓo�ꂷ��؃I�u�W�F�N�g
* @author Syota Tachikawa
*/

#ifndef ___TREE_H
#define ___TREE_H

#include "..\IObject.h"

/// �؃I�u�W�F�N�g�̍ő吔
const int TREE_OBJECT_MAX = 2;

/**
* @brief �X�e�[�W�P�̖؃I�u�W�F�N�g�N���X
*/
class Tree : public IMeshObject
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	Tree(const Vector3& pos);

	/**
	* @brief �f�X�g���N�^
	*/
	~Tree();

	/**
	* @brief �X�V
	*/
	bool Update();

	/**
	* @brief �`��
	*/
	void Render(const Matrix& view, const Matrix& proj);

	/**
	* @brief ���b�V�������擾����
	*/
	const MeshPtr& GetMesh() const;

private:
	/// �؃I�u�W�F�N�g
	MeshPtr m_obj;

	// ���W
	Vector3 m_pos;
};

/// �؃I�u�W�F�N�g�̃X�}�[�g�|�C���^
using TreePtr = shared_ptr<Tree>;

#endif