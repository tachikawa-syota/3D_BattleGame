/**
* @file   AnimHierarchy.h
* @brief  ヒエラルキー
* @author Syota Tachikawa
*/

#ifndef ___ANIMHIERARCHYH_H
#define ___ANIMHIERARCHYH_H

#include "..\Common\CommonLib.h"

/**
* @brief メッシュコンテナ
*/
class MeshContainer : public D3DXMESHCONTAINER
{
public:
	LPDIRECT3DTEXTURE9*	m_ppTextures;
	DWORD				m_dwWeight;
	DWORD				m_dwBoneNum;
	LPD3DXBUFFER		m_pBoneBuffer;
	D3DXMATRIX**		m_ppBoneMatrix;
	D3DXMATRIX*			m_pBoneOffsetMatrices;
};

/**
* @brief Xファイル内のアニメーション階層を読み下してくれるクラスを派生させる。
* @note ID3DXAllocateHierarchyインターフェース：フレーム階層を読み込みおよび破棄を行う
* @note	ID3DXAllocateHierarchyは派生すること想定して設計されている。
*/
class Hierachy : public ID3DXAllocateHierarchy
{
public:
	//コンストラクタ
	Hierachy(){};
	//デストラクタ
	~Hierachy(){};
	//STDMETHOD(FuncName) -->COMマクロ = virtual HRESULT _stdcall FuncName
	STDMETHOD(CreateFrame)(THIS_ LPCSTR, LPD3DXFRAME *);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCTSTR, CONST D3DXMESHDATA*, CONST D3DXMATERIAL*,
		CONST D3DXEFFECTINSTANCE*, DWORD, CONST DWORD *, LPD3DXSKININFO, LPD3DXMESHCONTAINER*);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);
};

/**
* @brief 派生フレーム構造体(それぞれのメッシュ用の最終ワールド行列を追加する）
* @note D3DXFRAMEに格納されるのは親->子への座標変換行列
* @note 子->親への逆方向の変換 = インバースキネマティクス
*/
class Frame : public D3DXFRAME
{
public:
	D3DXMATRIXA16 m_combinedTransformationMatrix;
	D3DXMATRIXA16 m_localCombinedTransformationMatrix;
};

#endif
