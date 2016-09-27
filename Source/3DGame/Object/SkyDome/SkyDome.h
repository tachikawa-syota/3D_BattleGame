/**
* @file   SkyDome.h
* @brief  �X�J�C�h�[��
* @author Syota Tachikawa
*/

#ifndef ___SKYDOME_H
#define ___SKYDOME_H

#include "..\IObject.h"

/**
* @brief �X�J�C�h�[���N���X
*/
class SkyDome : public ISkyDome
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	SkyDome();

	/**
	* @brief �f�X�g���N�^
	*/
	~SkyDome();

	/**
	* @brief �X�V
	*/
	bool Update();

	/**
	* @brief �`��
	*/
	void Render();

	/**
	* @brief �`��
	*/
	void Render(const Matrix& view, const Matrix& proj);

	/**
	* @brief ���[���h�s����擾����
	*/
	const Matrix& GetWorld();

	/**
	* @brief �A���r�G���g���擾����
	*/
	const Vector4& GetAmbient();

	/**
	* @brief ���b�V���̎擾
	*/
	const MeshPtr& GetMesh() const;

private:
	/// �I�u�W�F�N�g
	MeshPtr m_obj;

	/// ���W
	Vector3 m_pos;

	/// ���b�V���̃A���r�G���g
	Vector4 m_ambient;

	/// �T�u���[�N
	int m_subWork;
};

#endif