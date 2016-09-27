#include "BlurFilter.h"
#include "..\DirectX\Direct3DData.h"
#include "..\ApplicationSystem\ApplicationData.h"

/**
* @brief �R���X�g���N�^
*/
BlurFilter::BlurFilter()
{
	// �V�F�[�_�[�쐬
	m_shader = make_shared<Shader>("Data/Shader/BlurFilter.fx");

	// �C���X�^���X���擾
	m_2dTex = TextureManager::GetInstance();

	UINT winWidth = AppIns->GetWindowWidth();
	UINT winHeight = AppIns->GetWindowHeight();

	// �V�[���̃����_�����O���ʂ��i�[����T�[�t�F�C�X
	m_texData[Color] = m_2dTex->CreateTexture(winWidth, winHeight);

	// �V�[���̋P�x�̂��ӂ�����i�[����T�[�t�F�C�X
	m_texData[LightBloom] = m_2dTex->CreateTexture(winWidth, winHeight);

	// �ڂ������߂̃T�[�t�F�C�X
	for (int i = 0; i < 2; i++){
		m_texData[i + Blur1] = m_2dTex->CreateTexture(winWidth, winHeight);
	}
}

/**
* @brief �f�X�g���N�^
*/
BlurFilter::~BlurFilter()
{

}

/**
* @brief �u���[�J�n
*/
void BlurFilter::Begin()
{
	// �f�o�C�X���擾����
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();

	// �}���`�����_�[�^�[�Q�b�g�ɐ؂�ւ���B
	// �}���`�����_�[�^�[�Q�b�g�T�[�t�F�C�X�͈ꕔ�̎����ł͂��ׂẴT�[�t�F�C�X���A�t�H�[�}�b�g�͈قȂ��Ă��A�r�b�g�[�x�͓����łȂ���΂Ȃ�Ȃ��ꍇ������B
	// �o�b�N�o�b�t�@�̃J���[�r�b�g�[�x�͊��ɂ���ĕς�邽�߁A
	// �o�b�N�o�b�t�@�ƃ����_�[�^�[�Q�b�g�T�[�t�F�C�X���}���`�����_�[�^�[�Q�b�g�Ƃ��Ĉꏏ�Ɏg�p���Ȃ��悤�ɂ���B
	m_oldSurface = NULL;
	pDevice->GetRenderTarget(0, &m_oldSurface);
	pDevice->SetRenderTarget(0, m_texData[Color].pSurface);
	pDevice->SetRenderTarget(1, m_texData[LightBloom].pSurface);

	// �^�[�Q�b�g�o�b�t�@�̃N���A�A�y�o�b�t�@�̃N���A
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0);
}

/**
* @brief �u���[�I��
*/
void BlurFilter::End()
{
	// �f�o�C�X���擾����
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();

	// FVF��D3DFVF_XYZRHW��ݒ肵��2D�I�u�W�F�N�g�Ń����_�����O����̂�Z�o�b�t�@���g�p���Ȃ�
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

	// 1 / 4 �T�C�Y�̃T�[�t�F�C�X���g�p���ău���[�������s���̂Ńr���[�|�[�g��؂�ւ���
	D3DVIEWPORT9 OldViewport, NewViewport;
	pDevice->GetViewport(&OldViewport);
	NewViewport.Width = AppIns->GetWindowWidth();
	NewViewport.Height = AppIns->GetWindowHeight();
	NewViewport.MaxZ = 1.0f;
	NewViewport.MinZ = 0.0f;
	NewViewport.X = 0;
	NewViewport.Y = 0;
	pDevice->SetViewport(&NewViewport);

	// �K�E�V�A���t�B���^�[�̏d�݂��v�Z����
	GaussWeight(15 * 15);

	m_shader->SetValue("MapWidth", static_cast<float>(AppIns->GetWindowWidth()));
	m_shader->SetValue("MapHeight", static_cast<float>(AppIns->GetWindowHeight()));
	m_shader->Begine();
	// �����_�[�^�[�Q�b�g���Z�b�g����
	pDevice->SetRenderTarget(0, m_texData[Blur1].pSurface);
	// X�����Ƀu���[�t�B���^�[��K������
	m_shader->SetBool("isXY", true);
	m_shader->BeginePass(0);
	m_shader->SetValue("SrcTexture", m_texData[LightBloom].pTexture);
	m_shader->CommitChanges();
	m_2dTex->DrawTexture(m_texData[LightBloom].pTexture);
	m_shader->EndPass();

	// �����_�[�^�[�Q�b�g���Z�b�g����
	pDevice->SetRenderTarget(0, m_texData[Blur2].pSurface);
	// �o�b�N�o�b�t�@���e�N�X�`���[�X�e�[�W�O�ɃZ�b�g����
	// Y�����Ƀu���[�t�B���^�[��K������
	m_shader->SetBool("isXY", false);
	m_shader->BeginePass(0);
	m_shader->SetValue("SrcTexture", m_texData[Blur1].pTexture);
	m_shader->CommitChanges();									// �ύX���m��
	m_2dTex->DrawTexture(m_texData[Blur1].pTexture);
	m_shader->EndPass();
	m_shader->End();

	// �r���[�|�[�g��߂�
	pDevice->SetViewport(&OldViewport);

	// �����_�[�^�[�Q�b�g���o�b�N�o�b�t�@�ɖ߂�
	pDevice->SetRenderTarget(0, m_oldSurface);
	SafeRelease(m_oldSurface);

	// �J���[�����o�b�N�o�b�t�@�Ƀ����_�����O����
	m_2dTex->DrawTexture(m_texData[Color].pTexture);

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//���Z����
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	m_2dTex->DrawTexture(m_texData[Blur2].pTexture);

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

/**
* @brief �K�E�V�A���t�B���^�[�̏d�݂��v�Z����
*/
void BlurFilter::GaussWeight(float dispersion)
{
	DWORD i;
	// �d�݂̔z��
	float array[8];
	float total = 0;

	for (i = 0; i<8; i++){
		array[i] = expf(-0.5f*(FLOAT)(i*i) / dispersion);
		if (0 == i) {
			total += array[i];
		}
		else {
			// ���S�ȊO�́A�Q�񓯂��W�����g���̂łQ�{
			total += 2.0f * array[i];
		}
	}
	// �K�i��
	for (i = 0; i < 8; i++) {
		array[i] /= total;
	}
	if (m_shader) m_shader->SetValue("weight", array, 8);
}

/**
* @brief �u���[�p�ɍ쐬�����e�N�X�`�����擾����
*/
const LPDIRECT3DTEXTURE9& BlurFilter::GetTexture(int index) const
{
	return m_texData[index].pTexture;
}

/**
* @brief �V�F�[�_�[���̎擾
*/
ShaderPtr& BlurFilter::GetShader()
{
	return m_shader;
}