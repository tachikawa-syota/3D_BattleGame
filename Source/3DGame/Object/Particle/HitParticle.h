/**
* @file   HitParticle.h
* @brief  �q�b�g�p�[�e�B�N���̏���
* @author Syota Tachikawa
*/

#ifndef ___HITPARTICLE_H
#define ___HITPARTICLE_H

#include "..\..\Common\FileData.h"
#include "..\IObject.h"

/**
* @brief �q�b�g�p�[�e�B�N���N���X
*/
class HitParticle : public IParticle
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	HitParticle(const Vector3& pos, const Matrix& view);

	/**
	* @brief �f�X�g���N�^
	*/
	~HitParticle();

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

	/// �A���t�@�l
	int m_alpha;
};

#endif