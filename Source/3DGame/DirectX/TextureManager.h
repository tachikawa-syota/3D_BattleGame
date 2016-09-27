/**
* @file   TextureManager.h
* @brief  2Dテクスチャを管理する
* @author Syota Tachikawa
*/

#ifndef ___TEXTUREMANAGER_H
#define ___TEXTUREMANAGER_H

#include "TextureLib.h"

/**
* @brief テクスチャを管理するクラス
*/
class TextureManager
{
private:
	/// コンストラクタ(シングルトン)
	TextureManager(){
		m_p2Dtex = nullptr;
	};

	/// 2Dテクスチャ
	Texture2D* m_p2Dtex;

	/// デバイス
	LPDIRECT3DDEVICE9 m_device;

public:
	/**
	* @brief ゲットインスタンス
	*/
	static TextureManager* GetInstance(){
		static TextureManager singleton;
		return &singleton;
	}
	//デストラクタ
	~TextureManager(){};

	/**
	* @brief 初期化
	*/
	HRESULT Initialize();


#pragma region 描画メソッド

	/**
	* @brief テクスチャを中心に描画する
	* @param fileName - ファイルの名前
	* @param posX - X座標の指定
	* @param posY - Y座標の指定
	* @param angle - 角度
	* @param scale - 大きさ
	* @param alpha - アルファ値
	* @param add - 加算合成の有無
	* @param reverse - 反転して描画するか
	*
	* @note 使い方："fileName"に"LoadTexture"で読み込んだデータを指定してください
	*				テクスチャを回転して描画したい場合は"angle"に角度を指定してください
	*				角度・大きさ・加算・反転に関してはデフォルトで引数を指定しています
	*/
	HRESULT DrawTexture(string fileName, float posX, float posY, float angle = 0.0f, float scale = 1.0f, int alpha = 255, bool add = false, bool reverse = false){
		return m_p2Dtex->DrawTexture(fileName, posX, posY, angle, scale, alpha, add, reverse);
	}
	/**
	* @brief ブロック型で読み込んだテクスチャを描画する
	*/
	HRESULT DrawTexture(int index, float posX, float posY, float angle = 0.0f, float scale = 1.0f, int alpha = 255, bool add = false, bool reverse = false){
		return m_p2Dtex->DrawTexture(index, posX, posY, angle, scale, alpha, add, reverse);
	}
	/**
	* @brief 画面いっぱいにテクスチャを描画する
	* @param テクスチャデータ
	* @note シェーダーで利用する
	*/
	HRESULT DrawTexture(LPDIRECT3DTEXTURE9& texture){
		return m_p2Dtex->DrawTexture(texture);
	}

