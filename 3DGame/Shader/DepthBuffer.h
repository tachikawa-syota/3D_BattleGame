/**
* @file   DepthBuffer.h
* @brief  �[�x�o�b�t�@
* @author Syota Tachikawa
*/

#ifndef ___DEPTHBUFFER_H
#define ___DEPTHBUFFER_H

#include "..\DirectX\TextureManager.h"
#include "Shader.h"

/**
* @brief �[�x�o�b�t�@�N���X
*/
class DepthBuffer
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	DepthBuffer();

	/**
	* @brief �f�X�g���N�^
	*/
	~DepthBuffer();

	/**
	* @brief �[�x�o�b�t�@�����_�����O�J�n
	*/
	void Begin();

	/**
	* @brief �[�x�o�b�t�@�����_�����O�I��
	*/
	void End();

	/**
	* @brief �V���h�E�}�b�v�V�F�[�_�[�̎擾
	*/
	ShaderPtr& GetShadowMapShader();

	/**
	* @brief �X�L�����b�V���p�V���h�E�}�b�v�V�F�[�_�[�̎擾
	*/
	ShaderPtr& GetSkinShadowMapShader();

	/**
	* @brief �[�x�e�N�X�`�����擾����
	*/
	const LPDIRECT3DTEXTURE9& GetTexture() const;

private:
	/// �ʏ탁�b�V���p�V�F�[�_�[
	ShaderPtr m_shadowMap;

	/// �X�L�����b�V���p�V�F�[�_�[
	ShaderPtr m_skinShadowMap;

	/// �[�x�o�b�t�@�T�[�t�F�C�X
	LPDIRECT3DSURFACE9 m_pDepthSurf;

	/// �f�o�C�X�o�b�N�o�b�t�@
	LPDIRECT3DSURFACE9 m_pDevBuf;

	/// �f�o�C�X�[�x�o�b�t�@
	LPDIRECT3DSURFACE9 m_pDevDepthSur;

	/// �e�N�X�`���f�[�^
	TEXTURE_DATA m_texData;

	/// �r���[�|�[�g
	D3DVIEWPORT9 m_viewport;

	/// �O�̃r���[�|�[�g
	D3DVIEWPORT9 m_oldViewport;

	/// �V���h�[�}�b�v�p�̃e�N�X�`���̃I�t�Z�b�g�s��
	float m_fOffsetX;
	float m_fOffsetY;
	Matrix m_scaleBias;

	/**
	* @brief ���݂̐ݒ��ۑ�����
	*/
	void SaveRenderTarget();
};

/// �[�x�o�b�t�@�̃X�}�[�g�|�C���^
using DepthBufferPtr = shared_ptr<DepthBuffer>;

#endif