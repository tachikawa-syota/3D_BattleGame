#include "Mesh.h"
#include "..\DirectX\Direct3DData.h"

/**
* @brief コンストラクタ
*/
Mesh::Mesh()
{
	m_shader = nullptr;
	//メッシュ基本変数
	m_lpMesh = NULL;
	m_lpMeshTextures = NULL;
	m_lpMeshMaterials = NULL;
	m_numMaterials = 0;
	//初期設定
	SetPos(Vector3(0, 0, 0));
	SetScale(1.0f);
	SetAngle(0.0f);
	Update();
}

/**
* @brief デストラクタ
*/
Mesh::~Mesh()
{
}

/**
* @brief クローンを生成する
*/
Mesh* Mesh::Clone()
{
	//オブジェクト生成
	Mesh* obj = new Mesh(*this);
	obj->bLoad = false;
	return 0;
}

/**
* @brief Xファイルの読み込み
* @param path      - パス
* @param xFileName - ファイルの名前
*/
bool Mesh::LoadNonAnimMesh(string path, string xFileName)
{
	// デバイスの取得
	m_pDevice = D3DIns->GetDevice();

	//マテリアル情報を受け取る変数
	LPD3DXBUFFER pD3DXAdjacencyBuffer = NULL;
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;
	LPD3DVERTEXELEMENT9 pD3DXElement = NULL;

	// Xファイルを読み込む
	string modelName = path + xFileName;

	//Xファイルからメッシュをロードする
	if (FAILED(D3DXLoadMeshFromX(modelName.c_str(), D3DXMESH_MANAGED, m_pDevice, &pD3DXAdjacencyBuffer,
		&pD3DXMtrlBuffer, NULL, &m_numMaterials, &m_lpMesh))){
		return false;
	}
	//最適化
	if (FAILED(m_lpMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)pD3DXAdjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL))){
		return false;
	}
	SafeRelease(pD3DXAdjacencyBuffer);
	//マテリアルとマテリアルに属するテクスチャの読み込み
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	//マテリアルとテクスチャのオブジェクト生成
	m_lpMeshMaterials = new D3DMATERIAL9[m_numMaterials];
	m_lpMeshTextures = new LPDIRECT3DTEXTURE9[m_numMaterials];
	m_lpNormal = new  LPDIRECT3DTEXTURE9[m_numMaterials];
	m_lpSpecular = new  LPDIRECT3DTEXTURE9[m_numMaterials];
	m_lpHeight = new  LPDIRECT3DTEXTURE9[m_numMaterials];

	//マテリアル情報とテクスチャ情報取得
	for (DWORD i = 0; i < m_numMaterials; i++){
		//マテリアル情報
		m_lpMeshMaterials[i] = d3dxMaterials[i].MatD3D;
		m_lpMeshMaterials[i].Ambient = m_lpMeshMaterials[i].Diffuse;
		//テクスチャ情報
		m_lpMeshTextures[i] = NULL;
		m_lpNormal[i] = NULL;
		//テクスチャ読み込み
		if (d3dxMaterials[i].pTextureFilename != NULL){

			// テクスチャの名前
			string texFileName;
			texFileName = path + d3dxMaterials[i].pTextureFilename;

			if (FAILED(D3DXCreateTextureFromFile(m_pDevice, texFileName.c_str(), &m_lpMeshTextures[i]))){
				m_lpMeshTextures[i] = NULL;
			}
		}
	}
	//行列バッファの解放
	SafeRelease(pD3DXMtrlBuffer);

	// 成功
	return true;
}


void Mesh::SetPos(float x, float y, float z)
{
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}
void Mesh::SetPos(const Vector3& pos)
{
	m_pos = pos;
}

void Mesh::SetAngle(float x, float y, float z)
{
	m_angle.x = x;
	m_angle.y = y;
	m_angle.z = z;
}
void Mesh::SetAngle(float angle)
{
	m_angle.x = 0.0f;
	m_angle.y = angle;
	m_angle.z = 0.0f;
}
void Mesh::SetAngle(const Vector3& angle)
{
	m_angle = angle;
}


void Mesh::SetScale(float x, float y, float z)
{
	m_scale.x = x;
	m_scale.y = y;
	m_scale.z = z;
}
void Mesh::SetScale(float scale)
{
	m_scale.x = scale;
	m_scale.y = scale;
	m_scale.z = scale;
}
void Mesh::SetScale(const Vector3& scale)
{
	m_scale = scale;
}

