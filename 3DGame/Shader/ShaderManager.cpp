#include "ShaderManager.h"
#include "..\ApplicationSystem\ApplicationData.h"
#include "..\DirectX\Direct3DData.h"

/**
* @brief �V�F�[�_�[��ǉ�����
* @param shader - �V�F�[�_�[
* @param tecName - �e�N�j�b�N�̖��O
*/
void ShaderManager::Add(const ShaderPtr& shader, string tecName)
{
	// �p�����[�^�̐ݒ�
	Vector3 ld = m_lightAt - m_lightPos;
	shader->SetValue("LightDir", &ld);
	shader->SetValue("Diffuse", &m_diffuse);
	shader->SetValue("Ambient", &m_ambient);
	shader->SetValue("Specular", &m_specular);
	shader->SetValue("Emmisive", &m_emmisive);
//	shader->SetValue("EyePos", &camera->GetPos());

	// �e�N�j�b�N�̐ݒ�
	shader->GetEffect()->SetTechnique(tecName.c_str());

	// �V���h�E�}�b�v�p -------------------------------------
	shader->SetValue("LV", &GetLightView());
	shader->SetValue("LP", &GetLightProjection());

	shader->SetValue("ShadowMapTexture", m_depthBuffer->GetTexture());

	// �����܂�----------------------------------------------

	// �쐬�����V�F�[�_�[�����X�g�ɓo�^
	m_shaderList.emplace_back(shader);
}

/**
* @brief �쐬
*/
HRESULT ShaderManager::Create()
{
	// �X�^���_�[�h�V�F�[�_�[�̍쐬
	m_standard = make_shared<Shader>("Data/Shader/StandardShader.fx");

	// �[�x�o�b�t�@�̍쐬
	m_depthBuffer = make_shared<DepthBuffer>();

	// �u���[�t�B���^�[�̍쐬
	m_blurFilter = make_shared<BlurFilter>();

	// �V���h�E�}�b�v�V�F�[�_�[�����X�g�ɓo�^
	Add(m_depthBuffer->GetShadowMapShader(), "ZShadowMap");
	//	Add(m_depthBuffer->GetSkinShadowMapShader());
	// �u���[�t�B���^�[�����X�g�ɓo�^
	Add(m_blurFilter->GetShader(), "BlurFilter");

	// ����
	return S_OK;
}

/**
* @brief ������
*/
HRESULT ShaderManager::Initialize()
{
	m_lightPos = Vector3(0.0f, 11.8f, 20.4f);
	m_lightAt = Vector3(0.0f, 0.0f, 7.0f);
	m_ambient = Vector4(0.8f, 0.8f, 0.8f, 0.8f);
	m_diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_emmisive = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	
	// ����
	return S_OK;
}

/**
* @brief �X�V
* @param matWorld - ���f���̃��[���h�ϊ��s��
* @param camera - �J�����N���X
*/
void ShaderManager::Update(const Matrix& world, const CameraPtr& camera)
{
	// ���X�g�ɓo�^����Ă��鐔�������[�v
	for (auto& it : m_shaderList)
	{
		// �p�����[�^�̐ݒ�
		it->SetValue("W", &world);
		it->SetValue("V", &camera->GetView());
		it->SetValue("P", &camera->GetProj());

		// ���C�g�̌���
		Vector3 ld = m_lightAt - m_lightPos;

		it->SetValue("LightDir", &ld);
		it->SetValue("Diffuse", &m_diffuse);
		it->SetValue("Ambient", &m_ambient);
		it->SetValue("Specular", &m_specular);
		it->SetValue("Emmisive", &m_emmisive);
		it->SetValue("EyePos", &camera->GetPos());

		//�V���h�E�}�b�v�p
		it->SetValue("LV", &GetLightView());
		it->SetValue("LP", &GetLightProjection());
		it->SetValue("ShadowMapTexture", m_depthBuffer->GetTexture());
	}

	// ���C�g�̍��W���������Y����(�f�o�b�O�p)
	m_lightPos.z -= 0.001f;
}

/**
* @brief �[�x�o�b�t�@�����_�����O�J�n
*/
void ShaderManager::BeginDepth()
{
	m_depthBuffer->Begin();
}

/**
* @brief �[�x�o�b�t�@�����_�����O�I��
*/
void ShaderManager::EndDepth()
{
	m_depthBuffer->End();
}

/**
* @brief �u���[�J�n
*/
void ShaderManager::BeginBlur()
{
	m_blurFilter->Begin();
}

/**
* @brief �u���[�I��
*/
void ShaderManager::EndBlur()
{
	m_blurFilter->End();
}

/**
* @brief �K�E�V�A���t�B���^�[�̏d�݂��v�Z����
*/
void ShaderManager::GaussWeight(float dispersion)
{
	m_blurFilter->GaussWeight(dispersion);
}

/**
* @brief ���C�g�̍��W���擾����
*/
const Vector3& ShaderManager::GetLightPos()
{
	return m_lightPos;
}

/**
* @brief ���C�g�̒����_���擾����
*/
const Vector3& ShaderManager::GetLightAt()
{
	return m_lightAt;
}

/**
* @brief ���C�g�̃r���[�s����擾����
*/
const Matrix& ShaderManager::GetLightView()
{
	LookAtLH(m_lightView, m_lightPos, m_lightAt, Vector3(0.0f, 1.0f, 0.0f));
	return m_lightView;
}

/**
* @brief ���C�g�̓��e�s����擾����
*/
const Matrix& ShaderManager::GetLightProjection()
{
	OlthoLH(m_lightProj,
		static_cast<float>(AppIns->GetWindowWidth()),
		static_cast<float>(AppIns->GetWindowHeight()),
		0.1f, 200.0f);

	return m_lightProj;
}

/**
* @brief �V�F�[�_�[�p�̃e�N�X�`�����擾����
*/
const LPDIRECT3DTEXTURE9& ShaderManager::GetShaderTexture(TexDataType index) const
{
	return m_blurFilter->GetTexture(index);
}

/**
* @brief �V���h�E�}�b�v�e�N�X�`�����擾����
*/
const LPDIRECT3DTEXTURE9& ShaderManager::GetShadowMapTexture() const
{
	return m_depthBuffer->GetTexture();
}

/**
* @brief �X�^���_�[�h�V�F�[�_�[���擾����
*/
const ShaderPtr& ShaderManager::GetStandardShader() const
{
	return m_standard;
}

/**
* @brief �V���h�E�}�b�v�V�F�[�_�[���擾����
*/
const ShaderPtr& ShaderManager::GetShadowMapShader() const
{
	return m_depthBuffer->GetShadowMapShader();
}

/**
* @brief �X�L�����b�V���p�V���h�E�}�b�v�V�F�[�_�[���擾����
*/
const ShaderPtr& ShaderManager::GetSkinShadowMapShader() const
{
	return m_depthBuffer->GetSkinShadowMapShader();
}