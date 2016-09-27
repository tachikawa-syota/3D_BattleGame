#include "BlurFilter.h"
#include "..\DirectX\Direct3DData.h"
#include "..\ApplicationSystem\ApplicationData.h"

/**
* @brief コンストラクタ
*/
BlurFilter::BlurFilter()
{
	// シェーダー作成
	m_shader = make_shared<Shader>("Data/Shader/BlurFilter.fx");

	// インスタンスを取得
	m_2dTex = TextureManager::GetInstance();

	UINT winWidth = AppIns->GetWindowWidth();
	UINT winHeight = AppIns->GetWindowHeight();

	// シーンのレンダリング結果を格納するサーフェイス
	m_texData[Color] = m_2dTex->CreateTexture(winWidth, winHeight);

	// シーンの輝度のあふれ情報を格納するサーフェイス
	m_texData[LightBloom] = m_2dTex->CreateTexture(winWidth, winHeight);

	// ぼかすためのサーフェイス
	for (int i = 0; i < 2; i++){
		m_texData[i + Blur1] = m_2dTex->CreateTexture(winWidth, winHeight);
	}
}

/**
* @brief デストラクタ
*/
BlurFilter::~BlurFilter()
{

}

/**
* @brief ブラー開始
*/
void BlurFilter::Begin()
{
	// デバイスを取得する
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();

	// マルチレンダーターゲットに切り替える。
	// マルチレンダーターゲットサーフェイスは一部の実装ではすべてのサーフェイスが、フォーマットは異なっても、ビット深度は同じでなければならない場合がある。
	// バックバッファのカラービット深度は環境によって変わるため、
	// バックバッファとレンダーターゲットサーフェイスをマルチレンダーターゲットとして一緒に使用しないようにする。
	m_oldSurface = NULL;
	pDevice->GetRenderTarget(0, &m_oldSurface);
	pDevice->SetRenderTarget(0, m_texData[Color].pSurface);
	pDevice->SetRenderTarget(1, m_texData[LightBloom].pSurface);

	// ターゲットバッファのクリア、Ｚバッファのクリア
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0);
}

/**
* @brief ブラー終了
*/
void BlurFilter::End()
{
	// デバイスを取得する
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();

	// FVFにD3DFVF_XYZRHWを設定した2DオブジェクトでレンダリングするのでZバッファを使用しない
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

	// 1 / 4 サイズのサーフェイスを使用してブラー処理を行うのでビューポートを切り替える
	D3DVIEWPORT9 OldViewport, NewViewport;
	pDevice->GetViewport(&OldViewport);
	NewViewport.Width = AppIns->GetWindowWidth();
	NewViewport.Height = AppIns->GetWindowHeight();
	NewViewport.MaxZ = 1.0f;
	NewViewport.MinZ = 0.0f;
	NewViewport.X = 0;
	NewViewport.Y = 0;
	pDevice->SetViewport(&NewViewport);

	// ガウシアンフィルターの重みを計算する
	GaussWeight(15 * 15);

	m_shader->SetValue("MapWidth", static_cast<float>(AppIns->GetWindowWidth()));
	m_shader->SetValue("MapHeight", static_cast<float>(AppIns->GetWindowHeight()));
	m_shader->Begine();
	// レンダーターゲットをセットする
	pDevice->SetRenderTarget(0, m_texData[Blur1].pSurface);
	// X方向にブラーフィルターを適応する
	m_shader->SetBool("isXY", true);
	m_shader->BeginePass(0);
	m_shader->SetValue("SrcTexture", m_texData[LightBloom].pTexture);
	m_shader->CommitChanges();
	m_2dTex->DrawTexture(m_texData[LightBloom].pTexture);
	m_shader->EndPass();

	// レンダーターゲットをセットする
	pDevice->SetRenderTarget(0, m_texData[Blur2].pSurface);
	// バックバッファをテクスチャーステージ０にセットする
	// Y方向にブラーフィルターを適応する
	m_shader->SetBool("isXY", false);
	m_shader->BeginePass(0);
	m_shader->SetValue("SrcTexture", m_texData[Blur1].pTexture);
	m_shader->CommitChanges();									// 変更を確定
	m_2dTex->DrawTexture(m_texData[Blur1].pTexture);
	m_shader->EndPass();
	m_shader->End();

	// ビューポートを戻す
	pDevice->SetViewport(&OldViewport);

	// レンダーターゲットをバックバッファに戻す
	pDevice->SetRenderTarget(0, m_oldSurface);
	SafeRelease(m_oldSurface);

	// カラー情報をバックバッファにレンダリングする
	m_2dTex->DrawTexture(m_texData[Color].pTexture);

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//加算合成
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	m_2dTex->DrawTexture(m_texData[Blur2].pTexture);

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

/**
* @brief ガウシアンフィルターの重みを計算する
*/
void BlurFilter::GaussWeight(float dispersion)
{
	DWORD i;
	// 重みの配列
	float array[8];
	float total = 0;

	for (i = 0; i<8; i++){
		array[i] = expf(-0.5f*(FLOAT)(i*i) / dispersion);
		if (0 == i) {
			total += array[i];
		}
		else {
			// 中心以外は、２回同じ係数を使うので２倍
			total += 2.0f * array[i];
		}
	}
	// 規格化
	for (i = 0; i < 8; i++) {
		array[i] /= total;
	}
	if (m_shader) m_shader->SetValue("weight", array, 8);
}

/**
* @brief ブラー用に作成したテクスチャを取得する
*/
const LPDIRECT3DTEXTURE9& BlurFilter::GetTexture(int index) const
{
	return m_texData[index].pTexture;
}

/**
* @brief シェーダー情報の取得
*/
ShaderPtr& BlurFilter::GetShader()
{
	return m_shader;
}