/**
* @file   BombSmokeEffect.h
* @brief  ���G�t�F�N�g�̏���
* @author Syota Tachikawa
*/

#ifndef ___BOMBSMOKEEFFECT_H
#define ___BOMBSMOKEEFFECT_H

#include "..\IObject.h"

/**
* @brief �{������o�鉌�G�t�F�N�g�N���X
*/
class BombSmokeEffect : public IEffect
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	BombSmokeEffect(const Vector3& pos, const Matrix& view);

	/**
	* @brief �f�X�g���N�^
	*/
	~BombSmokeEffect();

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