/**
* @file   DepthBuffer.h
* @brief  深度バッファ
* @author Syota Tachikawa
*/

#ifndef ___DEPTHBUFFER_H
#define ___DEPTHBUFFER_H

#include "..\DirectX\TextureManager.h"
#include "Shader.h"

/**
* @brief 深度バッファクラス
*/
class DepthBuffer
{
public:
	/**
	* @brief コンストラクタ
	*/
	DepthBuffer();

	/**
	* @brief デストラクタ
	*/
	~DepthBuffer();

	/**
	* @brief 深度バッファレンダリング開始
	*/
	void Begin();

	/**
	* @brief 深度バッファレンダリング終了
	*/
	void End();

	/**
	* @brief シャドウマップシェーダーの取得
	*/
	ShaderPtr& GetShadowMapShader();

	/**
	* @brief スキンメッシュ用シャドウマップシェーダーの取得
	*/
	ShaderPtr& GetSkinShadowMapShader();

	/**
	* @brief 深度テクスチャを取得する
	*/
	const LPDIRECT3DTEXTURE9& GetTexture() const;

private:
	/// 通常メッシュ用シェーダー
	ShaderPtr m_shadowMap;

	/// スキンメッシュ用シェーダー
	ShaderPtr m_skinShadowMap;

	/// 深度バッファサーフェイス
	LPDIRECT3DSURFACE9 m_pDepthSurf;

	/// デバイスバックバッファ
	LPDIRECT3DSURFACE9 m_pDevBuf;

	/// デバイス深度バッファ
	LPDIRECT3DSURFACE9 m_pDevDepthSur;

	/// テクスチャデータ
	TEXTURE_DATA m_texData;

	/// ビューポート
	D3DVIEWPORT9 m_viewport;

	/// 前のビューポート
	D3DVIEWPORT9 m_oldViewport;

	/// シャドーマップ用のテクスチャのオフセット行列
	float m_fOffsetX;
	float m_fOffsetY;
	Matrix m_scaleBias;

	/**
	* @brief 現在の設定を保存する
	*/
	void SaveRenderTarget();
};

/// 深度バッファのスマートポインタ
using DepthBufferPtr = shared_ptr<DepthBuffer>;

#endif