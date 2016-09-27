/**
* @file   ShaderManager.h
* @brief  �V�F�[�_�[���Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___SHADERMANAGER_H
#define ___SHADERMANAGER_H

#include "..\Object\Camera.h"
#include "BlurFilter.h"
#include "DepthBuffer.h"

/**
* @brief �V�F�[�_�[�Ǘ��N���X
*/
class ShaderManager
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
	* @brief �C���X�^���X���擾����i�V���O���g���j
	*/
	static ShaderManager* GetInstance(){
		static ShaderManager singleton;
		return &singleton;
	}

	/**
	* @brief �V�F�[�_�[��ǉ�����
	* @param shader - �V�F�[�_�[
	* @param tecName - �e�N�j�b�N�̖��O
	*/
	void Add(const ShaderPtr& shader, string tecName);

	/**
	* @brief �쐬
	*/
	HRESULT Create();

	/**
	* @brief ������
	*/
	HRESULT Initialize();

	/**
	* @brief �X�V
	* @param world - ���[���h�s��
	* @param camera - �J�����N���X�̃C���X�^���X
	*/
	void Update(const Matrix& world, const CameraPtr& camera);

	/**
	* @brief �[�x�o�b�t�@�����_�����O�J�n
	*/
	void BeginDepth();

	/**
	* @brief �[�x�o�b�t�@�����_�����O�I��
	*/
	void EndDepth();

	/**
	* @brief �u���[�J�n
	*/
	void BeginBlur();

	/**
	* @brief �u���[�I��
	*/
	void EndBlur();

	/**
	* @brief �K�E�V�A���t�B���^�[�̏d�݂��v�Z����
	*/
	void GaussWeight(float dispersion);

	/**
	* @brief ���C�g�̍��W���擾����
	*/
	const Vector3& GetLightPos();

	/**
	* @brief ���C�g�̒����_���擾����
	*/
	const Vector3& GetLightAt();

	/**
	* @brief ���C�g�̃r���[�s����擾����
	*/
	const Matrix& GetLightView();

	/**
	* @brief ���C�g�̓��e�s����擾����
	*/
	const Matrix& GetLightProjection();

	/**
	* @brief �V�F�[�_�[�p�̃e�N�X�`�����擾����
	*/
	const LPDIRECT3DTEXTURE9& GetShaderTexture(TexDataType index) const;

	/**
	* @brief �V���h�E�}�b�v�e�N�X�`�����擾����
	*/
	const LPDIRECT3DTEXTURE9& GetShadowMapTexture() const;

	/**
	* @brief �X�^���_�[�h�V�F�[�_�[���擾����
	*/
	const ShaderPtr& GetStandardShader() const;

	/**
	* @brief �V���h�E�}�b�v�V�F�[�_�[���擾����
	*/
	const ShaderPtr& GetShadowMapShader() const;

	/**
	* @brief �X�L�����b�V���p�V���h�E�}�b�v�V�F�[�_�[���擾����
	*/
	const ShaderPtr& GetSkinShadowMapShader() const;

private:
	/// �X�^���_�[�h�V�F�[�_�[
	ShaderPtr m_standard;

	/// �u���[�t�B���^�[
	BlurFilterPtr m_blurFilter;

	/// �[�x�o�b�t�@
	DepthBufferPtr m_depthBuffer;

	/// �V�F�[�_�[���X�g
	list<ShaderPtr> m_shaderList;

	/// ���C�g�̃r���[�s��
	Matrix m_lightView;

	/// ���C�g�̓��e�s��
	Matrix m_lightProj;

	/// ���C�g�̍��W
	Vector3 m_lightPos;

	/// ���C�g�̒����_
	Vector3 m_lightAt;

	/// �A���r�G���g
	Vector4 m_ambient;

	/// �f�B�t���[�Y
	Vector4 m_diffuse;

	/// �X�y�L�����[
	Vector4 m_specular;

	/// �G�~�b�V�u
	Vector4 m_emmisive;
};

#endif