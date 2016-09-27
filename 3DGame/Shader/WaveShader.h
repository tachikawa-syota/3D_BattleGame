/**
* @file   WaveShader.h
* @brief  �g�V�F�[�_�[�������Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___WAVESHADER_H
#define ___WAVESHADER_H

#include "..\Common\FileData.h"

enum WAVE_SURFACE
{
	wsMAIN01,
	wsMAIN02,
	wsBUMP,
	wsMAX,
};

/**
* @brief �g�V�F�[�_�[���Ǘ�����N���X
*/
class WaveShader
{
public:
	/**
	* @brief �R���X�g���N�^
	* @param width - �쐬����e�N�X�`���̏c��
	* @param height - �쐬����e�N�X�`���̉���
	*/
	WaveShader(UINT width, UINT height);

	/**
	* @brief �쐬
	* @param width - �쐬����e�N�X�`���̏c��
	* @param height - �쐬����e�N�X�`���̉���
	*/
	HRESULT Create(UINT width, UINT height);

	/**
	* @brief �g��ǉ�����
	*/
	void AddWave(float posX, float posY, float height);

	/**
	* @brief �`��
	*/
	LPDIRECT3DTEXTURE9 Render();

	/**
	* @brief �V�F�[�_�[���擾����
	*/
	ShaderPtr& GetShader();

private:
	/// �V�F�[�_�[
	ShaderPtr m_shader;

	/// �e�N�X�`���f�[�^
	TEXTURE_DATA m_texData[wsMAX];

	/// �f�o�C�X
	LPDIRECT3DDEVICE9 m_pDevice;

	/// �e�N�X�`��
	LPDIRECT3DTEXTURE9* m_pTexture;

	/// �T�[�t�F�C�X
	LPDIRECT3DSURFACE9* m_pSurface;

	/// �e�N�X�`���}�l�[�W���[
	TextureManager* m_2dTex;

	/// �g�}�b�v�̃C���f�b�N�X
	int m_RenderTargetIndex;
};

/// �g�V�F�[�_�[�̃X�}�[�g�|�C���^
using WaveShaderPtr = shared_ptr<WaveShader>;

#endif