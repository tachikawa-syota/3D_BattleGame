/**
* @file Light.h
* @brief ライトの処理を管理する
* @author Syota Tachikawa
*/

#ifndef ___LIGHT_H
#define ___LIGHT_H

#include "..\Math\Math.h"
#include "..\Shader\Shader.h"

/**
* @brief ライトクラス
*/
class Light
{
public:
	/**
	* @brief コンストラクタ
	*/
	Light();

	/**
	* @brief ライト有効化
	* @param ライト番号
	*/
	void EnableLight(DWORD index);
	
	/**
	* @brief ライト無効化
	* @param ライト番号
	*/
	void DisableLight(DWORD index);

	/**
	* @brief 環境光の設定
	* @param 環境光
	*/
	void SetAmbient(DWORD ambient);

	/**
	* @brief フォグの設定
	* @param param1 - 開始位置
	* @param param2 - 終了位置
	* @param color  - 色
	*/
	void SetFog(float Param1, float Param2, DWORD Color);

	/**
	* @brief 平行光源の設定
	* @param index - ライトの番号
	* @param dir - 向き
	* @param r,g,b - RGBカラー
	*/
	void DirLight(int index, Vector3 dir, float r, float g, float b);
	
	/**
	* @brief 平行光源の設定
	* @param shader - シェーダー
	* @param index - ライトの番号
	* @param dir   - 向き
	* @param r,g,b - RGBカラー
	*/
	void DirLight(ShaderPtr shader, int index, Vector3 dir, float r, float g, float b);

	/**
	* @brief 点光源の設定
	* @param index - ライトの番号
	* @param pos - 座標
	* @param r,g,b - RGBカラー
	* @param range ライトの範囲
	*/
	void PointLight(int index, Vector3 Pos, float r, float g, float b, float range);
	
	/**
	* @brief スポットライトの設定
	* @param index - ライトの番号
	* @param pos   - 座標
	* @param r,g,b - RGBカラー
	* @param range - ライトの範囲
	*/
	void SpotLight(int index, Vector3 Pos, float r, float g, float b, float range);

private:
	/// デバイス
	LPDIRECT3DDEVICE9 m_pDevice;
};

#endif