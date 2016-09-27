#include "TextureLib.h"
#include "Direct3DData.h"
#include "..\ApplicationSystem\ApplicationData.h"
#include "..\Common\STDXDefine.h"

/**
* @brief コンストラクタ
*/
Texture2D::Texture2D()
{
	// デバイスの取得
	m_pDevice = D3DIns->GetDevice();

	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		m_tex[i].used = false;
	}
}

/**
* @brief デストラクタ
*/
Texture2D::~Texture2D()
{
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		if (m_tex[i].used){
			m_tex[i].pDxTex->Release();
		}
	}
}

#pragma region テクスチャの読み込みと作成メソッド

/**
* @brief テクスチャの読み込み
*/
BOOL Texture2D::LoadTexture(string fileName)
{
	// 既に指定したファイルが読み込まれていたら読み込まない
	auto it = m_textureMap.find(fileName);
	if (it != m_textureMap.end()){
		return FALSE;
	}

	D3DXIMAGE_INFO iiImageInfo;
	TEXTURE texture;
	//「テクスチャオブジェクト」の作成
	if (FAILED(D3DXCreateTextureFromFileEx(m_pDevice, fileName.c_str(), D3DX_DEFAULT, D3DX_DEFAULT, 1, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
		0xff00ff00, &iiImageInfo, NULL, &texture.pDxTex)))
	{
		MessageBox(0, fileName.c_str(), "テクスチャ読み込み失敗", MB_OK);
		return FALSE;
	}

	texture.nWidth = iiImageInfo.Width;
	texture.nHeight = iiImageInfo.Height;

	DWORD x = 1;
	while ((x = x * 2) < texture.nWidth);
	texture.uv1[0] = 0;
	texture.uv1[1] = (float)texture.nWidth / (float)x;
	DWORD y = 1;
	while ((y = y * 2) < texture.nHeight);
	texture.uv2[0] = 0;
	texture.uv2[1] = (float)texture.nHeight / (float)y;

	// マップに登録
	m_textureMap.insert(make_pair(fileName, texture));

	return TRUE;
}

/**
* @brief テクスチャの読み込み
* @param szFileName	- 読み込む画像ファイル
*/
LPDIRECT3DTEXTURE9 Texture2D::LoadD3DTexture(string fileName)
{
	HRESULT hr;

	// 初期化しておく
	LPDIRECT3DTEXTURE9 pTex = nullptr;
	D3DXIMAGE_INFO iiImageInfo;

	// テクスチャをファイルから作成
	hr = D3DXCreateTextureFromFileEx(m_pDevice, fileName.c_str(), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, &iiImageInfo, NULL, &pTex);

	if (FAILED(hr)){
		MessageBox(NULL, fileName.c_str(), "テクスチャ読み込み失敗", MB_OK);
		return NULL;
	}
	return pTex;
}

/**
* @brief テクスチャを作成
* @param フォーマット
* @return 作成したテクスチャデータ
*
* @note：主にシェーダーで使用する
*/
TEXTURE_DATA& Texture2D::CreateTexture(UINT width, UINT height, DWORD Usage, D3DFORMAT format)
{
	// 初期化しておく
	static TEXTURE_DATA data;
	data.pTexture = nullptr;
	data.pSurface = nullptr;

	m_pDevice->CreateTexture(width,
		height,
		1,
		Usage,
		format,
		D3DPOOL_DEFAULT,
		&data.pTexture,
		NULL);
	data.pTexture->GetSurfaceLevel(0, &data.pSurface);

	return data;
}

