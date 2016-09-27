#include "Direct3DData.h"

/**
* @brief D3DDeviceの設定
*/
void Direct3DData::SetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
}

/**
* @brief デバイスオブジェクトを設定する
*/
void Direct3DData::SetD3dpp(D3DPRESENT_PARAMETERS& dpp)
{
	m_pD3dpp = dpp;
}

/**
* @brief D3DDeviceを取得する
*/
LPDIRECT3DDEVICE9 Direct3DData::GetDevice()
{
	return m_pDevice;
}

/**
* @brief デバイスオブジェクトを取得する
*/
D3DPRESENT_PARAMETERS& Direct3DData::GetD3dpp()
{
	return m_pD3dpp;
}