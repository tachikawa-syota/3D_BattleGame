#include "Camera.h"
#include "..\DirectX\Direct3DData.h"

/**
* @brief �R���X�g���N�^
*/
Camera::Camera(DWORD width, DWORD height)
{
	//�r���[�I�u�W�F�N�g����
	m_view = make_unique<View>(width, height);
	//�J�����������W
	m_pos = Vector3(0.0f, 6.0f, 15.0f);
	//�^�[�Q�b�g
	m_target = Vector3(0.0f, 0.0f, 0.0f);
	//�p�x
	m_angle = Vector3(0.0f, 3.0f, 0.0f);
	//�J�������璍���_�܂ł̍ő�A�ŏ�����
	m_maxDist = MAX_DIST_CAMERA;
	m_minDist = MIN_DIST_CAMERA;
}

/**
* @brief �f�X�g���N�^
*/
Camera::~Camera()
{

}

/**
* @brief �J�����^�C�v�̐؂�ւ�
*/
void Camera::Normal(const Vector3& target)
{
	//�ʒu�X�V
	Vector3	v = target - m_pos;
	v.y = 0;
	float dist = v.Length();
	//�ő勗���ݒ�
	if (dist > m_maxDist){ m_pos = target - v / dist * m_maxDist; }
	//�ŏ������ݒ�
	if (dist < m_minDist){ m_pos = target - v / dist * m_minDist; }
	m_pos.y = target.y + 5.0f;
	//�����_�ݒ�
//	target += +Vector3(0, 0, 0);
}

/**
* @brief �X�V
*/
void Camera::Update(const Vector3& target)
{
	//�J�����ݒ�(�����A�p�x)
	Normal(target);
	//�ݒ�
	m_view->Set(Vector3(m_pos.x, m_pos.y - 1.0f, m_pos.z - 15.0f), target);

	//�J�����ݒ�
	Active();
}

/**
* @brief �`�揀��
*/
void Camera::Active()
{
	//�J�������_�ݒ�
	m_view->Activate(0);

	//��ʃN���A
	m_view->Clear();
}

/**
* @brief ���W���擾����
*/
const Vector3& Camera::GetPos()
{
	return m_pos;
}

/**
* @brief �J�����̍s����擾����
*/
const Matrix& Camera::GetView()
{
	return m_view->GetView();
}

/**
* @brief �J�����̋t�s����擾����
*/
const Matrix& Camera::GetInverseView()
{
	return m_view->GetInverseView();
}

/**
* @brief ���e���擾����
*/
const Matrix& Camera::GetProj()
{
	return m_view->GetProj();
}

/**
* @brief ���W��ݒ肷��
*/
void Camera::SetPos(const Vector3& pos)
{
	m_pos = pos;
}

/**
* @brief �^�[�Q�b�g��ݒ肷��
*/
void Camera::SetTarget(const Vector3& target)
{
	m_target = target;
}

/**
* @brief Y���W��ݒ肷��
*/
void Camera::SetY(float y)
{
	m_pos.y = y;
}