/**
* @brief テクスチャの読み込み
* @param fileName 読み込むファイル名
* @param sizeX    分割計算したテクスチャの横幅
* @param sizeY    分割計算したテクスチャの縦幅
* @param numX     分割計算したテクスチャの横に並べられた数
* @param numY     分割計算したテクスチャの縦に並べられた数
* @param numMax   分割計算したテクスチャの最大数
* @param *data    バッファ
*/
bool Texture2D::LoadBlkTexture(string fileName, int sizeX, int sizeY, int numX, int numY, int numMax, int *data)
{
	//テクスチャ要素数のデータ確保
	numx = numX;
	numy = numY;
	//未使用の要素を検索
	int i;
	for (i = 0; i < numMax; ++i){
		data[i] = FindEmptyArray();
		if (data[i] == -1) break;
		m_tex[data[i]].used = true;
	}
	if (i != numMax){
		for (int j = 0; j<i; ++j){ m_tex[data[j]].used = false; }
		return false;
	}
	D3DXIMAGE_INFO iiImageInfo;
	//読み込み
	TEXTURE bufTex;
	if (FAILED(D3DXCreateTextureFromFileEx(m_pDevice, fileName.c_str(), D3DX_DEFAULT, D3DX_DEFAULT, 1, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
		0xff00ff00, &iiImageInfo, NULL, &bufTex.pDxTex))){
		MessageBox(0, fileName.c_str(), "テクスチャ読み込み失敗", MB_OK);
		return false;
	}
	//テクスチャサイズ取得
	bufTex.nWidth = iiImageInfo.Width;
	bufTex.nHeight = iiImageInfo.Height;
	//テクスチャ補正
	DWORD x = 1;
	while ((x = x * 2) < bufTex.nWidth);
	bufTex.uv1[0] = 0;
	bufTex.uv1[1] = (float)bufTex.nWidth / (float)x;
	DWORD y = 1;
	while ((y = y * 2) < bufTex.nHeight);
	bufTex.uv2[0] = 0;
	bufTex.uv2[1] = (float)bufTex.nHeight / (float)y;

	float addx = ((float)sizeX / bufTex.nWidth) * bufTex.uv1[1];
	float addy = ((float)sizeY / bufTex.nHeight) * bufTex.uv2[1];

	for (int y = 0; y < numY; ++y)
	{
		for (int x = 0; x < numX; ++x)
		{
			//分割番号を順に取得
			int j = y * numX + x;
			//分割テクスチャ最大数に達するとBreak
			if (j >= numMax) break;
			//データを代入
			m_tex[data[j]].pDxTex = bufTex.pDxTex;
			m_tex[data[j]].nWidth = bufTex.nWidth / numX;
			m_tex[data[j]].nHeight = bufTex.nHeight / numY;

			m_tex[data[j]].uv1[0] = x*addx;
			m_tex[data[j]].uv1[1] = (x + 1)*addx;
			m_tex[data[j]].uv2[0] = y*addy;
			m_tex[data[j]].uv2[1] = (y + 1)*addy;
		}
	}
	return true;
}

