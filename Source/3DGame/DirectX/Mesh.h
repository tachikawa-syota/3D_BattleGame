/**
* @file   Mesh.h
* @brief  メッシュデータ
* @author Syota Tachikawa
*/

#ifndef ___MESH_H
#define ___MESH_H

#include "..\Shader\ShaderManager.h"

/**
* @brief メッシュクラス
*/
class Mesh
{
public:
	/// ロードフラグ
	bool bLoad;

	/**
	* @brief コンストラクタ
	*/
	Mesh();

	/**
	* @brief デストラクタ
	*/
	virtual ~Mesh();

	/**
	* @brief クローン生成
	*/
	Mesh* Clone();

	/**
	* @brief メッシュ読み込み
	*/
	virtual bool LoadNonAnimMesh(string path, string xFileName);

	/**
	* @brief 更新
	*/
	virtual void Update();

	/**
	* @brief レンダリング
	*/
	virtual void RenderXMesh();

	/**
	* @brief レンダリング(シェーダーの適用)
	* @param view - ビュー行列
	* @param proj - 投影行列
	*/
	virtual void RenderXMesh(const Matrix& view, const Matrix& proj);
	
	/**
	* @brief 影を描画する
	*/
	virtual void DrawShadow();

	/**
	* @brief 当たり判定
	*/
	int	RayPick(Vector3* out, Vector3* pos, Vector3* vec, float *Dist);
	int	RayPickUD(Vector3* out, Vector3* pos, Vector3* vec, float *Dist);

private:
	/// テクスチャ数
	static const int NumTexture = 1024;
	/// デバイス
	LPDIRECT3DDEVICE9 m_pDevice;

protected:
	unsigned long m_dwFlags;
	unsigned char m_bChanged;

	/// シェーダー
	ShaderPtr m_shader;

	// メッシュ情報
	LPD3DXMESH m_lpMesh;

	/// マテリアル情報
	D3DMATERIAL9* m_lpMeshMaterials;
	
	/// マテリアルの数
	DWORD m_numMaterials;
	
	/// テクスチャ
	LPDIRECT3DTEXTURE9*	m_lpMeshTextures;
	
	/// ノーマルマップ
	LPDIRECT3DTEXTURE9*	m_lpNormal;
	
	/// スペキュラー
	IDirect3DTexture9**	m_lpSpecular;

	/// ハイトマップ
	IDirect3DTexture9**	m_lpHeight;

	LPD3DXFRAME	m_pFrameRoot;

	//転送行列
	Matrix m_transMatrix;

	/// 座標
	Vector3	m_pos;

	/// 角度
	Vector3	m_angle;

	/// 大きさ
	Vector3	m_scale;
public:
	/**
	* @brief エフェクトマテリアルのセット
	* @param shader - シェーダー
	* @param material - メッシュのマテリアル情報
	*
	* @note シェーダー描画をする時に使用する
	*/
	void SetEffectMaterial(const ShaderPtr& shader, const D3DMATERIAL9& material);

	/**
	* @brief シェーダーの設定
	*/
	void SetShader(const ShaderPtr& shader);

	/**
	* @brief ワールド座標を取得する
	*/
	const Matrix& GetTransMatrix(){ return m_transMatrix; }
	
	/**
	* @brief 座標を取得する
	*/
	const Vector3& GetPos(){ return m_pos; }

	/**
	* @brief 角度を取得する
	*/
	const Vector3& GetAngle(){ return m_angle; }
	
	/**
	* @brief 大きさを取得する
	*/
	const Vector3& GetScale(){ return m_scale; }
	
	/**
	* @brief メッシュ情報を取得する
	*/
	const LPD3DXMESH& GetMesh(){ return m_lpMesh; }

	LPD3DXFRAME GetpFrameroot() { return m_pFrameRoot; }
	
	/**
	* @brief 座標の設定
	*/
	void SetPos(const Vector3& pos);
	void SetPos(float x, float y, float z);

	/**
	* @brief 角度の設定
	*/
	void SetAngle(const Vector3& angle);
	void SetAngle(float angle);
	void SetAngle(float x, float y, float z);
	
	/**
	* @brief 大きさの設定
	*/
	void SetScale(const Vector3& scale);
	void SetScale(float scale);
	void SetScale(float x, float y, float z);
};

/// メッシュのスマートポインタ
using MeshPtr = shared_ptr<Mesh>;

#endif