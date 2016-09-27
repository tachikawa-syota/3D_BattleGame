/**
* @file   WaveShader.h
* @brief  波シェーダー処理を管理する
* @author Syota Tachikawa
*/

#ifndef ___WAVESHADER_H
#define ___WAVESHADER_H

#include "..\Common\FileData.h"

enum WAVE_SURFACE
{
	wsMAIN01,
	wsMAIN02,
	wsBUMP,
	wsMAX,
};

/**
* @brief 波シェーダーを管理するクラス
*/
class WaveShader
{
public:
	/**
	* @brief コンストラクタ
	* @param width - 作成するテクスチャの縦幅
	* @param height - 作成するテクスチャの横幅
	*/
	WaveShader(UINT width, UINT height);

	/**
	* @brief 作成
	* @param width - 作成するテクスチャの縦幅
	* @param height - 作成するテクスチャの横幅
	*/
	HRESULT Create(UINT width, UINT height);

	/**
	* @brief 波を追加する
	*/
	void AddWave(float posX, float posY, float height);

	/**
	* @brief 描画
	*/
	LPDIRECT3DTEXTURE9 Render();

	/**
	* @brief シェーダーを取得する
	*/
	ShaderPtr& GetShader();

private:
	/// シェーダー
	ShaderPtr m_shader;

	/// テクスチャデータ
	TEXTURE_DATA m_texData[wsMAX];

	/// デバイス
	LPDIRECT3DDEVICE9 m_pDevice;

	/// テクスチャ
	LPDIRECT3DTEXTURE9* m_pTexture;

	/// サーフェイス
	LPDIRECT3DSURFACE9* m_pSurface;

	/// テクスチャマネージャー
	TextureManager* m_2dTex;

	/// 波マップのインデックス
	int m_RenderTargetIndex;
};

/// 波シェーダーのスマートポインタ
using WaveShaderPtr = shared_ptr<WaveShader>;

#endif