/**
* @brief 2Dポリゴンを生成する
* @param width - ポリゴンの横幅
* @param height - ポリゴンの縦幅
*
* @note ： シェーダーで使用する
*/
BOOL Texture2D::Create2DPolygon(UINT width, UINT height)
{
	HRESULT hr;
	//２Ｄポリゴンの作成
	hr = m_pDevice->CreateVertexBuffer(sizeof(D2D_TEXTURE) * 4,
		D3DUSAGE_WRITEONLY,
		D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1, 
		D3DPOOL_MANAGED, 
		&m_polygon.p2DVertexBuffer,
		NULL);

	m_polygon.width = width;
	m_polygon.height = height;

	// 頂点カラー
	DWORD vColor = 0xFFFFFFFF;

	float x = (float)m_polygon.width;
	float y = (float)m_polygon.height;
	float tu = 0.0f + 0.5f / (float)m_polygon.width;
	float tu1 = 1.0f + 0.5f / (float)m_polygon.width;
	float tv = 0.0f + 0.5f / (float)m_polygon.height;
	float tv1 = 1.0f + 0.5f / (float)m_polygon.height;


	D2D_TEXTURE* vtx;
	hr = m_polygon.p2DVertexBuffer->Lock(0, 0, (void**)&vtx, 0);
	if (FAILED(hr))
		return -3;

	//頂点の座標
	vtx[0].x = 0.0f;
	vtx[0].y = 0.0f;            
	vtx[0].z = 0.0f; 
	vtx[0].rhw = 1.0f;
	vtx[1].x = x; 
	vtx[1].y = 0.0f;            
	vtx[1].z = 0.0f;
	vtx[1].rhw = 1.0f;
	vtx[2].x = 0.0f;           
	vtx[2].y = y;
	vtx[2].z = 0.0f; 
	vtx[2].rhw = 1.0f;
	vtx[3].x = x;
	vtx[3].y = y; 
	vtx[3].z = 0.0f;
	vtx[3].rhw = 1.0f;

	//テクセル
	vtx[0].tu = tu; vtx[0].tv = tv;
	vtx[1].tu = tu1; vtx[1].tv = tv;
	vtx[2].tu = tu; vtx[2].tv = tv1;
	vtx[3].tu = tu1; vtx[3].tv = tv1;

	//頂点カラー
	vtx[0].uDiffuse = 0xFFFFFFFF;
	vtx[1].uDiffuse = 0xFFFFFFFF;
	vtx[2].uDiffuse = 0xFFFFFFFF;
	vtx[3].uDiffuse = 0xFFFFFFFF;

	m_polygon.p2DVertexBuffer->Unlock();

	if (FAILED(hr)){
		return FALSE;
	}

	return TRUE;
}

#pragma endregion


#pragma region テクスチャの描画メソッド

/**
* @brief 2Dテクスチャの描画（中心に）
* @param index テクスチャの配列番号
* @param posX 描画位置
* @param posY 描画位置
* @param angle 角度
* @param scale 拡大率
* @param add 加算
* @param reverse 反転
*/
HRESULT Texture2D::DrawTexture(string fileName, float posX, float posY, float angle, float scale, int alpha, bool add, bool reverse)
{
	if (alpha < ALPHA_MIN)
		alpha = ALPHA_MIN;
	else if (alpha > ALPHA_MAX)
		alpha = ALPHA_MAX;
	
	float x = m_textureMap[fileName].nWidth * 0.5f * scale;
	float y = m_textureMap[fileName].nHeight * 0.5f * scale;

	DWORD vColor = D3DCOLOR_ARGB(alpha, alpha, alpha, alpha);


	float uv10, uv11;
	if (reverse){
		uv10 = m_textureMap[fileName].uv1[1];
		uv11 = m_textureMap[fileName].uv1[0];
	}
	else {
		uv10 = m_textureMap[fileName].uv1[0];
		uv11 = m_textureMap[fileName].uv1[1];
	}
	D2D_TEXTURE vPoint[] = {
		{ -x, -y, 0, 1, vColor, uv10, m_textureMap[fileName].uv2[0] }
		, { x, -y, 0, 1, vColor, uv11, m_textureMap[fileName].uv2[0] }
		, { -x, y, 0, 1, vColor, uv10, m_textureMap[fileName].uv2[1] }
		, { x, y, 0, 1, vColor, uv11, m_textureMap[fileName].uv2[1] }
	};
	float RotX = sinf(angle * D3DX_PI / 180.0f);
	float RotY = cosf(angle * D3DX_PI / 180.0f);
	for (int i = 0; i<4; i++){
		D2D_TEXTURE *pv = &vPoint[i];
		float fX = pv->x;
		float fY = pv->y;
		pv->x = (fX*RotY) - (fY*RotX) + posX;
		pv->y = (fX*RotX) + (fY*RotY) + posY;
	}

	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	m_pDevice->SetTexture(0, m_textureMap[fileName].pDxTex);

	if (!add){
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}
	else{
		m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	}

	// 描画
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vPoint, sizeof(D2D_TEXTURE));

	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	
	// 成功
	return S_OK;
}
HRESULT Texture2D::DrawTexture(int index, float posX, float posY, float angle, float scale, int alpha, bool add, bool reverse)
{
	if (alpha < ALPHA_MIN)
		alpha = ALPHA_MIN;
	else if (alpha > ALPHA_MAX)
		alpha = ALPHA_MAX;

	float x = m_tex[index].nWidth * 0.5f * scale;
	float y = m_tex[index].nHeight * 0.5f * scale;

	DWORD vColor = D3DCOLOR_ARGB(alpha, alpha, alpha, alpha);


	float uv10, uv11;
	if (reverse){
		uv10 = m_tex[index].uv1[1];
		uv11 = m_tex[index].uv1[0];
	}
	else {
		uv10 = m_tex[index].uv1[0];
		uv11 = m_tex[index].uv1[1];
	}
	D2D_TEXTURE vPoint[] = {
		{ -x, -y, 0, 1, vColor, uv10, m_tex[index].uv2[0] }
		, { x, -y, 0, 1, vColor, uv11, m_tex[index].uv2[0] }
		, { -x, y, 0, 1, vColor, uv10, m_tex[index].uv2[1] }
		, { x, y, 0, 1, vColor, uv11, m_tex[index].uv2[1] }
	};
	float RotX = sinf(angle * D3DX_PI / 180.0f);
	float RotY = cosf(angle * D3DX_PI / 180.0f);
	for (int i = 0; i<4; i++){
		D2D_TEXTURE *pv = &vPoint[i];
		float fX = pv->x;
		float fY = pv->y;
		pv->x = (fX*RotY) - (fY*RotX) + posX;
		pv->y = (fX*RotX) + (fY*RotY) + posY;
	}

	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	m_pDevice->SetTexture(0, m_tex[index].pDxTex);

	if (!add){
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}
	else{
		m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	// 描画
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vPoint, sizeof(D2D_TEXTURE));

	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// 成功
	return S_OK;
}

