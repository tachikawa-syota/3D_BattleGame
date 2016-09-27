/**
* @file   SnowParticle.h
* @brief  �q�b�g�p�[�e�B�N���̏���
* @author Syota Tachikawa
*/

#ifndef ___SNOWPARTICLE_H
#define ___SNOWPARTICLE_H

#include "..\..\Common\FileData.h"
#include "..\IObject.h"

/**
* @brief ��p�[�e�B�N���N���X
*/
class SnowParticle : public IParticle
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	SnowParticle(const Vector3& pos, const Matrix& view);

	/**
	* @brief �f�X�g���N�^
	*/
	~SnowParticle();

	/**
	* @brief �X�V
	* @return true - �X�V �F false - �I�u�W�F�N�g�폜
	*/
	bool Update();

	/**
	* @brief �`��
	*/
	void Render();

private:
	/// �e�N�X�`���}�l�[�W��
	TextureManager* m_2dTex;

	/// ���W
	Vector3 m_pos;

	/// ���x
	Vector3 m_spd;

	/// �t�s��
	Matrix m_view;

	/// �t���[��
	int m_frame;
};

#endif