/**
* @file   BlurFilter.h
* @brief  �u���[������������
* @author Syota Tachikawa
*/

#ifndef ___BLURFILTER_H
#define ___BLURFILTER_H

#include "..\DirectX\TextureManager.h"
#include "Shader.h"

/**
* @brief �u���[�t�B���^�[�N���X
*/
class BlurFilter
{
public:
	/**
	* @enum TexDataType
	* @brief �V�F�[�_�[�p�̃e�N�X�`���̎��
	*/
	enum TexDataType{
		Color = 0,
		LightBloom,
		Blur1,
		Blur2,
		DataMax
	};

	/**
	* @brief �R���X�g���N�^
	*/
	BlurFilter();

	/**
	* @brief �f�X�g���N�^
	*/
	~BlurFilter();

	/**
	* @brief �u���[�J�n
	*/
	void Begin();

	/**
	* @brief �u���[�I��
	*/
	void End();

	/**
	* @brief �K�E�V�A���t�B���^�[�̏d�݂��v�Z����
	*/
	void GaussWeight(float dispersion);

	/**
	* @brief �u���[�p�ɍ쐬�����e�N�X�`�����擾����
	*/
	const LPDIRECT3DTEXTURE9& GetTexture(int index) const;

	/**
	* @brief �V�F�[�_�[���̎擾
	*/
	ShaderPtr& GetShader();

private:
	/// �u���[�t�B���^�[
	ShaderPtr m_shader;

	/// �e�N�X�`���}�l�[�W���[
	TextureManager* m_2dTex;

	// �e�N�X�`���f�[�^
	TEXTURE_DATA m_texData[DataMax];

	/// �O�̃T�[�t�F�C�X
	LPDIRECT3DSURFACE9 m_oldSurface;
};

using BlurFilterPtr = shared_ptr<BlurFilter>;

#endif