/**
* @brief 画面いっぱいにテクスチャを描画する
* @param テクスチャデータ
* @note シェーダーで利用する
*/
HRESULT Texture2D::DrawTexture(LPDIRECT3DTEXTURE9& texture) 
{
	float x = 0.0f, y = 0.0f, Xsize = static_cast<float>(AppIns->GetWindowWidth()), Ysize = static_cast<float>(AppIns->GetWindowHeight());

	D2D_TEXTURE vertex[4];

	for (int i = 0; i <= 3; i++) {
		vertex[i].z = 0.0f;
		vertex[i].rhw = 1.0f;
		vertex[i].uDiffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	vertex[0].x = x;	vertex[0].y = y;
	vertex[1].x = (+Xsize) + x;	vertex[1].y = y;
	vertex[2].x = x;	vertex[2].y = (+Ysize) + y;
	vertex[3].x = (+Xsize) + x;	vertex[3].y = (+Ysize) + y;

	vertex[0].tu = 0; vertex[0].tv = 0;
	vertex[1].tu = 1; vertex[1].tv = 0;
	vertex[2].tu = 0; vertex[2].tv = 1;
	vertex[3].tu = 1; vertex[3].tv = 1;

	//テクスチャのセット
	m_pDevice->SetTexture(0, texture);

	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	m_pDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		2,
		vertex,
		sizeof(D2D_TEXTURE));

	//テクスチャなし
	m_pDevice->SetTexture(0, NULL);

	return S_OK;
}

//射影変換済みとして扱われる頂点情報
typedef struct _CUSTOM_VERTEX_TEXTURE{
	D3DXVECTOR3 pos;                //位置
	float       rhw;                //1.0fを入れる        除算数？
	float       u, v;                //uv座標
}CUSTOM_VERTEX_TEXTURE;