/**
* @brief 更新
*/
void Mesh::Update()
{
	Matrix matScale;
	//サイズ設定
	D3DXMatrixScaling(&matScale, m_scale.x, m_scale.y, m_scale.z);
	//座標・回転設定 
	SetTransformMatrixXYZ(&m_transMatrix, m_pos.x, m_pos.y, m_pos.z, m_angle.x, m_angle.y, m_angle.z);
	//転送行列作成 
	m_transMatrix = matScale * m_transMatrix;
}

/**
* @brief 描画
*/
void Mesh::RenderXMesh()
{
	// デバイスの取得
	m_pDevice = D3DIns->GetDevice();

	//転送行列設定
	m_pDevice->SetTransform(D3DTS_WORLD, &m_transMatrix);
	//マテリアルセットごとにメッシュ描画
	for (DWORD i = 0; i < m_numMaterials; i++){
		//材質設定
		m_pDevice->SetMaterial(&m_lpMeshMaterials[i]);
		//テクスチャ設定
		m_pDevice->SetTexture(0, m_lpMeshTextures[i]);
		//材質グループ描画
		m_lpMesh->DrawSubset(i);
	}
}

/**
* @brief レンダリング(シェーダーの適用)
* @param shader - シェーダークラス(スマートポインタ)
* @param name - シェーダーパスの名前
* @param view - ビュー行列
* @param proj - 投影行列
*/
void Mesh::RenderXMesh(const Matrix& view, const Matrix& proj)
{
	// シェーダーが設定されていなかったらデフォルトシェーダーを適応する
	if (m_shader == nullptr){
		m_shader = ShaderManager::GetInstance()->GetStandardShader();
		m_shader->GetEffect()->SetTechnique("StandardShader");
	}

	//シェーダーの適用
	UINT pass = m_shader->Begine();
	//ローカル-射影変換行列
	m_shader->SetValue("W", m_transMatrix);
	m_shader->SetValue("V", view);
	m_shader->SetValue("P", proj);

	for (DWORD p = 0; p<pass; p++){
		//パスのレンダリング
		m_shader->BeginePass(p);
		for (DWORD i = 0; i < m_numMaterials; i++){
			//テクスチャ指定
			m_shader->SetTexture(m_lpMeshTextures[i]);
			//法線マップテクスチャ設定
	//		m_shader->SetValue("NormalMapTexture", m_lpNormal[i]);
			//スペキュラマップテクスチャ
	//		m_shader->SetValue("SpecularMap", m_lpMeshTextures[i]);
			//高さマップテクスチャ
	//		m_shader->SetValue("HeightMap", m_lpMeshTextures[i]);
			
	//		SetEffectMaterial(m_shader, m_lpMeshMaterials[i]);
			//コミットチェンジ
			m_shader->CommitChanges();
			//材質グループ描画
			m_lpMesh->DrawSubset(i);
		}
		m_shader->EndPass();
	}
	m_shader->End();
}

