/**
* @file   BurstEffect.h
* @brief  �Ռ��g�G�t�F�N�g���Ǘ�
* @author Syota Tachikawa
*/

#ifndef ___BURSTEFFECT_H
#define ___BURSTEFFECT_H

#include "..\IObject.h"

/**
* @brief �Ռ��g�G�t�F�N�g�N���X
*/
class BurstEffect : public IEffect
{
public:
	/**
	* @brief �R���X�g���N�^
	* @param pos - �`��ʒu
	* @param view - �r���[�s��
	*/
	BurstEffect(const Vector3& pos, const Matrix& view);

	/**
	* @brief �f�X�g���N�^
	*/
	~BurstEffect(){};

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