/**
* @file   HitEffect.h
* @brief  �q�b�g�G�t�F�N�g���Ǘ�
* @author Syota Tachikawa
*/

#ifndef ___HITEFFECT_H
#define ___HITEFFECT_H

#include "..\IObject.h"
#include "..\..\Common\FileData.h"

/**
* @brief �q�b�g�G�t�F�N�g
*/
class HitEffect : public IEffect
{
public:
	/**
	* @brief �R���X�g���N�^
	* @param pos - �`��ʒu
	* @param view - �t�s��
	*/
	HitEffect(const Vector3& pos, const Matrix& view);

	/**
	* @brief �f�X�g���N�^
	*/
	~HitEffect(){};

	/**
	* @brief �X�V
	*/
	bool Update();

	/**
	* @brief �`��
	*/
	void Render();

private:
	/// �O���t�B�b�N�X�}�l�[�W��
	TextureManager* m_2dTex;

	/// ���W
	Vector3 m_pos;

	/// ���x
	Vector3 m_spd;

	/// �p�x
	Vector3 m_rot;

	float m_scale;

	Matrix m_view;

	int m_frame;

	/// �A�j���[�V�����e�N�X�`��
	int m_tex[HITEFFECT_ANIM_MAX];

	/// �A�j���[�V�����p
	int m_animation;

	/// �����t���[��
	int m_liveFrame;
};

#endif