#include "WaveShader.h"
#include "..\Common\STDXStruct.h"
#include "..\ApplicationSystem\ApplicationData.h"
#include "..\DirectX\Direct3DData.h"

/**
* @brief コンストラクタ
* @param width - 作成するテクスチャの縦幅
* @param height - 作成するテクスチャの横幅
*/
WaveShader::WaveShader(UINT width, UINT height)
{
	// インスタンスを取得する
	m_2dTex = TextureManager::GetInstance();
	m_pDevice = D3DIns->GetDevice();

	// シェーダーを作成する
	m_shader = make_shared<Shader>("Data/Shader/Wave.fx");

	m_pTexture = new LPDIRECT3DTEXTURE9[wsMAX];
	m_pSurface = new LPDIRECT3DSURFACE9[wsMAX];

	for (int i = 0; i < wsMAX; i++) {
		m_pTexture[i] = NULL;
		m_pSurface[i] = NULL;
	}

	m_RenderTargetIndex = 0;

	if (FAILED(Create(width, height))){
		int a = 0;
	}
}

/**
* @brief 作成
* @param width - 作成するテクスチャの縦幅
* @param height - 作成するテクスチャの横幅
*/
HRESULT WaveShader::Create(UINT width, UINT height)
{
	D3DCAPS9 caps;
	HRESULT hr;

	m_pDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion >= D3DVS_VERSION(1, 1) && caps.PixelShaderVersion >= D3DPS_VERSION(2, 0))
	{
		hr = m_2dTex->Create2DPolygon(width, height);
		if (FAILED(hr))
			return -1;

		Vector2 Size;
		Size.x = 1.0f / m_2dTex->Get2DPolygonWidth();
		Size.y = 1.0f / m_2dTex->Get2DPolygonHeight();
		m_shader->SetValue("TexOffset", (float*)&Size, sizeof(Vector2));

		// ポリゴンの幅を取得する
		UINT polyWidth = m_2dTex->Get2DPolygonWidth();
		UINT polyHeight = m_2dTex->Get2DPolygonHeight();

		for (int i = 0; i < 2; i++)
		{
			// テクスチャの作成
			m_texData[i] = m_2dTex->CreateTexture(polyWidth, polyHeight, D3DUSAGE_RENDERTARGET, D3DFMT_G16R16F);

			//マップを黒で初期化
			LPDIRECT3DSURFACE9 OldSurface = NULL;
			m_pDevice->GetRenderTarget(0, &OldSurface);
			LPDIRECT3DSURFACE9 OldDepthStencilSurface = NULL;
			m_pDevice->GetDepthStencilSurface(&OldDepthStencilSurface);
			//デプスバッファは使用しないので無効にする
			m_pDevice->SetDepthStencilSurface(NULL);
			m_pDevice->SetRenderTarget(0, m_texData[i].pSurface);
			m_pDevice->Clear(0L, NULL, D3DCLEAR_TARGET, 0x0, 1.0f, 0L);
			m_pDevice->SetRenderTarget(0, OldSurface);
			SafeRelease(OldSurface);
			m_pDevice->SetDepthStencilSurface(OldDepthStencilSurface);
			SafeRelease(OldDepthStencilSurface);
		}

		// テクスチャの作成
		m_texData[wsBUMP] = m_2dTex->CreateTexture(polyWidth, polyHeight, D3DUSAGE_RENDERTARGET, D3DFMT_G16R16F);
	}
	else{
		// 失敗
		return E_FAIL;
	}

	// 成功
	return S_OK;
}

/**
* @brief 波を追加する
*/
void WaveShader::AddWave(float posX, float posY, float height)
{
	// エフェクトがなかったら
	if (m_shader->GetEffect() == nullptr) return;

	//波を追加する位置を設定
	Vector2 AddWavePos = Vector2(posX, posY);
	m_shader->SetValue("AddWavePos", &AddWavePos, sizeof(D3DXVECTOR2));
	//追加する波の高さを設定する
	m_shader->SetValue("AddWaveHeight", height);
}

LPDIRECT3DTEXTURE9 WaveShader::Render()
{
	if (m_shader->GetEffect() == nullptr) return nullptr;

	m_shader->SetValue("SpringPower", 0.2f);

	//適当に波を追加する
	AddWave((float)(rand() % 100) * 0.01f, (float)(rand() % 100) * 0.01f, (float)(rand() % 100 - 50) * 0.02f);

	D3DVIEWPORT9 OldViewport, NewViewport;

	//ビューポートを波マップのサイズに合わせる
	m_pDevice->GetViewport(&OldViewport);
	CopyMemory(&NewViewport, &OldViewport, sizeof(D3DVIEWPORT9));
		
	NewViewport.Width = m_2dTex->Get2DPolygonWidth();
	NewViewport.Height = m_2dTex->Get2DPolygonHeight();
	m_pDevice->SetViewport(&NewViewport);

	//現在のレンダーターゲットサーフェイスを取得
	LPDIRECT3DSURFACE9 OldSurface = NULL;
	m_pDevice->GetRenderTarget(0, &OldSurface);

	m_RenderTargetIndex = 1 - m_RenderTargetIndex;

	//レンダーターゲットをセットする
	m_pDevice->SetRenderTarget(0, m_texData[1 - m_RenderTargetIndex].pSurface);

	LPDIRECT3DSURFACE9 OldDepthStencilSurface = NULL;
	m_pDevice->GetDepthStencilSurface(&OldDepthStencilSurface);

	//デプスバッファを使用しないので無効にする
	m_pDevice->SetDepthStencilSurface(NULL);

	// 波マップをテクスチャーステージ０にセットする
	m_pDevice->SetTexture(0, m_texData[m_RenderTargetIndex].pTexture);

	// 波マップの更新
	m_shader->Begine();
	m_shader->BeginePass(0);

	// 2Dポリゴンのレンダリング
	m_2dTex->Draw2DPolygon();

	m_shader->EndPass();

	// 更新した波マップを参照し水面を凸凹にしてレンダリングする
	m_pDevice->SetRenderTarget(0, m_texData[wsBUMP].pSurface);

	m_pDevice->SetTexture(0, m_texData[1 - m_RenderTargetIndex].pTexture);

	m_shader->BeginePass(1);

	// 2Dポリゴンのレンダリング
	m_2dTex->Draw2DPolygon();

	m_shader->EndPass();
	m_shader->End();

	//戻す
	m_pDevice->SetRenderTarget(0, OldSurface);
	SafeRelease(OldSurface);

	m_pDevice->SetDepthStencilSurface(OldDepthStencilSurface);
	SafeRelease(OldDepthStencilSurface);

	m_pDevice->SetViewport(&OldViewport);

	return m_texData[wsBUMP].pTexture;
}

/**
* @brief シェーダーを取得する
*/
ShaderPtr& WaveShader::GetShader()
{
	return m_shader;
}