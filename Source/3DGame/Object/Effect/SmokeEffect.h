/**
* @file   SmokeEffect.h
* @brief  ���G�t�F�N�g�̏���
* @author Syota Tachikawa
*/

#ifndef ___SMOKEEFFECT_H
#define ___SMOKEEFFECT_H

#include "..\..\Common\FileData.h"
#include "..\IObject.h"

/**
* @brief ���G�t�F�N�g�N���X
*/
class SmokeEffect : public IEffect
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	SmokeEffect(const Vector3& pos, const Matrix& view);

	/**
	* @brief �f�X�g���N�^
	*/
	~SmokeEffect();

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

	/// �r���[
	Matrix m_view;

	/// �A���t�@�l
	int m_alpha;
};

#endif