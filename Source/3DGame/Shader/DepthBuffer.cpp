#include "DepthBuffer.h"
#include "..\DirectX\Direct3DData.h"
#include "..\ApplicationSystem\ApplicationData.h"

/**
* @brief コンストラクタ
*/
DepthBuffer::DepthBuffer()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();
	TextureManager* tex = TextureManager::GetInstance();

	// ウィンドウのサイズを取得する
	DWORD width = AppIns->GetWindowWidth();
	DWORD height = AppIns->GetWindowHeight();

	// Z値テクスチャを作成とサーフェイスを保持
	m_texData = tex->CreateTexture(width, height);

	// テクスチャをレンダリングターゲットにする際のＺバッファを作成
	D3DSURFACE_DESC desc;

	m_texData.pTexture->GetLevelDesc(0, &desc);

	// 深度バッファサーフェイスの作成
	pDevice->CreateDepthStencilSurface(
		desc.Width,
		desc.Height,
		D3DFMT_D32,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pDepthSurf,
		NULL);

	// ビューポートの設定
	m_viewport.X = 0;
	m_viewport.Y = 0;
	m_viewport.Width = width;
	m_viewport.Height = height;
	m_viewport.MaxZ = 1.0f;
	m_viewport.MinZ = 0.0f;

	//m_fOffsetX = 0.5f + (0.5f / static_cast<float>(width));
	//m_fOffsetY = 0.5f + (0.5f / static_cast<float>(height));

	//m_scaleBias = Matrix(0.5f, 0.0f, 0.0f, 0.0f,
	//	0.0f, -0.5f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 0.0f, 0.0f,
	//	m_fOffsetX, m_fOffsetY, 0.0f, 1.0f);

	//m_shadowMap->GetEffect()->

	// シャドウマップシェーダーの作成
	m_shadowMap = make_shared<Shader>("Data/Shader/ZShadowMap.fx");
	// スキンメッシュシャドウマップシェーダーの作成
//	m_skinShadowMap = make_shared<Shader>("Data/Shader/ZSkinShadowMap.fx");
}

/**
* @brief デストラクタ
*/
DepthBuffer::~DepthBuffer()
{

}

/**
* @brief 現在の設定を保存する
*/
void DepthBuffer::SaveRenderTarget()
{
	// デバイスを取得する
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();

	pDevice->GetRenderTarget(0, &m_pDevBuf);
	pDevice->GetDepthStencilSurface(&m_pDevDepthSur);
	// 現在のビューポートを保存
	pDevice->GetViewport(&m_oldViewport);
}

/**
* @brief 深度バッファレンダリング開始
*/
void DepthBuffer::Begin()
{
	// デバイスを取得する
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();

	// レンダーターゲットの保存
	SaveRenderTarget();

	// レンダリングターゲットの変更
	pDevice->SetRenderTarget(0, m_texData.pSurface);
	pDevice->SetDepthStencilSurface(m_pDepthSurf);
	// ビューポートの変更
	pDevice->SetViewport(&m_viewport);

	// シャドウテクスチャのクリア
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
}

/**
* @brief 深度バッファレンダリング終了
*/
void DepthBuffer::End()
{
	// デバイスを取得する
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();

	// 設定を元に戻す
	pDevice->SetRenderTarget(0, m_pDevBuf);
	pDevice->SetDepthStencilSurface(m_pDevDepthSur);
	pDevice->SetViewport(&m_oldViewport);

	SafeRelease(m_pDevBuf);
	SafeRelease(m_pDevDepthSur);
	pDevice->SetVertexShader(NULL);
	pDevice->SetPixelShader(NULL);
}

/**
* @brief シャドウマップシェーダーの取得
*/
ShaderPtr& DepthBuffer::GetShadowMapShader()
{
	return m_shadowMap;
}

/**
* @brief スキンメッシュ用シャドウマップシェーダーの取得
*/
ShaderPtr& DepthBuffer::GetSkinShadowMapShader()
{
	return m_skinShadowMap;
}

/**
* @brief 深度テクスチャを取得する
*/
const LPDIRECT3DTEXTURE9& DepthBuffer::GetTexture() const
{
	return m_texData.pTexture;
}