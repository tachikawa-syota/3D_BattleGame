#include "ShaderManager.h"
#include "..\ApplicationSystem\ApplicationData.h"
#include "..\DirectX\Direct3DData.h"

/**
* @brief シェーダーを追加する
* @param shader - シェーダー
* @param tecName - テクニックの名前
*/
void ShaderManager::Add(const ShaderPtr& shader, string tecName)
{
	// パラメータの設定
	Vector3 ld = m_lightAt - m_lightPos;
	shader->SetValue("LightDir", &ld);
	shader->SetValue("Diffuse", &m_diffuse);
	shader->SetValue("Ambient", &m_ambient);
	shader->SetValue("Specular", &m_specular);
	shader->SetValue("Emmisive", &m_emmisive);
//	shader->SetValue("EyePos", &camera->GetPos());

	// テクニックの設定
	shader->GetEffect()->SetTechnique(tecName.c_str());

	// シャドウマップ用 -------------------------------------
	shader->SetValue("LV", &GetLightView());
	shader->SetValue("LP", &GetLightProjection());

	shader->SetValue("ShadowMapTexture", m_depthBuffer->GetTexture());

	// ここまで----------------------------------------------

	// 作成したシェーダーをリストに登録
	m_shaderList.emplace_back(shader);
}

/**
* @brief 作成
*/
HRESULT ShaderManager::Create()
{
	// スタンダードシェーダーの作成
	m_standard = make_shared<Shader>("Data/Shader/StandardShader.fx");

	// 深度バッファの作成
	m_depthBuffer = make_shared<DepthBuffer>();

	// ブラーフィルターの作成
	m_blurFilter = make_shared<BlurFilter>();

	// シャドウマップシェーダーをリストに登録
	Add(m_depthBuffer->GetShadowMapShader(), "ZShadowMap");
	//	Add(m_depthBuffer->GetSkinShadowMapShader());
	// ブラーフィルターをリストに登録
	Add(m_blurFilter->GetShader(), "BlurFilter");

	// 成功
	return S_OK;
}

/**
* @brief 初期化
*/
HRESULT ShaderManager::Initialize()
{
	m_lightPos = Vector3(0.0f, 11.8f, 20.4f);
	m_lightAt = Vector3(0.0f, 0.0f, 7.0f);
	m_ambient = Vector4(0.8f, 0.8f, 0.8f, 0.8f);
	m_diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_emmisive = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	
	// 成功
	return S_OK;
}

/**
* @brief 更新
* @param matWorld - モデルのワールド変換行列
* @param camera - カメラクラス
*/
void ShaderManager::Update(const Matrix& world, const CameraPtr& camera)
{
	// リストに登録されている数だけループ
	for (auto& it : m_shaderList)
	{
		// パラメータの設定
		it->SetValue("W", &world);
		it->SetValue("V", &camera->GetView());
		it->SetValue("P", &camera->GetProj());

		// ライトの向き
		Vector3 ld = m_lightAt - m_lightPos;

		it->SetValue("LightDir", &ld);
		it->SetValue("Diffuse", &m_diffuse);
		it->SetValue("Ambient", &m_ambient);
		it->SetValue("Specular", &m_specular);
		it->SetValue("Emmisive", &m_emmisive);
		it->SetValue("EyePos", &camera->GetPos());

		//シャドウマップ用
		it->SetValue("LV", &GetLightView());
		it->SetValue("LP", &GetLightProjection());
		it->SetValue("ShadowMapTexture", m_depthBuffer->GetTexture());
	}

	// ライトの座標を少しずつズラす(デバッグ用)
	m_lightPos.z -= 0.001f;
}

/**
* @brief 深度バッファレンダリング開始
*/
void ShaderManager::BeginDepth()
{
	m_depthBuffer->Begin();
}

/**
* @brief 深度バッファレンダリング終了
*/
void ShaderManager::EndDepth()
{
	m_depthBuffer->End();
}

/**
* @brief ブラー開始
*/
void ShaderManager::BeginBlur()
{
	m_blurFilter->Begin();
}

/**
* @brief ブラー終了
*/
void ShaderManager::EndBlur()
{
	m_blurFilter->End();
}

/**
* @brief ガウシアンフィルターの重みを計算する
*/
void ShaderManager::GaussWeight(float dispersion)
{
	m_blurFilter->GaussWeight(dispersion);
}

/**
* @brief ライトの座標を取得する
*/
const Vector3& ShaderManager::GetLightPos()
{
	return m_lightPos;
}

/**
* @brief ライトの注視点を取得する
*/
const Vector3& ShaderManager::GetLightAt()
{
	return m_lightAt;
}

/**
* @brief ライトのビュー行列を取得する
*/
const Matrix& ShaderManager::GetLightView()
{
	LookAtLH(m_lightView, m_lightPos, m_lightAt, Vector3(0.0f, 1.0f, 0.0f));
	return m_lightView;
}

/**
* @brief ライトの投影行列を取得する
*/
const Matrix& ShaderManager::GetLightProjection()
{
	OlthoLH(m_lightProj,
		static_cast<float>(AppIns->GetWindowWidth()),
		static_cast<float>(AppIns->GetWindowHeight()),
		0.1f, 200.0f);

	return m_lightProj;
}

/**
* @brief シェーダー用のテクスチャを取得する
*/
const LPDIRECT3DTEXTURE9& ShaderManager::GetShaderTexture(TexDataType index) const
{
	return m_blurFilter->GetTexture(index);
}

/**
* @brief シャドウマップテクスチャを取得する
*/
const LPDIRECT3DTEXTURE9& ShaderManager::GetShadowMapTexture() const
{
	return m_depthBuffer->GetTexture();
}

/**
* @brief スタンダードシェーダーを取得する
*/
const ShaderPtr& ShaderManager::GetStandardShader() const
{
	return m_standard;
}

/**
* @brief シャドウマップシェーダーを取得する
*/
const ShaderPtr& ShaderManager::GetShadowMapShader() const
{
	return m_depthBuffer->GetShadowMapShader();
}

/**
* @brief スキンメッシュ用シャドウマップシェーダーを取得する
*/
const ShaderPtr& ShaderManager::GetSkinShadowMapShader() const
{
	return m_depthBuffer->GetSkinShadowMapShader();
}