/**
* @brief レイピック（一番近いメッシュデータ）
* @param out
* @param pos  - 判定メッシュの位置
* @param vec  - 方向ベクトル
* @param Dist - 距離
*/
int	Mesh::RayPick(Vector3* out, Vector3* pos, Vector3* vec, float *Dist){
	int ret = -1;
	if (vec->x == 0.0f && vec->z == 0.0f) return RayPickUD(out, pos, vec, Dist);
	Vector3 p = *pos;		//レイの開始位置
	Vector3 vv = *vec;		//向きベクトル
	float neart = *Dist;
	float dist = *Dist;
	dist = dist * dist;
	*out = p;
	//情報取得(頂点フォーマット)
	ULONG fvf = m_lpMesh->GetFVF();
	//頂点サイズ計算
	int VertexSize = D3DXGetFVFVertexSize(fvf) / sizeof(float);
	//バッファロック
	float*	pVertices;
	WORD*	pIndices;
	ULONG	NumIndices = m_lpMesh->GetNumFaces();
	//頂点バッファをロック
	m_lpMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertices);
	//インデックスバッファをロック(ポインタへのメモリ取得)
	m_lpMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndices);
	//面の三頂点
	Vector3 v1, v2, v3;
	Vector3	n;
	//辺の長さ
	Vector3	l1, l2, l3;
	Vector3	temp;
	Vector3	cp;
	Vector3 p1, p2, p3;
	for (ULONG j = 0; j < NumIndices; j++){
		//面頂点取得
		int a = pIndices[j * 3 + 0] * VertexSize;
		v1.x = pVertices[a];
		v1.y = pVertices[a + 1];
		v1.z = pVertices[a + 2];
		int b = pIndices[j * 3 + 1] * VertexSize;
		v2.x = pVertices[b];
		v2.y = pVertices[b + 1];
		v2.z = pVertices[b + 2];
		int c = pIndices[j * 3 + 2] * VertexSize;
		v3.x = pVertices[c];
		v3.y = pVertices[c + 1];
		v3.z = pVertices[c + 2];
		//距離判定
		//Vector3	ss = (v1 + v2 + v3) / 3.0f - p;
		//if( ss.LengthSq() > dist ) continue;
		l1.x = v2.x - v1.x;	l1.y = v2.y - v1.y;	l1.z = v2.z - v1.z;
		l2.x = v3.x - v2.x;	l2.y = v3.y - v2.y;	l2.z = v3.z - v2.z;
		//外積による法線算出		
		Vector3Cross(n, l1, l2);
		//内積の結果がプラスならば裏向き
		float dot = Vector3Dot(vv, n);
		if (dot >= 0) continue;
		//交点算出
		p1.x = v1.x - p.x;	p1.y = v1.y - p.y;	p1.z = v1.z - p.z;
		float t = Vector3Dot(n, p1) / dot;
		if (t < .0f || t > neart) continue;

		cp.x = vv.x*t + p.x;	cp.y = vv.y*t + p.y;	cp.z = vv.z*t + p.z;
		//内点判定
		p1.x = v1.x - cp.x;		p1.y = v1.y - cp.y;	p1.z = v1.z - cp.z;
		Vector3Cross(temp, p1, l1);
		if (Vector3Dot(temp, n) < 0.0f) continue;

		p2.x = v2.x - cp.x;		p2.y = v2.y - cp.y;		p2.z = v2.z - cp.z;
		Vector3Cross(temp, p2, l2);
		if (Vector3Dot(temp, n) < 0.0f) continue;

		l3.x = v1.x - v3.x;		l3.y = v1.y - v3.y;		l3.z = v1.z - v3.z;
		p3.x = v3.x - cp.x;		p3.y = v3.y - cp.y;		p3.z = v3.z - cp.z;
		//外積
		Vector3Cross(temp, p3, l3);
		//内積
		if (Vector3Dot(temp, n) < 0.0f) continue;
		*out = cp;
		*vec = n;
		ret = j;
		neart = t;
	}
	//頂点バッファのロックを解除
	m_lpMesh->UnlockVertexBuffer();
	//インデックスバッファをロックを解除
	m_lpMesh->UnlockIndexBuffer();
	*Dist = neart;
	return	ret;
}

