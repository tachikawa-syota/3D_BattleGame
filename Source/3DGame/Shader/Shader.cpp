#include "Shader.h"
#include "..\DirectX\Direct3DData.h"

// �x���}��
#pragma warning (disable:4996)

/**
* @brief �R���X�g���N�^
* @param fx�t�@�C���̖��O
*/
Shader::Shader(string fileName)
{
	// �f�o�C�X���擾����
	LPDIRECT3DDEVICE9 device = D3DIns->GetDevice();

	// �V�F�[�_�̓ǂݍ���
	HRESULT	hr;
	LPD3DXBUFFER pErr = NULL;

	// fx�t�@�C���̃G���[�`�F�b�N
	hr = D3DXCreateEffectFromFile(device, fileName.c_str(), NULL, NULL, 0, NULL, &m_pShader, &pErr);
	if (FAILED(hr))
	{
		char szBuffer[2000];
		sprintf(szBuffer, "\terrors: %s\n", (char*)pErr->GetBufferPointer());
		OutputDebugString(szBuffer);

		MessageBox(nullptr, (char*)pErr->GetBufferPointer(), (char*)pErr->GetBufferPointer(), MB_OK);
		return;
	}

	//	��{�ϐ��̓ǂݍ���
	m_hmWVP = m_pShader->GetParameterByName(NULL, "WVP");
	m_htexDecale = m_pShader->GetParameterByName(NULL, "MainTexture");
}

/**
* @brief �f�X�g���N�^
*/
Shader::~Shader()
{
	m_pShader->Release();
}

/**
* @brief �G�t�F�N�g�̊J�n
*/
UINT Shader::Begine()
{
	UINT pass;
	m_pShader->Begin(&pass, 0);
	return pass;
}

/**
* @brief �G�t�F�N�g�̏I��
*/
void Shader::End(){ m_pShader->End(); }

/**
* @brief �p�X�J�n
*/
void Shader::BeginePass(int pass){ m_pShader->BeginPass(pass); }

/**
* @brief �p�X�J�n
*/
void Shader::EndPass(){ m_pShader->EndPass(); }

/**
* @brief �X�e�C�g�̕ω����f�o�C�X�ɓ`����
*/
void Shader::CommitChanges(){ m_pShader->CommitChanges(); }

//*****************************************************************************
//	�p�����[�^�ݒ�
//*****************************************************************************

void Shader::SetMatrix(const Matrix* mat){ m_pShader->SetMatrix(m_hmWVP, mat); }
void Shader::SetMatrix(const Matrix& mat){ m_pShader->SetMatrix(m_hmWVP, &mat); }

void Shader::SetTexture(IDirect3DTexture9* tex){ m_pShader->SetTexture(m_htexDecale, tex); }
void Shader::SetTexture(IDirect3DTexture9& tex){ m_pShader->SetTexture(m_htexDecale, &tex); }

void Shader::SetValue(string name, IDirect3DTexture9* tex){ m_pShader->SetTexture(name.c_str(), tex); }
void Shader::SetValue(string name, IDirect3DTexture9& tex){ m_pShader->SetTexture(name.c_str(), &tex); }

void Shader::SetValue(string name, const Matrix* mat){ m_pShader->SetMatrix(name.c_str(), mat); }
void Shader::SetValue(string name, const Matrix& mat){ m_pShader->SetMatrix(name.c_str(), &mat); }

void Shader::SetValue(string name, const Vector4* v){ m_pShader->SetVector(name.c_str(), v); }
void Shader::SetValue(string name, const Vector4& v){ m_pShader->SetVector(name.c_str(), &v); }

void Shader::SetValue(string name, const  Vector3* v){ m_pShader->SetFloatArray(name.c_str(), (float*)v, 3); }
void Shader::SetValue(string name, const Vector3& v){ m_pShader->SetFloatArray(name.c_str(), (float*)&v, 3); }
void Shader::SetValue(string name, float* pf, int count){ m_pShader->SetFloatArray(name.c_str(), pf, count); }

void Shader::SetValue(string name, float f){ m_pShader->SetFloat(name.c_str(), f); }
void Shader::SetValue(string name, int d){ m_pShader->SetInt(name.c_str(), d); }
void Shader::SetValue(string name, DWORD d){ m_pShader->SetValue(name.c_str(), &d, 4); }
void Shader::SetValue(string name, const void* v, byte b) { m_pShader->SetValue(name.c_str(), v, b); }

void Shader::SetBool(string name, bool bFlag){ m_pShader->SetBool(name.c_str(), bFlag); }