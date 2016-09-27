#include "Direct3DSystem.h"
#include "Direct3DData.h"
#include "..\ApplicationSystem\ApplicationData.h"

/**
* @brief �R���X�g���N�^
*/
Direct3DSystem::Direct3DSystem()
{
	m_pD3d = nullptr;
	m_pDevice = nullptr;
}

/**
* @brief �f�X�g���N�^
*/
Direct3DSystem::~Direct3DSystem()
{
	SafeRelease(m_pDevice);
	SafeRelease(m_pD3d);
}

/**
* @brief �쐬
* @param �E�B���h�E�n���h��
*/
HRESULT Direct3DSystem::Create(HWND hWnd)
{
	// �uDirect3D�v�I�u�W�F�N�g�̍쐬
	if (nullptr == (m_pD3d = Direct3DCreate9(D3D_SDK_VERSION))){
		MessageBox(0, "Direct3D�̍쐬�Ɏ��s���܂���", "", MB_OK);
		return E_FAIL;
	}

	// �f�o�C�X�p�����[�^���쐬
	if (!CreateParam()) return E_FAIL;

	// �f�o�C�X�̍쐬
	if (!CreateDevice(hWnd)) return E_FAIL;

	// �f�o�C�X���Z�b�g
	D3DIns->SetDevice(m_pDevice);

	/// �f�o�C�X�I�u�W�F�N�g���Z�b�g
	D3DIns->SetD3dpp(m_d3dpp);

	// ����
	return S_OK;
}

/**
* @brief �f�o�C�X�p�����[�^�̍쐬
*/
BOOL Direct3DSystem::CreateParam()
{
	// �uDIRECT3D�f�o�C�X�v�I�u�W�F�N�g�̍쐬
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
	m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.Windowed = NULL;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// �E�B���h�E���[�h
	if (AppIns->GetWindowed()){
		m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_d3dpp.Windowed = TRUE;
	}
	// �t���X�N���[�����[�h
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

	// ����
	return TRUE;
}

/**
* @brief �f�o�C�X�̍쐬
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
			MessageBox(0, "HAL���[�h��DIRECT3D�f�o�C�X���쐬�ł��܂���\nREF���[�h�ōĎ��s���܂�", NULL, MB_OK);
			if (FAILED(m_pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&m_d3dpp, &m_pDevice)))
			{
				if (FAILED(m_pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&m_d3dpp, &m_pDevice)))
				{
					MessageBox(0, "DIRECT3D�f�o�C�X�̍쐬�Ɏ��s���܂���", NULL, MB_OK);
					return FALSE;
				}
			}
		}
	}

	// ����
	return TRUE;
}