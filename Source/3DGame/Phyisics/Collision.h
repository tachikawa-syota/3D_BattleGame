/**
* @file   Collision.h
* @brief  �����蔻����Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___COLLISION_H
#define ___COLLISION_H

#include "..\DirectX\Mesh.h"

/**
* @brief �����蔻��N���X
*/
class Collision
{
public:
	/**
	* @brief �Q�b�g�C���X�^���X
	*/
	static Collision* GetInstance(){
		static Collision singleton;
		return &singleton;
	}

	/**
	* @brief �f�X�g���N�^
	*/
	~Collision();

	/**
	* @brief �X�e�[�W�̃��b�V����ݒ肷��
	* @param �X�e�[�W�̃��b�V�����
	*/
	void SetStageObj(MeshPtr obj);

	/**
	* @brief ���Ƃ̓����蔻��
	* @param �v���C���[�̍��W
	*/
	float GetHeight(const Vector3& pos);

	/**
	* @brief ���̓��m�̓����蔻��
	* @param pos - �I�u�W�F�N�g�̍��W
	* @param radius - �I�u�W�F�N�g�̔��a
	*/
	bool CheckShpere(const Vector3& posA, float radiusA, const Vector3& posB, float radiusB);

private:
	/// �X�e�[�W
	MeshPtr m_stage;
};

#endif