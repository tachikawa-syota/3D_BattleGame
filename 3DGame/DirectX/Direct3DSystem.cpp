#include "Direct3DSystem.h"
#include "Direct3DData.h"
#include "..\ApplicationSystem\ApplicationData.h"

/**
* @brief コンストラクタ
*/
Direct3DSystem::Direct3DSystem()
{
	m_pD3d = nullptr;
	m_pDevice = nullptr;
}

/**
* @brief デストラクタ
*/
Direct3DSystem::~Direct3DSystem()
{
	SafeRelease(m_pDevice);
	SafeRelease(m_pD3d);
}

/**
* @brief 作成
* @param ウィンドウハンドル
*/
HRESULT Direct3DSystem::Create(HWND hWnd)
{
	// 「Direct3D」オブジェクトの作成
	if (nullptr == (m_pD3d = Direct3DCreate9(D3D_SDK_VERSION))){
		MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}

	// デバイスパラメータを作成
	if (!CreateParam()) return E_FAIL;

	// デバイスの作成
	if (!CreateDevice(hWnd)) return E_FAIL;

	// デバイスをセット
	D3DIns->SetDevice(m_pDevice);

	/// デバイスオブジェクトをセット
	D3DIns->SetD3dpp(m_d3dpp);

	// 成功
	return S_OK;
}

/**
* @brief デバイスパラメータの作成
*/
BOOL Direct3DSystem::CreateParam()
{
	// 「DIRECT3Dデバイス」オブジェクトの作成
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
	m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.Windowed = NULL;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// ウィンドウモード
	if (AppIns->GetWindowed()){
		m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_d3dpp.Windowed = TRUE;
	}
	// フルスクリーンモード
	else {
		D3DDISPLAYMODE DMode;
		m_pD3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DMode);
		m_d3dpp.BackBufferFormat = DMode.Format;
		m_d3dpp.BackBufferWidth = 1280;
		m_d3dpp.BackBufferHeight = 720;
		m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_d3dpp.Windowed = FALSE;
		m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	}

	// 成功
	return TRUE;
}

/**
* @brief デバイスの作成
*/
BOOL Direct3DSystem::CreateDevice(HWND hWnd)
{
	if (FAILED(m_pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_d3dpp, &m_pDevice)))
	{
		if (FAILED(m_pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp, &m_pDevice)))
		{
			MessageBox(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);
			if (FAILED(m_pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&m_d3dpp, &m_pDevice)))
			{
				if (FAILED(m_pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&m_d3dpp, &m_pDevice)))
				{
					MessageBox(0, "DIRECT3Dデバイスの作成に失敗しました", NULL, MB_OK);
					return FALSE;
				}
			}
		}
	}

	// 成功
	return TRUE;
}