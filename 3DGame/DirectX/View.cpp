#include "View.h"
#include "Direct3DData.h"

/**
* @brief �R���X�g���N�^
*/
View::View(DWORD width, DWORD height)
{
	m_width = width;
	m_height = height;
	Set(Vector3(0.0f, 0.0f, 0.0f), Vector3(5.0f, 5.0f, 0.0f));
	SetViewport();
	//�r���[�ϊ�
	SetProjection(D3DX_PI / 2.0f, 0.1f, 200.0f, static_cast<float>(m_width) / static_cast<float>(m_height));
}

/**
* @brief �f�X�g���N�^
*/
View::~View()
{

}

/**
* @brief �K�p
*/
void View::Activate(int type)
{
	//�r���[�ϊ�
	LookAtLH(m_matView, m_pos, m_target, Vector3(0.0f, 1.0f, 0.0f));

	////���e�s��ݒ�
	if (type == 0){
		PerspectiveLH(m_matProj, m_fovY, m_aspect, m_near, m_far);
	}
	else{
		OlthoLH(m_matProj, static_cast<float>(m_width), static_cast<float>(m_height), m_near, m_far);
	}
	//DirectX�ݒ�
	D3DIns->GetDevice()->SetTransform(D3DTS_VIEW, &m_matView);
	D3DIns->GetDevice()->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

/**
* @brief ��ʃN���A
*/
void View::Clear()
{
	//Z�o�b�t�@�ƃo�b�N�o�b�t�@�̃N���A
	D3DIns->GetDevice()->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
}

/**
* @brief ���_�ݒ�
*/
void View::Set(const Vector3& pos, const Vector3& target)
{
	m_pos = pos;
	m_target = target;
}

/**
* @brief ���_�ݒ�
*/
void View::Set(float x, float y, float z, float ax, float ay, float az)
{
	float sx = sinf(ax);
	float sy = sinf(ay);
	float cx = cosf(ax);
	float cy = cosf(ay);
	Vector3	p, t;
	p.x = x;
	p.y = y;
	p.z = z;
	t.x = x + (cx*sy);
	t.y = y + (-sx);
	t.z = z + (cx*cy);
	Set(p, t);
}

/**
* @brief ���e���ʐݒ�
*/
void View::SetViewport()
{
	m_viewport.X = 0;
	m_viewport.Y = 0;
	m_viewport.Width = m_width;
	m_viewport.Height = m_height;
	m_viewport.MaxZ = 1.0f;
	m_viewport.MinZ = 0.0f;
	m_aspect = static_cast<float>(m_viewport.Width) / static_cast<float>(m_viewport.Height);
}

/**
* @brief ���e���ʐݒ�
*/
void View::SetViewport(int x, int y, int w, int h)
{
	m_viewport.X = x;
	m_viewport.Y = y;
	m_viewport.Width = w;
	m_viewport.Height = h;
	m_viewport.MaxZ = 1.0f;
	m_viewport.MinZ = 0.0f;
	m_aspect = static_cast<float>(w) / static_cast<float>(h);
}

/**
* @brief ���e�ݒ�
*/
void View::SetProjection(float FovY, float Near, float Far)
{
	if (m_viewport.Width == 0){
		m_viewport.Width = m_width;
		m_viewport.Height = m_height;
	}
	SetProjection(FovY, Near, Far, static_cast<float>(m_viewport.Width) / static_cast<float>(m_viewport.Height));
}

/**
* @brief ���e�ݒ�
*/
void View::SetProjection(float FovY, float Near, float Far, float asp)
{
	m_fovY = FovY;
	m_near = Near;
	m_far = Far;
	m_aspect = asp;
}

/**
* @brief ���_�擾
*/
const Matrix& View::GetView()
{
	return m_matView; 
}

/**
* @brief ���e�s��擾
*/
const Matrix& View::GetProj()
{
	return m_matProj; 
}

/**
* @brief ���_�̋t�s��擾
*/
const Matrix& View::GetInverseView()
{
	static Matrix invMat;
	D3DXMatrixInverse(&invMat, NULL, &m_matView);

	return invMat;
}

/**
* @brief ���W�擾
*/
const Vector3& View::GetPos()
{
	return m_pos; 
}

/**
* @brief �����_�擾
*/
const Vector3& View::GetTarget()
{
	return m_target; 
}

/**
* @brief �����擾
*/
DWORD View::GetWidth()
{
	return m_width; 
}

/**
* @brief �c���擾
*/
DWORD View::GetHeight()
{
	return m_height; 
}