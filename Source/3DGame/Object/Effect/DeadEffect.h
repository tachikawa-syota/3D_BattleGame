/**
* @file   DeadEffect.h
* @brief  ���S�G�t�F�N�g���Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___DEADEFFECT_H
#define ___DEADEFFECT_H

#include "..\IObject.h"
#include "..\..\Common\STDXStruct.h"
#include "..\..\Common\FileData.h"

/**
* @brief ���S�G�t�F�N�g
*/
class DeadEffect : public IEffect
{
public:
	/**
	* @brief �R���X�g���N�^
	* @param pos - �`��ʒu
	*/
	DeadEffect(int type);

	/**
	* @brief �f�X�g���N�^
	*/
	~DeadEffect();

	/**
	* @brief �X�V
	*/
	bool Update();

	/**
	* @brief �`��
	*/
	void Render();

private:
	/// �e�N�X�`��	
	int m_tex[DEADEFFECT_TEXTURE_MAX];

	/// �A�j���[�V�����p
	int m_animation;

	/// ���W
	Vector2 m_pos;

	/// �p�x
	float m_rot;

	/// �t���[��
	int m_frame;
};

#endif