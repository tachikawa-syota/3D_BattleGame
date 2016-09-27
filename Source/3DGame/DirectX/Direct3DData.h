/**
* @file Direct3DData.h
* @brief Direct3D�V�X�e���f�[�^�̊Ǘ�
* @author Syota Tachikawa
*/

#ifndef ___DIRECT3DDATA_H
#define ___DIRECT3DDATA_H

#include "..\Common\CommonLib.h"

/// �C���X�^���X���擾
#define D3DIns Direct3DData::GetInstance()

/**
* @brief Direct3D�̃f�[�^���Ǘ�����N���X
*/
class Direct3DData
{
public:
	/**
	* @brief �Q�b�g�C���X�^���X
	*/
	static Direct3DData* GetInstance()
	{
		static Direct3DData singlton;
		return &singlton;
	}

	/**
	* @brief D3DDevice�̐ݒ肷��
	*/
	void SetDevice(LPDIRECT3DDEVICE9 pDevice);

	/**
	* @brief �f�o�C�X�I�u�W�F�N�g��ݒ肷��
	*/
	void SetD3dpp(D3DPRESENT_PARAMETERS&);

	/**
	* @brief D3DDevice���擾����
	*/
	LPDIRECT3DDEVICE9 GetDevice();

	/**
	* @brief �f�o�C�X�I�u�W�F�N�g���擾����
	*/
	D3DPRESENT_PARAMETERS& GetD3dpp();

private:
	/// D3DDevice�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 m_pDevice;

	/// �f�o�C�X�I�u�W�F�N�g
	D3DPRESENT_PARAMETERS m_pD3dpp;
};

#endif