/**
* @brief 2Dテクスチャの描画（(拡大縮小可能)
* @param x1 左上
* @param y1 左上
* @param x2 右下
* @param y2 右下
* @param pTex テクスチャ
*/
HRESULT Texture2D::DrawTextureEx(float x1, float y1, float x2, float y2, const  LPDIRECT3DBASETEXTURE9& pTex, float z, bool add)
{
	float tmp;
	if (x1 > x2){
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}

	if (y1 > y2){
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	D3DXVECTOR3 vPnt[4];

	//右上
	vPnt[0].x = x2;
	vPnt[0].y = y1;
	vPnt[0].z = z;

	//右下
	vPnt[1] = vPnt[0];
	vPnt[1].y = y2;

	//左上
	vPnt[2] = vPnt[0];
	vPnt[2].x = x1;

	//左下
	vPnt[3] = vPnt[1];
	vPnt[3].x = x1;

	CUSTOM_VERTEX_TEXTURE vertex[4];        //頂点

	for (int i = 0; i < 4; i++){
		vertex[i].pos = vPnt[i];// 座標コピー
		vertex[i].rhw = 1.0f;// 射影変換済みとする
	}

	//右上
	vertex[0].u = 1.0f;
	vertex[0].v = 0.0f;

	//右下
	vertex[1].u = 1.0f;
	vertex[1].v = 1.0f;

	//左上
	vertex[2].u = 0.0f;
	vertex[2].v = 0.0f;

	//左下
	vertex[3].u = 0.0f;
	vertex[3].v = 1.0f;
	
	if (!add){
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	else{
		m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	}

	//レンダリング
	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);

	//テクスチャ設定
	m_pDevice->SetTexture(0, pTex);
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(CUSTOM_VERTEX_TEXTURE));

	// 成功
	return S_OK;
}

/**
* @brief 2Dテクスチャを3D描画
*
* @param matView カメラのマトリックス
* @param fileName テクスチャファイルの名前
* @param pos 座標
* @param angle 角度
* @param scale 大きさ
* @param alpha アルファ値
*/
HRESULT Texture2D::Draw3DTex(const Matrix& matView, string fileName, const Vector3& pos, const Vector3& angle, float scale, int alpha, bool add, bool bAlpha)
{
	// α制限
	if (alpha < 0) alpha = 0;
	else if (alpha > 255) alpha = 255;
	// 座標・回転・スケール設定
	D3DXMATRIX mWorld, mRotation, mPosition;
	D3DXMatrixTranslation(&mPosition, pos.x, pos.y, pos.z);
	float RadX = D3DX_PI * angle.x / 180.0f;
	float RadY = D3DX_PI * angle.y / 180.0f;
	float RadZ = D3DX_PI * angle.z / 180.0f;
	D3DXMatrixRotationYawPitchRoll(&mRotation, RadY, RadX, RadZ);
	mWorld = mRotation*mPosition;
	// ビルボード処理
	Matrix mat = matView;
	D3DXMatrixInverse(&mat, NULL, &mat);
	// 前と右をーに（180°回転）
	mat._11 *= -1; mat._12 *= -1; mat._13 *= -1;
	mat._31 *= -1; mat._32 *= -1; mat._33 *= -1;
	// 位置に自分の位置をコピー
	mat._41 = pos.x;
	mat._42 = pos.y;
	mat._43 = pos.z;
	// 行列(=姿勢)コピー
	mWorld = mat;
	m_pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	// テクスチャ座標設定
	float x = m_textureMap[fileName].nWidth  * 0.5f* scale;
	float y = m_textureMap[fileName].nHeight * 0.5f* scale;
	// α設定
	DWORD vColor = D3DCOLOR_ARGB(alpha, alpha, alpha, alpha);
	// テクスチャの頂点設定
	D3D_TEXTURE vPoint[4] = {
		{ -x, -y, 0, vColor,m_textureMap[fileName].uv1[1],m_textureMap[fileName].uv2[1] },
		{ x, -y, 0, vColor,m_textureMap[fileName].uv1[0],m_textureMap[fileName].uv2[1] },
		{ -x, y, 0, vColor,m_textureMap[fileName].uv1[1],m_textureMap[fileName].uv2[0] },
		{ x, y, 0, vColor,m_textureMap[fileName].uv1[0],m_textureMap[fileName].uv2[0] }
	};

	if (!bAlpha){
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		// テクスチャ設定
		m_pDevice->SetTexture(0, m_textureMap[fileName].pDxTex);
		m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vPoint, sizeof(D3D_TEXTURE));
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		return S_OK;
	}

	if (add){
		// レンダーステイトの設定
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		// テクスチャ設定
		m_pDevice->SetTexture(0, m_textureMap[fileName].pDxTex);
		m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vPoint, sizeof(D3D_TEXTURE));
		m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
	else{
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		// テクスチャ設定
		m_pDevice->SetTexture(0, m_textureMap[fileName].pDxTex);
		m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vPoint, sizeof(D3D_TEXTURE));
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	return S_OK;
}

