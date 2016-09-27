/**
* @file   Shader.h
* @brief  �V�F�[�_�[�V�X�e���̊Ǘ�
* @author Syota Tachikawa
*/

#ifndef ___SHADER_H
#define ___SHADER_H

#include "..\Math\Math.h"

/**
* @brief �V�F�[�_�[�N���X
*/
class Shader
{
public:
	/**
	* @brief �R���X�g���N�^
	* @param fx�t�@�C���̖��O
	*/
	Shader(string fileName);

	/**
	* @brief �f�X�g���N�^
	*/
	~Shader();

	/**
	* @brief �G�t�F�N�g�̊J�n
	*/
	UINT Begine();

	/**
	* @brief �G�t�F�N�g�̏I��
	*/
	void End();

	/**
	* @brief �p�X�J�n
	*/
	void BeginePass(int pass);

	/**
	* @brief �p�X�J�n
	*/
	void EndPass();

	/**
	* @brief �X�e�C�g�̕ω����f�o�C�X�ɓ`����
	*/
	void CommitChanges();

	/**
	* @brief �G�t�F�N�g���擾����
	*/
	LPD3DXEFFECT GetEffect(){ return m_pShader; }

	/**
	* @brief �p�����[�^�[���擾����
	*/
	D3DXHANDLE GetParameter(D3DXHANDLE paramName){ return m_pShader->GetParameterByName(NULL, paramName); }

	/**
	* @brief ���C���e�N�X�`���̐ݒ�
	*/
	void SetTexture(IDirect3DTexture9* tex);
	/**
	* @brief ���C���e�N�X�`���̐ݒ�
	*/
	void SetTexture(IDirect3DTexture9& tex);

	void SetMatrix(const Matrix* mat);
	void SetMatrix(const Matrix& mat);

	void SetValue(string name, IDirect3DTexture9* tex);
	void SetValue(string name, IDirect3DTexture9& tex);

	void SetValue(string name, const Matrix* mat);
	void SetValue(string name, const Matrix& mat);

	void SetValue(string name, const D3DXVECTOR4* v);
	void SetValue(string name, const D3DXVECTOR4& v);

	void SetValue(string name, const Vector3* v);
	void SetValue(string name, const Vector3& v);
	void SetValue(string name, float* pf, int count);

	void SetValue(string name, float f);
	void SetValue(string name, int d);
	void SetValue(string name, DWORD d);
	void SetValue(string name, const void* v, byte b);

	void SetBool(string name, bool bFlag);

private:
	/// �V�F�[�_�[
	LPD3DXEFFECT m_pShader;

	/// �ˉe�ϊ��s��
	D3DXHANDLE m_hmWVP;

	/// �e�N�X�`��
	D3DXHANDLE m_htexDecale;
};

/// �V�F�[�_�[�̃X�}�[�g�|�C���^
using ShaderPtr = shared_ptr<Shader>;

#endif