/**
* @file   View.h
* @brief  ���_�̏����Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___VIEW_H
#define ___VIEW_H

#include "..\Math\Math.h"

/**
* @brief ���_�N���X
*/
class View
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	View(DWORD width, DWORD height);

	/**
	* @brief �f�X�g���N�^
	*/
	~View();

	/**
	* @brief �K�p
	*/
	void Activate(int type);

	/**
	* @brief ��ʃN���A
	*/
	void Clear();

	/**
	* @brief ���_�ݒ�
	*/
	void Set(const Vector3& pos, const Vector3& target);
	/**
	* @brief ���_�ݒ�
	*/
	void Set(float x, float y, float z, float ax, float ay, float az);
	
	/**
	* @brief �ʒu�ݒ�
	*/
	void SetPos(float x, float y, float z);
	
	/**
	* @brief �p�x�ݒ�
	*/
	void SetAngle(float x, float y, float z);

	/**
	* @brief ���e���ʐݒ�
	*/
	void SetViewport();

	/**
	* @brief ���e���ʐݒ�
	*/
	void SetViewport(int x, int y, int w, int h);

	/**
	* @brief ���e�ݒ�
	*/
	void SetProjection(float FovY, float Near, float Far);

	/**
	* @brief ���e�ݒ�
	*/
	void SetProjection(float FovY, float Near, float Far, float asp);
	
	/**
	* @brief ���_�擾
	*/
	const Matrix& GetView();

	/**
	* @brief ���e�s��擾
	*/
	const Matrix& GetProj();

	/**
	* @brief ���_�̋t�s��擾
	*/
	const Matrix& GetInverseView();

	/**
	* @brief ���W�擾
	*/
	const Vector3& GetPos();

	/**
	* @brief �����_�擾
	*/
	const Vector3& GetTarget();
	
	/**
	* @brief �����擾
	*/
	DWORD GetWidth();

	/**
	* @brief �c���擾
	*/
	DWORD GetHeight();

private:
	/// ����
	DWORD m_width;

	/// �c��
	DWORD m_height;

	/// ���_�s��
	Matrix m_matView;

	/// ���e�s��
	Matrix m_matProj;

	/// ���W
	Vector3 m_pos;

	/// �����_
	Vector3 m_target;

	/// �\���̈�
	D3DVIEWPORT9 m_viewport;

	// ���e�p�����[�^
	float m_fovY;
	float m_near, m_far;
	float m_aspect;
	RECT m_rViewport;
};

#endif