/**
* @brief 上下最適化
* @param out
* @param pos  - 判定メッシュの位置
* @param vec  - 方向ベクトル
* @param Dist - 距離
*/
int	Mesh::RayPickUD(Vector3* out, Vector3* pos, Vector3* vec, float *Dist){
	float t, neart;
	float vy;
	int ret = -1;

	int	VertexSize;

	Vector3	p = *pos;
	vy = vec->y;

	neart = *Dist;
	//情報取得	
	int fvf = m_lpMesh->GetFVF();
	//頂点サイズ計算
	VertexSize = D3DXGetFVFVertexSize(fvf) / sizeof(float);
	//バッファロック
	float*	pVertices;
	WORD*	pIndices;
	int		NumIndices = m_lpMesh->GetNumFaces();
	m_lpMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertices);
	m_lpMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndices);

	Vector	l1, l2, l3;
	Vector	p1, p2, p3;
	Vector v[3];
	Vector n;

	for (int j = 0; j<NumIndices; j++){
		//面頂点取得
		int a = pIndices[j * 3 + 0] * VertexSize;
		int b = pIndices[j * 3 + 1] * VertexSize;
		int c = pIndices[j * 3 + 2] * VertexSize;

		v[0].x = pVertices[a];	v[1].x = pVertices[b];	v[2].x = pVertices[c];
		if (v[0].x > p.x && v[1].x > p.x && v[2].x > p.x) continue;

		v[0].z = pVertices[a + 2];	v[1].z = pVertices[b + 2];	v[2].z = pVertices[c + 2];
		if (v[0].z > p.z && v[1].z > p.z && v[2].z > p.z) continue;

		v[0].y = pVertices[a + 1];	v[1].y = pVertices[b + 1];	v[2].y = pVertices[c + 1];
		//内点判定（全外積がマイナス）		
		l1.x = v[1].x - v[0].x;
		l1.z = v[1].z - v[0].z;
		p1.x = v[0].x - p.x;
		p1.z = v[0].z - p.z;
		if ((p1.x*l1.z - p1.z*l1.x)*vy < 0) continue;

		l2.x = v[2].x - v[1].x;
		l2.z = v[2].z - v[1].z;
		p2.x = v[1].x - p.x;
		p2.z = v[1].z - p.z;
		if ((p2.x*l2.z - p2.z*l2.x)*vy < 0) continue;

		l3.x = v[0].x - v[2].x;
		l3.z = v[0].z - v[2].z;
		p3.x = v[2].x - p.x;
		p3.z = v[2].z - p.z;
		if ((p3.x*l3.z - p3.z*l3.x)*vy < 0) continue;
		//外積による法線算出		
		l1.y = v[1].y - v[0].y;
		l2.y = v[2].y - v[1].y;
		Vector3Cross(n, l1, l2);
		//表裏判定
		if (vy*n.y >= 0) continue;
		//交点算出
		p1.y = v[0].y - p.y;
		t = Vector3Dot(n, p1) / (n.y*vy);
		if (t < .0f || t > neart) continue;

		*vec = n;
		ret = j;
		neart = t;
	}
	m_lpMesh->UnlockVertexBuffer();
	m_lpMesh->UnlockIndexBuffer();

	out->y = neart*vy + p.y;
	out->x = pos->x;
	out->z = pos->z;
	*Dist = neart;

	return	ret;
}

/**
* @brief エフェクトマテリアルのセット
* @param shader - シェーダー
* @param material - メッシュのマテリアル情報
*
* @note シェーダー描画をする時に使用する
*/
void Mesh::SetEffectMaterial(const ShaderPtr& shader, const D3DMATERIAL9& material)
{
	Vector4 tempcolor;
	// 環境光用のマテリアルをセット
	tempcolor.x = material.Ambient.r;
	tempcolor.y = material.Ambient.g;
	tempcolor.z = material.Ambient.b;
	tempcolor.w = material.Ambient.a;
	shader->SetValue("AmbientMat", &tempcolor);

	// ディフューズ光用のマテリアルをセット
	tempcolor.x = material.Diffuse.r;
	tempcolor.y = material.Diffuse.g;
	tempcolor.z = material.Diffuse.b;
	tempcolor.w = material.Diffuse.a;
	shader->SetValue("DiffuseMat", &tempcolor);

	tempcolor.x = material.Specular.r;
	tempcolor.y = material.Specular.g;
	tempcolor.z = material.Specular.b;
	tempcolor.w = material.Specular.a;
	shader->SetValue("SpecularMat", &tempcolor);

	tempcolor.x = material.Emissive.r;
	tempcolor.y = material.Emissive.g;
	tempcolor.z = material.Emissive.b;
	tempcolor.w = material.Emissive.a;
	shader->SetValue("EmmisiveMat", &tempcolor);
}

void Mesh::DrawShadow()
{
	ShaderManager* shaderMgr = ShaderManager::GetInstance();
	// サブセットを描画する(マテリアル数分ループ)
	for (unsigned int i = 0; i < m_numMaterials; i++)
	{
		// パス０開始
		shaderMgr->GetShadowMapShader()->BeginePass(0);
		m_lpMesh->DrawSubset(i);					// サブセットの描画
		shaderMgr->GetShadowMapShader()->CommitChanges();
		shaderMgr->GetShadowMapShader()->EndPass();
	}
}

/**
* @brief シェーダーの設定
*/
void Mesh::SetShader(const ShaderPtr& shader)
{
	m_shader = shader;
}