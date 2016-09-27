/**
* @file   ShaderManager.h
* @brief  シェーダーを管理する
* @author Syota Tachikawa
*/

#ifndef ___SHADERMANAGER_H
#define ___SHADERMANAGER_H

#include "..\Object\Camera.h"
#include "BlurFilter.h"
#include "DepthBuffer.h"

/**
* @brief シェーダー管理クラス
*/
class ShaderManager
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
	* @brief インスタンスを取得する（シングルトン）
	*/
	static ShaderManager* GetInstance(){
		static ShaderManager singleton;
		return &singleton;
	}

	/**
	* @brief シェーダーを追加する
	* @param shader - シェーダー
	* @param tecName - テクニックの名前
	*/
	void Add(const ShaderPtr& shader, string tecName);

	/**
	* @brief 作成
	*/
	HRESULT Create();

	/**
	* @brief 初期化
	*/
	HRESULT Initialize();

	/**
	* @brief 更新
	* @param world - ワールド行列
	* @param camera - カメラクラスのインスタンス
	*/
	void Update(const Matrix& world, const CameraPtr& camera);

	/**
	* @brief 深度バッファレンダリング開始
	*/
	void BeginDepth();

	/**
	* @brief 深度バッファレンダリング終了
	*/
	void EndDepth();

	/**
	* @brief ブラー開始
	*/
	void BeginBlur();

	/**
	* @brief ブラー終了
	*/
	void EndBlur();

	/**
	* @brief ガウシアンフィルターの重みを計算する
	*/
	void GaussWeight(float dispersion);

	/**
	* @brief ライトの座標を取得する
	*/
	const Vector3& GetLightPos();

	/**
	* @brief ライトの注視点を取得する
	*/
	const Vector3& GetLightAt();

	/**
	* @brief ライトのビュー行列を取得する
	*/
	const Matrix& GetLightView();

	/**
	* @brief ライトの投影行列を取得する
	*/
	const Matrix& GetLightProjection();

	/**
	* @brief シェーダー用のテクスチャを取得する
	*/
	const LPDIRECT3DTEXTURE9& GetShaderTexture(TexDataType index) const;

	/**
	* @brief シャドウマップテクスチャを取得する
	*/
	const LPDIRECT3DTEXTURE9& GetShadowMapTexture() const;

	/**
	* @brief スタンダードシェーダーを取得する
	*/
	const ShaderPtr& GetStandardShader() const;

	/**
	* @brief シャドウマップシェーダーを取得する
	*/
	const ShaderPtr& GetShadowMapShader() const;

	/**
	* @brief スキンメッシュ用シャドウマップシェーダーを取得する
	*/
	const ShaderPtr& GetSkinShadowMapShader() const;

private:
	/// スタンダードシェーダー
	ShaderPtr m_standard;

	/// ブラーフィルター
	BlurFilterPtr m_blurFilter;

	/// 深度バッファ
	DepthBufferPtr m_depthBuffer;

	/// シェーダーリスト
	list<ShaderPtr> m_shaderList;

	/// ライトのビュー行列
	Matrix m_lightView;

	/// ライトの投影行列
	Matrix m_lightProj;

	/// ライトの座標
	Vector3 m_lightPos;

	/// ライトの注視点
	Vector3 m_lightAt;

	/// アンビエント
	Vector4 m_ambient;

	/// ディフューズ
	Vector4 m_diffuse;

	/// スペキュラー
	Vector4 m_specular;

	/// エミッシブ
	Vector4 m_emmisive;
};

#endif