/**
* @brief 2Dテクスチャを3D描画
*
* @param matView カメラのマトリックス
* @param index テクスチャの番号
* @param pos 座標
* @param angle 角度
* @param scale 大きさ
* @param alpha アルファ値
*/
HRESULT Texture2D::Draw3DTex(const Matrix& matView, int index, const Vector3& pos, const Vector3& angle, float scale, int alpha, bool add)
{
	// α制限
	if (alpha < 0) alpha = 0;
	else if (alpha > 255) alpha = 255;
	// 座標・回転・スケール設定
	D3DXMATRIX mWorld, mRotation, mPosition;
	D3DXMatrixTranslation(&mPosition, pos.x, pos.y, pos.z);
	float RadX = D3DX_PI * angle.x / 180.0f;
	float RadY = D3DX_PI * angle.y / 180.0f;
	float RadZ = D3DX_PI * angle.z / 180.0f;
	D3DXMatrixRotationYawPitchRoll(&mRotation, RadY, RadX, RadZ);
	mWorld = mRotation*mPosition;
	// ビルボード処理
	Matrix mat = matView;
	D3DXMatrixInverse(&mat, NULL, &mat);
	// 前と右をーに（180°回転）
	mat._11 *= -1; mat._12 *= -1; mat._13 *= -1;
	mat._31 *= -1; mat._32 *= -1; mat._33 *= -1;
	// 位置に自分の位置をコピー
	mat._41 = pos.x;
	mat._42 = pos.y;
	mat._43 = pos.z;
	// 行列(=姿勢)コピー
	mWorld = mat;
	m_pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	// テクスチャ座標設定
	float x = m_tex[index].nWidth  * 0.5f* scale;
	float y = m_tex[index].nHeight * 0.5f* scale;
	// α設定
	DWORD vColor = D3DCOLOR_ARGB(alpha, alpha, alpha, alpha);
	// テクスチャの頂点設定
	D3D_TEXTURE vPoint[4] = {
		{ -x, -y, 0, vColor, m_tex[index].uv1[1], m_tex[index].uv2[1] },
		{ x, -y, 0, vColor, m_tex[index].uv1[0], m_tex[index].uv2[1] },
		{ -x, y, 0, vColor, m_tex[index].uv1[1], m_tex[index].uv2[0] },
		{ x, y, 0, vColor, m_tex[index].uv1[0], m_tex[index].uv2[0] }
	};

	if (add){
		// レンダーステイトの設定
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		// テクスチャ設定
		m_pDevice->SetTexture(0, m_tex[index].pDxTex);
		m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vPoint, sizeof(D3D_TEXTURE));
		m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
	else{
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		// テクスチャ設定
		m_pDevice->SetTexture(0, m_tex[index].pDxTex);
		m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vPoint, sizeof(D3D_TEXTURE));
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	// 成功
	return S_OK;
}

