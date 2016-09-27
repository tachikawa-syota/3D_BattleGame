/**
* @file Direct3DSystem.h
* @brief Direct3Dシステムの管理
* @author Syota Tachikawa
*/

#ifndef ___DIRECT3DSYSTEM_H
#define ___DIRECT3DSYSTEM_H

#include "..\Common\CommonLib.h"

/**
* @brief Direct3Dシステムクラス
*/
class Direct3DSystem
{
public:
	/**
	* @brief コンストラクタ
	*/
	Direct3DSystem();
	
	/**
	* @brief デストラクタ
	*/
	~Direct3DSystem();

	/**
	* @brief 作成
	* @param ウィンドウハンドル
	*/
	HRESULT Create(HWND hWnd);

	/**
	* @brief デバイスパラメータの作成
	*/
	BOOL CreateParam();

	/**
	* @brief デバイスの作成
	*/
	BOOL CreateDevice(HWND hWnd);

private:
	// インターフェイス
	LPDIRECT3D9			m_pD3d;

	LPDIRECT3DDEVICE9	m_pDevice;

	/// デバイスパラメータ
	D3DPRESENT_PARAMETERS m_d3dpp;
};

#endif