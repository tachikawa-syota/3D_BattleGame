#include "Direct3DData.h"

/**
* @brief D3DDevice�̐ݒ�
*/
void Direct3DData::SetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
}

/**
* @brief �f�o�C�X�I�u�W�F�N�g��ݒ肷��
*/
void Direct3DData::SetD3dpp(D3DPRESENT_PARAMETERS& dpp)
{
	m_pD3dpp = dpp;
}

/**
* @brief D3DDevice���擾����
*/
LPDIRECT3DDEVICE9 Direct3DData::GetDevice()
{
	return m_pDevice;
}

/**
* @brief �f�o�C�X�I�u�W�F�N�g���擾����
*/
D3DPRESENT_PARAMETERS& Direct3DData::GetD3dpp()
{
	return m_pD3dpp;
}