/**
* @brief テクスチャを描画(円柱の側面のみ)
*/
HRESULT Texture2D::Draw3DBandTexture(const Matrix& matView, string fileName, const Vector3& pos, const Vector3& angle, float scale, float upRadius, float downRadius, float height, int alpha)
{
	DWORD vColor = D3DCOLOR_ARGB(255, 255, 255, alpha);

	D3DXMATRIX mWorld, mRotation, mPosition, mScale;

	D3DXMatrixTranslation(&mPosition, pos.x, pos.y, pos.z);
	float RadX = D3DX_PI * angle.x / 180.0f;
	float RadY = D3DX_PI * angle.y / 180.0f;
	float RadZ = D3DX_PI * angle.z / 180.0f;
	D3DXMatrixRotationYawPitchRoll(&mRotation, RadY, RadX, RadZ);

	mWorld = mRotation*mPosition;

	//ビルボード処理
	Matrix mat = matView;
	D3DXMatrixInverse(&mat, NULL, &mat);
	//	前と右をーに（180°回転）
	mat._11 *= -1; mat._12 *= -1; mat._13 *= -1;
	mat._31 *= -1; mat._32 *= -1; mat._33 *= -1;
	//	位置に自分の位置をコピー
	mat._41 = pos.x;
	mat._42 = pos.y;
	mat._43 = pos.z;
	//	行列(=姿勢)コピー
	mWorld = mat;

	m_pDevice->SetTransform(D3DTS_WORLD, &mWorld);

	//描画
	m_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	// テクスチャを設定する
	m_pDevice->SetTexture(0, m_textureMap[fileName].pDxTex);

	height = height * 0.5f * scale;
	float add = (float)1 / 60.0f;
	float uv1 = 0, uv2 = add;
	for (int i = 0; i<60; ++i){
		int j = -(i * 6);
		float x1 = sin(j * D3DX_PI / 180.0f) * upRadius;
		float z1 = cos(j * D3DX_PI / 180.0f) * upRadius;
		float x2 = sin(j * D3DX_PI / 180.0f) * downRadius;
		float z2 = cos(j * D3DX_PI / 180.0f) * downRadius;
		float x3 = sin((j - 6) * D3DX_PI / 180.0f) * upRadius;
		float z3 = cos((j - 6) * D3DX_PI / 180.0f) * upRadius;
		float x4 = sin((j - 6) * D3DX_PI / 180.0f) * downRadius;
		float z4 = cos((j - 6) * D3DX_PI / 180.0f) * downRadius;

		// 横の面
		D3D_TEXTURE vPoint[4] = {
			{ x1, height, z1, vColor, uv1, 0 }
			, { x2, -height, z2, vColor, uv1, 1 }
			, { x3, height, z3, vColor, uv2, 0 }
			, { x4, -height, z4, vColor, uv2, 1 }
		};

		// テクスチャの位置をずらす
		uv1 += add;
		uv2 += add;

		//レンダーステイトの設定
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vPoint, sizeof(D3D_TEXTURE));

		m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	}

	return S_OK;
}

