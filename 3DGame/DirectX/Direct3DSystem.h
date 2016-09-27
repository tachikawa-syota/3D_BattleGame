/**
* @file Direct3DSystem.h
* @brief Direct3D�V�X�e���̊Ǘ�
* @author Syota Tachikawa
*/

#ifndef ___DIRECT3DSYSTEM_H
#define ___DIRECT3DSYSTEM_H

#include "..\Common\CommonLib.h"

/**
* @brief Direct3D�V�X�e���N���X
*/
class Direct3DSystem
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	Direct3DSystem();
	
	/**
	* @brief �f�X�g���N�^
	*/
	~Direct3DSystem();

	/**
	* @brief �쐬
	* @param �E�B���h�E�n���h��
	*/
	HRESULT Create(HWND hWnd);

	/**
	* @brief �f�o�C�X�p�����[�^�̍쐬
	*/
	BOOL CreateParam();

	/**
	* @brief �f�o�C�X�̍쐬
	*/
	BOOL CreateDevice(HWND hWnd);

private:
	// �C���^�[�t�F�C�X
	LPDIRECT3D9			m_pD3d;

	LPDIRECT3DDEVICE9	m_pDevice;

	/// �f�o�C�X�p�����[�^
	D3DPRESENT_PARAMETERS m_d3dpp;
};

#endif