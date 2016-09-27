/**
* @file   Camera.h
* @brief  �J�������Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___CAMERA_H
#define ___CAMERA_H

#include "..\DirectX\View.h"

/// �J�����ƒ����_�̍ő勗��
#define MAX_DIST_CAMERA 100.0f
/// �J�����ƒ����_�̍ŏ�����
#define MIN_DIST_CAMERA -100.0f

/**
* @brief �J�����N���X
*/
class Camera
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	Camera(DWORD width, DWORD height);
	
	/**
	* @brief �f�X�g���N�^
	*/
	~Camera();

	/**
	* @brief �X�V
	*/
	void Update(const Vector3& target);
	
	/**
	* @brief �`�揀��
	*/
	void Active();

	/**
	* @brief ���W���擾����
	*/
	const Vector3& GetPos();
	
	/**
	* @brief �J�����̍s����擾����
	*/
	const Matrix& GetView();

	/**
	* @brief �J�����̋t�s����擾����
	*/
	const Matrix& GetInverseView();

	/**
	* @brief ���e���擾����
	*/
	const Matrix& GetProj();

	/**
	* @brief ���W��ݒ肷��
	*/
	void SetPos(const Vector3& pos);

	/**
	* @brief �^�[�Q�b�g��ݒ肷��
	*/
	void SetTarget(const Vector3& target);

	/**
	* @brief Y���W��ݒ肷��
	*/
	void SetY(float y);

private:
	/// �r���[
	unique_ptr<View> m_view;
	
	/// �J�����̍��W
	Vector3 m_pos;

	/// �^�[�Q�b�g�̍��W
	Vector3 m_target;

	/// �p�x
	Vector3 m_angle;

	/// �ő勗��
	float m_maxDist;

	/// �ŏ�����
	float m_minDist;

	/**
	* @brief �J�����^�C�v�̐؂�ւ�
	*/
	void Normal(const Vector3& pPos);
};

/// �J�����I�u�W�F�N�g�̃X�}�[�g�|�C���^
using CameraPtr = shared_ptr<Camera>;

#endif