/**
* @brief 2Dテクスチャの描画（カラーを弄れる）
* @param index テクスチャの配列番号
* @param posX 描画位置
* @param posY 描画位置
* @param argb カラー
* @param add 加算
* @param angle 角度
* @param scale 拡大率
* @param reverse 反転
*/
HRESULT Texture2D::DrawTexARGB(string fileName, float posX, float posY, ARGB argb, bool add, float angle, float scale, bool reverse)
{
	if (argb.alpha < 0)
		argb.alpha = 0;
	else if (argb.alpha > 255)
		argb.alpha = 255;

	float x = m_textureMap[fileName].nWidth * 0.5f * scale;
	float y = m_textureMap[fileName].nHeight * 0.5f * scale;

	DWORD vColor = D3DCOLOR_ARGB(argb.alpha, argb.red, argb.green, argb.blue);

	float uv10, uv11;
	if (reverse){
		uv10 = m_textureMap[fileName].uv1[1];
		uv11 = m_textureMap[fileName].uv1[0];
	}
	else {
		uv10 = m_textureMap[fileName].uv1[0];
		uv11 = m_textureMap[fileName].uv1[1];
	}
	D2D_TEXTURE vPoint[] = {
		{ -x, -y, 0, 1, vColor, uv10, m_textureMap[fileName].uv2[0] }
		, { x, -y, 0, 1, vColor, uv11, m_textureMap[fileName].uv2[0] }
		, { -x, y, 0, 1, vColor, uv10, m_textureMap[fileName].uv2[1] }
		, { x, y, 0, 1, vColor, uv11, m_textureMap[fileName].uv2[1] }
	};
	float RotX = sinf(angle * D3DX_PI / 180.0f);
	float RotY = cosf(angle * D3DX_PI / 180.0f);
	for (int i = 0; i<4; i++){
		D2D_TEXTURE *pv = &vPoint[i];
		float fX = pv->x;
		float fY = pv->y;
		pv->x = (fX*RotY) - (fY*RotX) + posX;
		pv->y = (fX*RotX) + (fY*RotY) + posY;
	}

	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	m_pDevice->SetTexture(0, m_textureMap[fileName].pDxTex);

	if (!add){
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	else{
		m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vPoint, sizeof(D2D_TEXTURE));

	// 成功
	return S_OK;
}

#pragma endregion


#pragma region ポリゴンの描画メソッド

/**
* 四角形の描画(スクリーン座標)
*
* @param nMinX	四角形左上X座標
* @param nMinY	四角形左上Y座標
* @param nMaxX	四角形右下X座標
* @param nMaxY	四角形右下Y座標
* @param color	色
*/
HRESULT Texture2D::DrawBox(float nMinX, float nMinY, float nMaxX, float nMaxY, DWORD color)
{
	COLOR_POINT vPoint[] = {
		{ nMinX, nMinY, 0.0f, 1.0f, color }
		, { nMaxX, nMinY, 0.0f, 1.0f, color }
		, { nMinX, nMaxY, 0.0f, 1.0f, color }
		, { nMaxX, nMaxY, 0.0f, 1.0f, color }
	};

	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_pDevice->SetTexture(0, NULL);
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vPoint, sizeof(COLOR_POINT));

	// 成功
	return S_OK;
}

/**
* @brief 2Dポリゴンを描画
*/
HRESULT Texture2D::Draw2DPolygon()
{
	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	m_pDevice->SetStreamSource(0, m_polygon.p2DVertexBuffer, 0, sizeof(D2D_TEXTURE));
	// ▲ポリゴンを２枚使用
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	return S_OK;
}

#pragma endregion


/**
* @brief テクスチャの横幅を取得する
*/
const DWORD Texture2D::GetWidth(string fileName)
{
	return m_textureMap[fileName].nWidth;
}

/**
* @brief テクスチャの横幅を取得する
*/
const DWORD Texture2D::GetWidth(int index)
{
	return m_tex[index].nWidth;
}

/**
* @brief テクスチャの縦幅を取得する
*/
const DWORD Texture2D::GetHeight(string fileName)
{
	return m_textureMap[fileName].nHeight;
}

/**
* @brief テクスチャの縦幅を取得する
*/
const DWORD Texture2D::GetHeight(int index)
{
	return m_tex[index].nHeight;
}

/**
* @brief 2Dポリゴンの横幅を取得する
*/
const UINT Texture2D::Get2DPolygonWidth()
{
	return m_polygon.width;
}

/**
* @brief 2Dポリゴンの縦幅を取得する
*/
const UINT Texture2D::Get2DPolygonHeight()
{
	return m_polygon.height;
}

#pragma region private関数

/**
* @brief 空き配列の検索
*/
int Texture2D::FindEmptyArray()
{
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		if (!m_tex[i].used){
			return i;
		}
	}
	return -1;
}

#pragma endregion