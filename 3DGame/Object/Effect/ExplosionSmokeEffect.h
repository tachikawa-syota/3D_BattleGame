/**
* @file   ExplosionSmokeEffect.h
* @brief  �Ռ��g�G�t�F�N�g���Ǘ�
* @author Syota Tachikawa
*/

#ifndef ___EXPLOSIONSMOKEEFFECT_H
#define ___EXPLOSIONSMOKEEFFECT_H

#include "..\IObject.h"

/**
* @brief �������G�t�F�N�g�N���X
*/
class ExplosionSmokeEffect : public IEffect
{
public:
	/**
	* @brief �R���X�g���N�^
	* @param pos - �`��ʒu
	* @param view - �r���[�s��
	*/
	ExplosionSmokeEffect(const Vector3& pos, const Matrix& view);

	/**
	* @brief �f�X�g���N�^
	*/
	~ExplosionSmokeEffect(){};

	/**
	* @brief �X�V
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

	/// �p�x
	Vector3 m_rot;

	/// �傫��
	float m_scale;

	/// �A���t�@�l
	int m_alpha;

	/// �r���[�s��
	Matrix m_view;
};

#endif