#include "Light.h"
#include "..\DirectX\Direct3DData.h"

/**
* @brief �R���X�g���N�^
*/
Light::Light()
{
	// �f�o�C�X���Z�b�g
	m_pDevice = D3DIns->GetDevice();
}

/**
* @brief ���C�g�L����
* @param ���C�g�ԍ�
*/
void Light::EnableLight(DWORD index)
{
	m_pDevice->LightEnable(index, TRUE);
}

/**
* @brief ���C�g������
* @param ���C�g�ԍ�
*/
void Light::DisableLight(DWORD index)
{
	m_pDevice->LightEnable(index, FALSE);
}

/**
* @brief �����̐ݒ�
* @param ����
*/
void Light::SetAmbient(DWORD ambient)
{
	m_pDevice->SetRenderState(D3DRS_AMBIENT, ambient);
}

/**
* @brief �t�H�O�̐ݒ�
* @param param1 - �J�n�ʒu
* @param param2 - �I���ʒu
* @param color  - �F
*/
void Light::SetFog(float param1, float param2, DWORD color)
{
	m_pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	//�t�H�O�J���[�ݒ�
	m_pDevice->SetRenderState(D3DRS_FOGCOLOR, color);
	//�t�H�O���[�h�ݒ�
	m_pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
	m_pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	//�p�����[�^�ݒ�
	m_pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&param1));
	m_pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&param2));
}

/**
* @brief ���s�����̐ݒ�
* @param index - ���C�g�̔ԍ�
* @param dir   - ����
* @param r,g,b - RGB�J���[
*/
void Light::DirLight(int index, Vector3 dir, float r, float g, float b)
{
	D3DLIGHT9 Light;
	ZeroMemory(&Light, sizeof(D3DLIGHT9));
	// ���C�g�^�C�v
	Light.Type = D3DLIGHT_DIRECTIONAL;
	// �g�U��
	Light.Diffuse.r = r;
	Light.Diffuse.g = g;
	Light.Diffuse.b = b;
	Light.Diffuse.a = 1.0f;
	// ����
	Light.Specular.r = r;
	Light.Specular.g = g;
	Light.Specular.b = b;
	Light.Specular.a = 1.0f;
	// ����
	Light.Ambient.r = 0.5f;
	Light.Ambient.g = 0.5f;
	Light.Ambient.b = 0.5f;
	Light.Ambient.a = 1.0f;
	Light.Direction.x = dir.x;
	Light.Direction.y = dir.y;
	Light.Direction.z = dir.z;
	// ���C�g�̐ݒ�
	m_pDevice->SetLight(index, &Light);
}

/**
* @brief ���s�����̐ݒ�
* @param shader - �V�F�[�_�[
* @param index - ���C�g�̔ԍ�
* @param dir   - ����
* @param r,g,b - RGB�J���[
*/
void Light::DirLight(ShaderPtr shader, int index, Vector3 dir, float r, float g, float b)
{
	dir.Normalize();
	DirLight(index, dir, r, g, b);
	//	�V�F�[�_�[�ݒ�
	Vector3	work = dir;
	Vector3 color(r, g, b);
	shader->SetValue("LightDir", work);
	shader->SetValue("DirLightColor", &color);
}

/**
* @brief �_�����̐ݒ�
* @param index - ���C�g�̔ԍ�
* @param pos   - ���W
* @param r,g,b - RGB�J���[
* @param range - ���C�g�͈̔�
*/
void Light::PointLight(int index, Vector3 pos, float r, float g, float b, float range)
{
	D3DLIGHT9 Light;
	ZeroMemory(&Light, sizeof(D3DLIGHT9));
	// ���C�g�^�C�v
	Light.Type = D3DLIGHT_POINT;
	// �g�U��
	Light.Diffuse.r = r;
	Light.Diffuse.g = g;
	Light.Diffuse.b = b;
	Light.Diffuse.a = 1.0f;
	// ����
	Light.Specular.r = r;
	Light.Specular.g = g;
	Light.Specular.b = b;
	// ����
	Light.Ambient.r = 0.0f;
	Light.Ambient.g = 0.0f;
	Light.Ambient.b = 0.0f;
	Light.Ambient.a = 0.0f;

	Light.Direction.x = 1.0f;
	Light.Direction.y = 0.0f;
	Light.Direction.z = 0.0f;
	// ���W
	Light.Position.x = pos.x;
	Light.Position.y = pos.y;
	Light.Position.z = pos.z;
	// �͈�
	Light.Range = range;

	Light.Attenuation0 = 1.0f;
	Light.Attenuation1 = 0.8f / range;
	Light.Attenuation2 = 0.0f;
	// ���C�g�̐ݒ�
	m_pDevice->SetLight(index, &Light);
}

/**
* @brief �X�|�b�g���C�g�̐ݒ�
* @param index - ���C�g�̔ԍ�
* @param pos   - ���W
* @param r,g,b - RGB�J���[
* @param range - ���C�g�͈̔�
*/
void Light::SpotLight(int index, Vector3 pos, float r, float g, float b, float range)
{
	D3DLIGHT9 Light;
	ZeroMemory(&Light, sizeof(D3DLIGHT9));
	// ���C�g�^�C�v
	Light.Type = D3DLIGHT_SPOT;
	// �g�U��
	Light.Diffuse.r = r;
	Light.Diffuse.g = g;
	Light.Diffuse.b = b;
	Light.Diffuse.a = 0.5f;
	// ����
	Light.Specular.r = r;
	Light.Specular.g = g;
	Light.Specular.b = b;
	// ����
	Light.Ambient.r = 0.6f;
	Light.Ambient.g = 0.6f;
	Light.Ambient.b = 0.6f;
	Light.Ambient.a = 0.5f;
	// ����
	Light.Direction.x = 0.0f;
	Light.Direction.y = -2.0f;
	Light.Direction.z = 0.0f;
	// �ʒu
	Light.Position.x = pos.x;
	Light.Position.y = pos.y;
	Light.Position.z = pos.z;
	// �����R�[��
	Light.Theta = 4.0f;
	// �O���R�[��
	Light.Phi = 5.0f;
	// �͈�
	Light.Range = range;
	Light.Attenuation0 = 1.0f;
	Light.Attenuation1 = 0.8f / range;
	Light.Attenuation2 = 0.0f;
	// ���C�g�̐ݒ�
	m_pDevice->SetLight(index, &Light);
}