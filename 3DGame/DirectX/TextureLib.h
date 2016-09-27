/**
* @file TextureLib.h
* @brief テクスチャの読み込みと描画処理
* @author Syota Tachikawa
*/

#ifndef ___TEXTURELIB_H
#define ___TEXTURELIB_H

#include "..\Math\Math.h"

// 画像格納最大数
#define MAX_TEXTURE		270


#pragma region 構造体定義

/**
* @struct TEXTURE
* @brief テクスチャデータ
*/
typedef struct{
	LPDIRECT3DTEXTURE9	pDxTex;
	DWORD				nWidth;
	DWORD				nHeight;
	float				uv1[2], uv2[2];
	bool				used;
}TEXTURE;

/**
* @struct COLOR_POINT
* @brief カラーポイント
*/
typedef struct{
	float	x, y, z;
	float	rhw;
	DWORD	dwColor;
}COLOR_POINT;

/**
* @struct TEXTURE_DATA
* @brief CreateTextureで作成したデータを格納
*
* @note：シェーダーで使用する
*/
typedef struct{
	/// 作成したテクスチャデータ
	LPDIRECT3DTEXTURE9 pTexture;
	/// 作成したサーフェイスデータ
	LPDIRECT3DSURFACE9 pSurface;
}TEXTURE_DATA;

/**
* @struct D2D_POLYGON
* @brief 2Dポリゴン
*/
typedef struct{
	/// 頂点バッファ
	LPDIRECT3DVERTEXBUFFER9 p2DVertexBuffer;
	/// 横幅
	UINT width;
	/// 縦幅
	UINT height;
}D2D_POLYGON;

/**
* @struct D2D_TEXTURE
* @brief 2Dテクスチャ
*/
typedef struct{
	float	x, y, z;
	float	rhw;
	DWORD	uDiffuse;
	float	tu, tv;
}D2D_TEXTURE;

/**
* @struct D3D_TEXTURE
* @brief ビルボードで使用する
*/
typedef struct{
	float	x, y, z;
	DWORD	uDiffuse;
	float	tu, tv;
}D3D_TEXTURE;

/**
* @struct ARGB
* @brief アルファ、赤、緑、青
*/
typedef struct{
	DWORD	alpha;
	DWORD	red;
	DWORD	green;
	DWORD	blue;
}ARGB;

#pragma endregion


/**
* @brief 2Dテクスチャクラス
*/
class Texture2D
{
public:
	/**
	* @brief コンストラクタ
	*/
	Texture2D();

	/**
	* @brief デストラクタ
	*/
	~Texture2D();

	/**
	* @brief テクスチャ読み込み
	*/
	BOOL LoadTexture(string fileName);
	
	/**
	* @brief テクスチャ読み込み
	*/
	LPDIRECT3DTEXTURE9 LoadD3DTexture(string fileName);

	/**
	* @brief ブロック型テクスチャの読み込み
	*/
	bool LoadBlkTexture(string fileName, int sizeX, int sizeY, int numX, int numY, int num, int *data);

	/**
	* @brief テクスチャを作成
	* @param フォーマット
	* @return 作成したテクスチャデータ
	*
	* @note：主にシェーダーで使用する
	*/
	TEXTURE_DATA& CreateTexture(UINT width, UINT height, DWORD Usage, D3DFORMAT format);

	/**
	* @brief 2Dポリゴンを生成する
	* @param width - ポリゴンの横幅
	* @param height - ポリゴンの縦幅
	*
	* @note ： シェーダーで使用する
	*/
	BOOL Create2DPolygon(UINT width, UINT height);

	/**
	* @brief テクスチャを描画(中央に)
	*/
	HRESULT DrawTexture(string fileName, float posX, float posY, float angle = 0.0f, float scale = 1.0f, int alpha = 255, bool add = false, bool reverse = false);
	HRESULT DrawTexture(int index, float posX, float posY, float angle = 0.0f, float scale = 1.0f, int alpha = 255, bool add = false, bool reverse = false);
	/**
	* @brief 画面いっぱいにテクスチャを描画する
	* @param テクスチャデータ
	*
	* @note ： シェーダーで利用する
	*/
	HRESULT DrawTexture(LPDIRECT3DTEXTURE9& texture);

	/**
	* @brief テクスチャを描画(左右引き伸ばし)
	*/
	HRESULT DrawTextureEx(float x1, float y1, float x2, float y2, const LPDIRECT3DBASETEXTURE9& pTex, float z = 0.0f, bool add = false);

	/**
	* @brief テクスチャを描画(ビルボード)
	*/
	HRESULT Draw3DTex(const Matrix& matView, string fileName, const Vector3& pos, const Vector3& angle, float scale, int alpha, bool add = false, bool bAlpha = true);
	HRESULT Draw3DTex(const Matrix& matView, int index, const Vector3& pos, const Vector3& angle, float scale, int alpha, bool add = false);

	/**
	* @brief テクスチャを描画(円柱の側面のみ)
	*/
	HRESULT Draw3DBandTexture(const Matrix& matView, string fileName, const  Vector3& pos, const Vector3& angle, float scale, float upRadius, float downRadius, float height, int alpha);

	/**
	* @brief テクスチャを描画(カラー設定有り)
	*/
	HRESULT DrawTexARGB(string fileName, float posX, float posY, ARGB argb, bool add = true, float angle = 0.0f, float scale = 1.0f, bool reverse = false);
	
	/**
	* @brief 四角形を描画
	*/
	HRESULT DrawBox(float nMinX, float nMinY, float nMaxX, float nMaxY, DWORD color);

	/**
	* @brief 2Dポリゴンを描画
	*/
	HRESULT Draw2DPolygon();

	/**
	* @brief テクスチャの横幅を取得する
	*/
	const DWORD GetWidth(string fileName);

	/**
	* @brief テクスチャの横幅を取得する
	*/
	const DWORD GetWidth(int index);

	/**
	* @brief テクスチャの縦幅を取得する
	*/
	const DWORD GetHeight(string fileName);

	/**
	* @brief テクスチャの縦幅を取得する
	*/
	const DWORD GetHeight(int index);

	/**
	* @brief 2Dポリゴンの横幅を取得する
	*/
	const UINT Get2DPolygonWidth();

	/**
	* @brief 2Dポリゴンの縦幅を取得する
	*/
	const UINT Get2DPolygonHeight();

private:
	/// デバイス
	LPDIRECT3DDEVICE9 m_pDevice;

	/// テクスチャマップ
	map<string, TEXTURE> m_textureMap;

	/// ブロック読み込み時に使用
	int numx, numy;

	/// ブロックテクスチャの配列
	TEXTURE m_tex[MAX_TEXTURE];

	/// ポリゴン
	D2D_POLYGON m_polygon;

	/**
	* @brief 空き配列の検索
	*/
	int FindEmptyArray();
};

#endif