	/**
	* @brief テクスチャを中心に描画する
	* @param x1 - 左端X座標
	* @param y1 - 左端Y座標
	* @param x2 - 右端X座標
	* @param y2 - 右端Y座標
	* @param pTex - テクスチャデータ（D3DLoadTextureで読み込んだやつ）
	* @param z	- z軸
	* @param add - 加算合成の有無
	*
	* @note 使い方："pTex"に"D3DLoadTexture"で読み込んだデータを指定してください
	*/
	HRESULT DrawTextureEx(float x1, float y1, float x2, float y2, const LPDIRECT3DBASETEXTURE9& pTex, float z = 0.0f, bool add = false){
		return m_p2Dtex->DrawTextureEx(x1, y1, x2, y2, pTex, z, add);
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
	*
	* @note - 使い方：この関数は単体で読み込んだテクスチャで使用する。
	*                 "fileName"にはテクスチャの名前を指定してください。
	*                 textureMapに格納されたテクスチャが描画されます。
	*/
	HRESULT Draw3DTexture(const Matrix& matView, string fileName, const Vector3& pos, const Vector3& angle, float scale, int alpha, bool add = false, bool bAlpha = true) {
		return m_p2Dtex->Draw3DTex(matView, fileName, pos, angle, scale, alpha, add, bAlpha);
	}

	/**
	* @brief 2Dテクスチャを3D描画
	* @param matView カメラのビュー行列
	* @param index テクスチャの番号
	* @param pos 座標
	* @param angle 角度
	* @param scale 大きさ
	* @param alpha アルファ値
	*
	* @note - 使い方：この関数はブロック型で読み込んだテクスチャで使用する。
	*                 "index"にはテクスチャの番号を指定してください。
	*/
	HRESULT Draw3DTexture(const Matrix& matView, int index, const Vector3& pos, const Vector3& angle, float scale, int alpha, bool add = false){
		return m_p2Dtex->Draw3DTex(matView, index, pos, angle, scale, alpha, add);
	}

	/**
	* @brief テクスチャを描画(円柱の側面のみ)
	* @param matView カメラのマトリックス
	* @param fileName テクスチャファイルの名前
	* @param pos 座標
	* @param angle 角度
	* @param scale 大きさ
	* @param radius 半径
	* @param height 高さ
	* @param alpha アルファ値
	*/
	HRESULT Draw3DBandTexture(const Matrix& matView, string fileName, const Vector3& pos, const Vector3& angle, float scale, float upRadius, float downRadius, float height, int alpha){
		return m_p2Dtex->Draw3DBandTexture(matView, fileName, pos, angle, scale, upRadius, downRadius, height, alpha);
	}

	/**
	* @brief テクスチャを描画する（カラーの変更が可能）
	* @param fileName - ファイルの名前
	* @param posX - X座標の指定
	* @param posY - Y座標の指定
	* @param argb - カラーデータの指定
	* @param add - 加算合成の有無
	* @param angle - 角度
	* @param scale - 大きさ
	* @param reverse - 反転して描画するか
	*/
	HRESULT DrawTexARGB(string fileName, float posX, float posY, ARGB argb, bool add = true, float angle = 0.0f, float scale = 1.0f, bool reverse = false){
		return m_p2Dtex->DrawTexARGB(fileName, posX, posY, argb, add, angle, scale, reverse);
	}

	/**
	* @brief 四角形を描画する
	*/
	HRESULT DrawBox(float nMinX, float nMinY, float nMaxX, float nMaxY, DWORD color){
		return m_p2Dtex->DrawBox(nMinX, nMinY, nMaxX, nMaxY, color);
	}

	/**
	* @brief 2Dポリゴンを描画
	*/
	HRESULT Draw2DPolygon(){
		return m_p2Dtex->Draw2DPolygon();
	}


#pragma endregion


#pragma region 読み込みと作成メソッド

	/**
	* @brief テクスチャの読み込み
	* @param szFileName - テクスチャファイルの名前
	* @return テクスチャの番号
	*/
	BOOL LoadTexture(string fileName){
		return m_p2Dtex->LoadTexture(fileName);
	}

	/**
	* @brief D3D型のテクスチャの読み込み
	* @return 読み込んだテクスチャ
	*
	* @note：LPDIRECT3DTEXTURE9型でテクスチャを読み込みます
	*/
	LPDIRECT3DTEXTURE9 D3LoadTexture(string fileName){
		return m_p2Dtex->LoadD3DTexture(fileName);
	}

	/**
	* @brief ブロック型のテクスチャの読み込み
	* @param szFileName - テクスチャファイルの名前
	* @param sizeX - 分割した時の1枚分のテクスチャの横幅
	* @param sizeY - 分割した時の1枚分のテクスチャの縦幅
	* @param numX - 横に並べられているテクスチャの枚数
	* @param numY - 縦に並べられているテクスチャの枚数
	* @param numMax - テクスチャの総枚数
	* @param *data - 読み込んだテクスチャを格納する配列ポインタ
	* @return テクスチャの番号
	*
	* @note 使い方："sizeX,sizeY"に1枚分のテクスチャの幅を指定してください
	*				テクスチャのサイズが全て同じサイズのモノでないと読み込みが上手くいきません（バラバラでも読み込めるようにいずれ対応させたい）
	*				"numMax"は"numX"に入れた値 * numYに入れた値"にしてください
	*				*dataの配列サイズとテクスチャの枚数が異なるとバグの要因になるので注意
	*/
	int LoadBlkTexture(string fileName, int sizeX, int sizeY, int numX, int numY, int numMax, int *data){
		return m_p2Dtex->LoadBlkTexture(fileName, sizeX, sizeY, numX, numY, numMax, data);
	}

	/**
	* @brief テクスチャを作成
	* @return 作成したテクスチャデータ
	*
	* @note：主にシェーダーで使用します
	*/
	TEXTURE_DATA& CreateTexture(UINT width, UINT height, DWORD Usage = D3DUSAGE_RENDERTARGET, D3DFORMAT format = D3DFMT_A8R8G8B8){
		return m_p2Dtex->CreateTexture(width, height, Usage, format);
	}

	/**
	* @brief 2Dポリゴンを生成する
	* @param width - ポリゴンの横幅
	* @param height - ポリゴンの縦幅
	*
	* @note 使い方：作成したいポリゴンの幅を引数に持たせてください
	*               ※シェーダーで使用
	*/
	BOOL Create2DPolygon(UINT width, UINT height){
		return m_p2Dtex->Create2DPolygon(width, height);
	}

#pragma endregion


	/**
	* @brief テクスチャの横幅を取得する
	*/
	const DWORD GetWidth(string fileName){
		return m_p2Dtex->GetWidth(fileName);
	}

	/**
	* @brief テクスチャの横幅を取得する
	*/
	const DWORD GetWidth(int index){
		return m_p2Dtex->GetWidth(index);
	}

	/**
	* @brief テクスチャの縦幅を取得する
	*/
	const DWORD GetHeight(string fileName){
		return m_p2Dtex->GetHeight(fileName);
	}

	/**
	* @brief テクスチャの縦幅を取得する
	*/
	const DWORD GetHeight(int index){
		return m_p2Dtex->GetHeight(index);
	}

	/**
	* @brief 2Dポリゴンの横幅を取得する
	*/
	const UINT Get2DPolygonWidth(){
		return m_p2Dtex->Get2DPolygonWidth();
	}

	/**
	* @brief 2Dポリゴンの縦幅を取得する
	*/
	const UINT Get2DPolygonHeight(){
		return m_p2Dtex->Get2DPolygonHeight();
	}
};

#endif