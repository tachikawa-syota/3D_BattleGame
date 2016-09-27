/**
* @file   Shader.h
* @brief  シェーダーシステムの管理
* @author Syota Tachikawa
*/

#ifndef ___SHADER_H
#define ___SHADER_H

#include "..\Math\Math.h"

/**
* @brief シェーダークラス
*/
class Shader
{
public:
	/**
	* @brief コンストラクタ
	* @param fxファイルの名前
	*/
	Shader(string fileName);

	/**
	* @brief デストラクタ
	*/
	~Shader();

	/**
	* @brief エフェクトの開始
	*/
	UINT Begine();

	/**
	* @brief エフェクトの終了
	*/
	void End();

	/**
	* @brief パス開始
	*/
	void BeginePass(int pass);

	/**
	* @brief パス開始
	*/
	void EndPass();

	/**
	* @brief ステイトの変化をデバイスに伝える
	*/
	void CommitChanges();

	/**
	* @brief エフェクトを取得する
	*/
	LPD3DXEFFECT GetEffect(){ return m_pShader; }

	/**
	* @brief パラメーターを取得する
	*/
	D3DXHANDLE GetParameter(D3DXHANDLE paramName){ return m_pShader->GetParameterByName(NULL, paramName); }

	/**
	* @brief メインテクスチャの設定
	*/
	void SetTexture(IDirect3DTexture9* tex);
	/**
	* @brief メインテクスチャの設定
	*/
	void SetTexture(IDirect3DTexture9& tex);

	void SetMatrix(const Matrix* mat);
	void SetMatrix(const Matrix& mat);

	void SetValue(string name, IDirect3DTexture9* tex);
	void SetValue(string name, IDirect3DTexture9& tex);

	void SetValue(string name, const Matrix* mat);
	void SetValue(string name, const Matrix& mat);

	void SetValue(string name, const D3DXVECTOR4* v);
	void SetValue(string name, const D3DXVECTOR4& v);

	void SetValue(string name, const Vector3* v);
	void SetValue(string name, const Vector3& v);
	void SetValue(string name, float* pf, int count);

	void SetValue(string name, float f);
	void SetValue(string name, int d);
	void SetValue(string name, DWORD d);
	void SetValue(string name, const void* v, byte b);

	void SetBool(string name, bool bFlag);

private:
	/// シェーダー
	LPD3DXEFFECT m_pShader;

	/// 射影変換行列
	D3DXHANDLE m_hmWVP;

	/// テクスチャ
	D3DXHANDLE m_htexDecale;
};

/// シェーダーのスマートポインタ
using ShaderPtr = shared_ptr<Shader>;

#endif