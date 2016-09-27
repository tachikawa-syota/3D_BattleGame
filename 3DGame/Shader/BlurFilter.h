/**
* @file   BlurFilter.h
* @brief  ブラーを処理をする
* @author Syota Tachikawa
*/

#ifndef ___BLURFILTER_H
#define ___BLURFILTER_H

#include "..\DirectX\TextureManager.h"
#include "Shader.h"

/**
* @brief ブラーフィルタークラス
*/
class BlurFilter
{
public:
	/**
	* @enum TexDataType
	* @brief シェーダー用のテクスチャの種類
	*/
	enum TexDataType{
		Color = 0,
		LightBloom,
		Blur1,
		Blur2,
		DataMax
	};

	/**
	* @brief コンストラクタ
	*/
	BlurFilter();

	/**
	* @brief デストラクタ
	*/
	~BlurFilter();

	/**
	* @brief ブラー開始
	*/
	void Begin();

	/**
	* @brief ブラー終了
	*/
	void End();

	/**
	* @brief ガウシアンフィルターの重みを計算する
	*/
	void GaussWeight(float dispersion);

	/**
	* @brief ブラー用に作成したテクスチャを取得する
	*/
	const LPDIRECT3DTEXTURE9& GetTexture(int index) const;

	/**
	* @brief シェーダー情報の取得
	*/
	ShaderPtr& GetShader();

private:
	/// ブラーフィルター
	ShaderPtr m_shader;

	/// テクスチャマネージャー
	TextureManager* m_2dTex;

	// テクスチャデータ
	TEXTURE_DATA m_texData[DataMax];

	/// 前のサーフェイス
	LPDIRECT3DSURFACE9 m_oldSurface;
};

using BlurFilterPtr = shared_ptr<BlurFilter>;

#endif