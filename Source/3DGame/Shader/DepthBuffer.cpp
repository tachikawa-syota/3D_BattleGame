#include "DepthBuffer.h"
#include "..\DirectX\Direct3DData.h"
#include "..\ApplicationSystem\ApplicationData.h"

/**
* @brief �R���X�g���N�^
*/
DepthBuffer::DepthBuffer()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();
	TextureManager* tex = TextureManager::GetInstance();

	// �E�B���h�E�̃T�C�Y���擾����
	DWORD width = AppIns->GetWindowWidth();
	DWORD height = AppIns->GetWindowHeight();

	// Z�l�e�N�X�`�����쐬�ƃT�[�t�F�C�X��ێ�
	m_texData = tex->CreateTexture(width, height);

	// �e�N�X�`���������_�����O�^�[�Q�b�g�ɂ���ۂ̂y�o�b�t�@���쐬
	D3DSURFACE_DESC desc;

	m_texData.pTexture->GetLevelDesc(0, &desc);

	// �[�x�o�b�t�@�T�[�t�F�C�X�̍쐬
	pDevice->CreateDepthStencilSurface(
		desc.Width,
		desc.Height,
		D3DFMT_D32,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pDepthSurf,
		NULL);

	// �r���[�|�[�g�̐ݒ�
	m_viewport.X = 0;
	m_viewport.Y = 0;
	m_viewport.Width = width;
	m_viewport.Height = height;
	m_viewport.MaxZ = 1.0f;
	m_viewport.MinZ = 0.0f;

	//m_fOffsetX = 0.5f + (0.5f / static_cast<float>(width));
	//m_fOffsetY = 0.5f + (0.5f / static_cast<float>(height));

	//m_scaleBias = Matrix(0.5f, 0.0f, 0.0f, 0.0f,
	//	0.0f, -0.5f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 0.0f, 0.0f,
	//	m_fOffsetX, m_fOffsetY, 0.0f, 1.0f);

	//m_shadowMap->GetEffect()->

	// �V���h�E�}�b�v�V�F�[�_�[�̍쐬
	m_shadowMap = make_shared<Shader>("Data/Shader/ZShadowMap.fx");
	// �X�L�����b�V���V���h�E�}�b�v�V�F�[�_�[�̍쐬
//	m_skinShadowMap = make_shared<Shader>("Data/Shader/ZSkinShadowMap.fx");
}

/**
* @brief �f�X�g���N�^
*/
DepthBuffer::~DepthBuffer()
{

}

/**
* @brief ���݂̐ݒ��ۑ�����
*/
void DepthBuffer::SaveRenderTarget()
{
	// �f�o�C�X���擾����
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();

	pDevice->GetRenderTarget(0, &m_pDevBuf);
	pDevice->GetDepthStencilSurface(&m_pDevDepthSur);
	// ���݂̃r���[�|�[�g��ۑ�
	pDevice->GetViewport(&m_oldViewport);
}

/**
* @brief �[�x�o�b�t�@�����_�����O�J�n
*/
void DepthBuffer::Begin()
{
	// �f�o�C�X���擾����
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();

	// �����_�[�^�[�Q�b�g�̕ۑ�
	SaveRenderTarget();

	// �����_�����O�^�[�Q�b�g�̕ύX
	pDevice->SetRenderTarget(0, m_texData.pSurface);
	pDevice->SetDepthStencilSurface(m_pDepthSurf);
	// �r���[�|�[�g�̕ύX
	pDevice->SetViewport(&m_viewport);

	// �V���h�E�e�N�X�`���̃N���A
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
}

/**
* @brief �[�x�o�b�t�@�����_�����O�I��
*/
void DepthBuffer::End()
{
	// �f�o�C�X���擾����
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();

	// �ݒ�����ɖ߂�
	pDevice->SetRenderTarget(0, m_pDevBuf);
	pDevice->SetDepthStencilSurface(m_pDevDepthSur);
	pDevice->SetViewport(&m_oldViewport);

	SafeRelease(m_pDevBuf);
	SafeRelease(m_pDevDepthSur);
	pDevice->SetVertexShader(NULL);
	pDevice->SetPixelShader(NULL);
}

/**
* @brief �V���h�E�}�b�v�V�F�[�_�[�̎擾
*/
ShaderPtr& DepthBuffer::GetShadowMapShader()
{
	return m_shadowMap;
}

/**
* @brief �X�L�����b�V���p�V���h�E�}�b�v�V�F�[�_�[�̎擾
*/
ShaderPtr& DepthBuffer::GetSkinShadowMapShader()
{
	return m_skinShadowMap;
}

/**
* @brief �[�x�e�N�X�`�����擾����
*/
const LPDIRECT3DTEXTURE9& DepthBuffer::GetTexture() const
{
	return m_texData.pTexture;
}