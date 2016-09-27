#include "Light.h"
#include "..\DirectX\Direct3DData.h"

/**
* @brief コンストラクタ
*/
Light::Light()
{
	// デバイスをセット
	m_pDevice = D3DIns->GetDevice();
}

/**
* @brief ライト有効化
* @param ライト番号
*/
void Light::EnableLight(DWORD index)
{
	m_pDevice->LightEnable(index, TRUE);
}

/**
* @brief ライト無効化
* @param ライト番号
*/
void Light::DisableLight(DWORD index)
{
	m_pDevice->LightEnable(index, FALSE);
}

/**
* @brief 環境光の設定
* @param 環境光
*/
void Light::SetAmbient(DWORD ambient)
{
	m_pDevice->SetRenderState(D3DRS_AMBIENT, ambient);
}

/**
* @brief フォグの設定
* @param param1 - 開始位置
* @param param2 - 終了位置
* @param color  - 色
*/
void Light::SetFog(float param1, float param2, DWORD color)
{
	m_pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	//フォグカラー設定
	m_pDevice->SetRenderState(D3DRS_FOGCOLOR, color);
	//フォグモード設定
	m_pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
	m_pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	//パラメータ設定
	m_pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&param1));
	m_pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&param2));
}

/**
* @brief 平行光源の設定
* @param index - ライトの番号
* @param dir   - 向き
* @param r,g,b - RGBカラー
*/
void Light::DirLight(int index, Vector3 dir, float r, float g, float b)
{
	D3DLIGHT9 Light;
	ZeroMemory(&Light, sizeof(D3DLIGHT9));
	// ライトタイプ
	Light.Type = D3DLIGHT_DIRECTIONAL;
	// 拡散光
	Light.Diffuse.r = r;
	Light.Diffuse.g = g;
	Light.Diffuse.b = b;
	Light.Diffuse.a = 1.0f;
	// 光沢
	Light.Specular.r = r;
	Light.Specular.g = g;
	Light.Specular.b = b;
	Light.Specular.a = 1.0f;
	// 環境光
	Light.Ambient.r = 0.5f;
	Light.Ambient.g = 0.5f;
	Light.Ambient.b = 0.5f;
	Light.Ambient.a = 1.0f;
	Light.Direction.x = dir.x;
	Light.Direction.y = dir.y;
	Light.Direction.z = dir.z;
	// ライトの設定
	m_pDevice->SetLight(index, &Light);
}

/**
* @brief 平行光源の設定
* @param shader - シェーダー
* @param index - ライトの番号
* @param dir   - 向き
* @param r,g,b - RGBカラー
*/
void Light::DirLight(ShaderPtr shader, int index, Vector3 dir, float r, float g, float b)
{
	dir.Normalize();
	DirLight(index, dir, r, g, b);
	//	シェーダー設定
	Vector3	work = dir;
	Vector3 color(r, g, b);
	shader->SetValue("LightDir", work);
	shader->SetValue("DirLightColor", &color);
}

/**
* @brief 点光源の設定
* @param index - ライトの番号
* @param pos   - 座標
* @param r,g,b - RGBカラー
* @param range - ライトの範囲
*/
void Light::PointLight(int index, Vector3 pos, float r, float g, float b, float range)
{
	D3DLIGHT9 Light;
	ZeroMemory(&Light, sizeof(D3DLIGHT9));
	// ライトタイプ
	Light.Type = D3DLIGHT_POINT;
	// 拡散光
	Light.Diffuse.r = r;
	Light.Diffuse.g = g;
	Light.Diffuse.b = b;
	Light.Diffuse.a = 1.0f;
	// 光沢
	Light.Specular.r = r;
	Light.Specular.g = g;
	Light.Specular.b = b;
	// 環境光
	Light.Ambient.r = 0.0f;
	Light.Ambient.g = 0.0f;
	Light.Ambient.b = 0.0f;
	Light.Ambient.a = 0.0f;

	Light.Direction.x = 1.0f;
	Light.Direction.y = 0.0f;
	Light.Direction.z = 0.0f;
	// 座標
	Light.Position.x = pos.x;
	Light.Position.y = pos.y;
	Light.Position.z = pos.z;
	// 範囲
	Light.Range = range;

	Light.Attenuation0 = 1.0f;
	Light.Attenuation1 = 0.8f / range;
	Light.Attenuation2 = 0.0f;
	// ライトの設定
	m_pDevice->SetLight(index, &Light);
}

/**
* @brief スポットライトの設定
* @param index - ライトの番号
* @param pos   - 座標
* @param r,g,b - RGBカラー
* @param range - ライトの範囲
*/
void Light::SpotLight(int index, Vector3 pos, float r, float g, float b, float range)
{
	D3DLIGHT9 Light;
	ZeroMemory(&Light, sizeof(D3DLIGHT9));
	// ライトタイプ
	Light.Type = D3DLIGHT_SPOT;
	// 拡散光
	Light.Diffuse.r = r;
	Light.Diffuse.g = g;
	Light.Diffuse.b = b;
	Light.Diffuse.a = 0.5f;
	// 光沢
	Light.Specular.r = r;
	Light.Specular.g = g;
	Light.Specular.b = b;
	// 環境光
	Light.Ambient.r = 0.6f;
	Light.Ambient.g = 0.6f;
	Light.Ambient.b = 0.6f;
	Light.Ambient.a = 0.5f;
	// 方向
	Light.Direction.x = 0.0f;
	Light.Direction.y = -2.0f;
	Light.Direction.z = 0.0f;
	// 位置
	Light.Position.x = pos.x;
	Light.Position.y = pos.y;
	Light.Position.z = pos.z;
	// 内部コーン
	Light.Theta = 4.0f;
	// 外部コーン
	Light.Phi = 5.0f;
	// 範囲
	Light.Range = range;
	Light.Attenuation0 = 1.0f;
	Light.Attenuation1 = 0.8f / range;
	Light.Attenuation2 = 0.0f;
	// ライトの設定
	m_pDevice->SetLight(index, &Light);
}