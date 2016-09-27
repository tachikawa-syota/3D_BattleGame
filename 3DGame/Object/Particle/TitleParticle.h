/**
* @file   TitleParticle.h
* @brief  �^�C�g����ʂŎg�p����p�[�e�B�N���̏���
* @author Syota Tachikawa
*/

#ifndef ___TITLEPARTICLE_H
#define ___TITLEPARTICLE_H

#include "..\..\Common\FileData.h"
#include "..\..\Common\STDXStruct.h"
#include "..\IObject.h"

/**
* @brief �^�C�g����ʂ̃p�[�e�B�N��
*/
class TitleParticle : public IParticle
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	TitleParticle(const Vector2& pos);

	/**
	* @brief �f�X�g���N�^
	*/
	~TitleParticle();

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
	Vector2 m_pos;

	/// ���x
	Vector2 m_spd;

	/// �t���[��
	int m_frame;
};

#endif