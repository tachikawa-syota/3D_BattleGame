#include "WaveShader.h"
#include "..\Common\STDXStruct.h"
#include "..\ApplicationSystem\ApplicationData.h"
#include "..\DirectX\Direct3DData.h"

/**
* @brief �R���X�g���N�^
* @param width - �쐬����e�N�X�`���̏c��
* @param height - �쐬����e�N�X�`���̉���
*/
WaveShader::WaveShader(UINT width, UINT height)
{
	// �C���X�^���X���擾����
	m_2dTex = TextureManager::GetInstance();
	m_pDevice = D3DIns->GetDevice();

	// �V�F�[�_�[���쐬����
	m_shader = make_shared<Shader>("Data/Shader/Wave.fx");

	m_pTexture = new LPDIRECT3DTEXTURE9[wsMAX];
	m_pSurface = new LPDIRECT3DSURFACE9[wsMAX];

	for (int i = 0; i < wsMAX; i++) {
		m_pTexture[i] = NULL;
		m_pSurface[i] = NULL;
	}

	m_RenderTargetIndex = 0;

	if (FAILED(Create(width, height))){
		int a = 0;
	}
}

/**
* @brief �쐬
* @param width - �쐬����e�N�X�`���̏c��
* @param height - �쐬����e�N�X�`���̉���
*/
HRESULT WaveShader::Create(UINT width, UINT height)
{
	D3DCAPS9 caps;
	HRESULT hr;

	m_pDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion >= D3DVS_VERSION(1, 1) && caps.PixelShaderVersion >= D3DPS_VERSION(2, 0))
	{
		hr = m_2dTex->Create2DPolygon(width, height);
		if (FAILED(hr))
			return -1;

		Vector2 Size;
		Size.x = 1.0f / m_2dTex->Get2DPolygonWidth();
		Size.y = 1.0f / m_2dTex->Get2DPolygonHeight();
		m_shader->SetValue("TexOffset", (float*)&Size, sizeof(Vector2));

		// �|���S���̕����擾����
		UINT polyWidth = m_2dTex->Get2DPolygonWidth();
		UINT polyHeight = m_2dTex->Get2DPolygonHeight();

		for (int i = 0; i < 2; i++)
		{
			// �e�N�X�`���̍쐬
			m_texData[i] = m_2dTex->CreateTexture(polyWidth, polyHeight, D3DUSAGE_RENDERTARGET, D3DFMT_G16R16F);

			//�}�b�v�����ŏ�����
			LPDIRECT3DSURFACE9 OldSurface = NULL;
			m_pDevice->GetRenderTarget(0, &OldSurface);
			LPDIRECT3DSURFACE9 OldDepthStencilSurface = NULL;
			m_pDevice->GetDepthStencilSurface(&OldDepthStencilSurface);
			//�f�v�X�o�b�t�@�͎g�p���Ȃ��̂Ŗ����ɂ���
			m_pDevice->SetDepthStencilSurface(NULL);
			m_pDevice->SetRenderTarget(0, m_texData[i].pSurface);
			m_pDevice->Clear(0L, NULL, D3DCLEAR_TARGET, 0x0, 1.0f, 0L);
			m_pDevice->SetRenderTarget(0, OldSurface);
			SafeRelease(OldSurface);
			m_pDevice->SetDepthStencilSurface(OldDepthStencilSurface);
			SafeRelease(OldDepthStencilSurface);
		}

		// �e�N�X�`���̍쐬
		m_texData[wsBUMP] = m_2dTex->CreateTexture(polyWidth, polyHeight, D3DUSAGE_RENDERTARGET, D3DFMT_G16R16F);
	}
	else{
		// ���s
		return E_FAIL;
	}

	// ����
	return S_OK;
}

/**
* @brief �g��ǉ�����
*/
void WaveShader::AddWave(float posX, float posY, float height)
{
	// �G�t�F�N�g���Ȃ�������
	if (m_shader->GetEffect() == nullptr) return;

	//�g��ǉ�����ʒu��ݒ�
	Vector2 AddWavePos = Vector2(posX, posY);
	m_shader->SetValue("AddWavePos", &AddWavePos, sizeof(D3DXVECTOR2));
	//�ǉ�����g�̍�����ݒ肷��
	m_shader->SetValue("AddWaveHeight", height);
}

LPDIRECT3DTEXTURE9 WaveShader::Render()
{
	if (m_shader->GetEffect() == nullptr) return nullptr;

	m_shader->SetValue("SpringPower", 0.2f);

	//�K���ɔg��ǉ�����
	AddWave((float)(rand() % 100) * 0.01f, (float)(rand() % 100) * 0.01f, (float)(rand() % 100 - 50) * 0.02f);

	D3DVIEWPORT9 OldViewport, NewViewport;

	//�r���[�|�[�g��g�}�b�v�̃T�C�Y�ɍ��킹��
	m_pDevice->GetViewport(&OldViewport);
	CopyMemory(&NewViewport, &OldViewport, sizeof(D3DVIEWPORT9));
		
	NewViewport.Width = m_2dTex->Get2DPolygonWidth();
	NewViewport.Height = m_2dTex->Get2DPolygonHeight();
	m_pDevice->SetViewport(&NewViewport);

	//���݂̃����_�[�^�[�Q�b�g�T�[�t�F�C�X���擾
	LPDIRECT3DSURFACE9 OldSurface = NULL;
	m_pDevice->GetRenderTarget(0, &OldSurface);

	m_RenderTargetIndex = 1 - m_RenderTargetIndex;

	//�����_�[�^�[�Q�b�g���Z�b�g����
	m_pDevice->SetRenderTarget(0, m_texData[1 - m_RenderTargetIndex].pSurface);

	LPDIRECT3DSURFACE9 OldDepthStencilSurface = NULL;
	m_pDevice->GetDepthStencilSurface(&OldDepthStencilSurface);

	//�f�v�X�o�b�t�@���g�p���Ȃ��̂Ŗ����ɂ���
	m_pDevice->SetDepthStencilSurface(NULL);

	// �g�}�b�v���e�N�X�`���[�X�e�[�W�O�ɃZ�b�g����
	m_pDevice->SetTexture(0, m_texData[m_RenderTargetIndex].pTexture);

	// �g�}�b�v�̍X�V
	m_shader->Begine();
	m_shader->BeginePass(0);

	// 2D�|���S���̃����_�����O
	m_2dTex->Draw2DPolygon();

	m_shader->EndPass();

	// �X�V�����g�}�b�v���Q�Ƃ����ʂ�ʉ��ɂ��ă����_�����O����
	m_pDevice->SetRenderTarget(0, m_texData[wsBUMP].pSurface);

	m_pDevice->SetTexture(0, m_texData[1 - m_RenderTargetIndex].pTexture);

	m_shader->BeginePass(1);

	// 2D�|���S���̃����_�����O
	m_2dTex->Draw2DPolygon();

	m_shader->EndPass();
	m_shader->End();

	//�߂�
	m_pDevice->SetRenderTarget(0, OldSurface);
	SafeRelease(OldSurface);

	m_pDevice->SetDepthStencilSurface(OldDepthStencilSurface);
	SafeRelease(OldDepthStencilSurface);

	m_pDevice->SetViewport(&OldViewport);

	return m_texData[wsBUMP].pTexture;
}

/**
* @brief �V�F�[�_�[���擾����
*/
ShaderPtr& WaveShader::GetShader()
{
	return m_shader;
}