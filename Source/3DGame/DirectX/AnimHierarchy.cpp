#include "AnimHierarchy.h"

//警告抑制
#pragma warning(disable:4996)
#pragma warning(disable:4316)
#pragma warning(disable:4018)

/**
* @brief フレームを作成する
*/
HRESULT Hierachy::CreateFrame(LPCTSTR szName, LPD3DXFRAME *ppNewFrame)
{
	Frame* pFrame;
	*ppNewFrame = NULL;
	//フレームポインタオブジェクト生成
	pFrame = new Frame;
	if (pFrame == NULL){
		return E_OUTOFMEMORY;
	}
	//名前フレームオブジェクトの生成(文字の長さ+1メモリ確保)
	pFrame->Name = new TCHAR[lstrlen(szName) + 1];
	if (!pFrame->Name){
		return E_FAIL;
	}
	strcpy(pFrame->Name, szName);
	//初期化
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->m_combinedTransformationMatrix);
	pFrame->pMeshContainer = NULL;
	pFrame->pFrameSibling = NULL;
	pFrame->pFrameFirstChild = NULL;
	*ppNewFrame = pFrame;
	return S_OK;
}

/**
* @brief メッシュコンテナを作成する
*/
HRESULT Hierachy::CreateMeshContainer(LPCSTR szName, CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppMeshContainer)
{
	// メッシュコンテナー
	MeshContainer *pMeshContainer = NULL;
	*ppMeshContainer = NULL;

	// 面数
	int iFacesAmount;

	// マテリアル数
	int iMaterial;

	// デバイスの設定
	LPDIRECT3DDEVICE9 pDevice = NULL;

	// メッシュ
	LPD3DXMESH pMesh = NULL;
	
	// ボーン数
	DWORD dwBoneNum = 0;
	
	// メッシュデータ
	pMesh = pMeshData->pMesh;
	
	// メッシュコンテナオブジェクトの生成
	pMeshContainer = new MeshContainer;
	
	// メッシュコンテナ作成失敗
	if (pMeshContainer == NULL){ 
		return E_OUTOFMEMORY; 
	}

	ZeroMemory(pMeshContainer, sizeof(MeshContainer));

	// メッシュコンテナ名前
	pMeshContainer->Name = new TCHAR[lstrlen(szName) + 1];
	if (!pMeshContainer->Name){ return E_FAIL; }
	strcpy(pMeshContainer->Name, szName);

	pMesh->GetDevice(&pDevice);
	iFacesAmount = pMesh->GetNumFaces();
	pMeshContainer->MeshData.pMesh = pMesh;
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	// スキン情報
	// スキンメッシュ以外の場合
	if (pSkinInfo == NULL){
		// 参照カウンタを1つ増やす
		pMesh->AddRef();
	}
	// マテリアルデータ
	// メッシュのマテリアル設定
	pMeshContainer->NumMaterials = max(1, NumMaterials);

	// 配列の確保
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->m_ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[iFacesAmount * 3];
	if ((pMeshContainer->pAdjacency == NULL)
		|| (pMeshContainer->pMaterials == NULL)){
		return E_FAIL;
	}
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD)* iFacesAmount * 3);
	memset(pMeshContainer->m_ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9)* pMeshContainer->NumMaterials);
	if (NumMaterials > 0){
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL)* NumMaterials);
		// テクスチャ読み込み
		for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++){
			if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL){
				TCHAR strTexturePath[MAX_PATH];
				strcpy(strTexturePath, pMeshContainer->pMaterials[iMaterial].pTextureFilename);
				if (FAILED(D3DXCreateTextureFromFile(pDevice, strTexturePath,
					&pMeshContainer->m_ppTextures[iMaterial]))){
					pMeshContainer->m_ppTextures[iMaterial] = NULL;
				}
				pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
			}
		}
	}
	else{
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}
	// 当該メッシュがスキン情報を持っている場合（スキンメッシュ固有の処理）
	if (pSkinInfo != NULL){
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();
		dwBoneNum = pSkinInfo->GetNumBones();
		pMeshContainer->m_pBoneOffsetMatrices = new D3DXMATRIX[dwBoneNum];

		for (DWORD i = 0; i < dwBoneNum; i++){
			memcpy(&pMeshContainer->m_pBoneOffsetMatrices[i],
				pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i), sizeof(D3DMATRIX));
		}
		if (FAILED(pMeshContainer->pSkinInfo->ConvertToBlendedMesh(pMesh, NULL, pMeshContainer->pAdjacency,
			NULL, NULL, NULL, &pMeshContainer->m_dwWeight, &pMeshContainer->m_dwBoneNum,
			&pMeshContainer->m_pBoneBuffer, &pMeshContainer->MeshData.pMesh))){
			return E_FAIL;
		}
	}
	// ローカルに生成したメッシュコンテナーを呼び出し側にコピー
	*ppMeshContainer = pMeshContainer;
	// 参照カウンタを増やしたので減らす
	SafeRelease(pDevice);

	// 成功
	return S_OK;
}

/**
* @brief フレームを解放
*/
HRESULT Hierachy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	//名前フレーム配列の解放
	SafeDeleteArray(pFrameToFree->Name);
	//子フレームを再帰解放
	if (pFrameToFree->pFrameFirstChild){
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}
	//親フレームを再帰解放
	if (pFrameToFree->pFrameSibling){
		DestroyFrame(pFrameToFree->pFrameSibling);
	}
	//フレームの解放
	SafeDelete(pFrameToFree);

	// 成功
	return S_OK;
}

/**
* @brief メッシュコンテナを解放
*/
HRESULT Hierachy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	int iMaterial;
	MeshContainer *pMeshContainer = (MeshContainer*)pMeshContainerBase;
	//名前メッシュコンテナを解放
	SafeDeleteArray(pMeshContainer->Name);
	//スキン情報の開放
	SafeRelease(pMeshContainer->pSkinInfo);
	SafeDeleteArray(pMeshContainer->pAdjacency);
	//マテリアルの解放
	SafeDeleteArray(pMeshContainer->pMaterials);
	//ボーン行列の解放
	SafeDeleteArray(pMeshContainer->m_ppBoneMatrix);
	//テクスチャ配列の解放
	for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++){
		SafeRelease(pMeshContainer->m_ppTextures[iMaterial]);
	}
	//テクスチャの解放
	SafeDeleteArray(pMeshContainer->m_ppTextures);
	//メッシュコンテナのメッシュデータ解放
	SafeRelease(pMeshContainer->MeshData.pMesh);
	//ボーンバッファの解放
	SafeRelease(pMeshContainer->m_pBoneBuffer);
	SafeDeleteArray(pMeshContainer->m_pBoneOffsetMatrices);
	//メッシュコンテナの解放
	SafeDelete(pMeshContainer);

	// 成功
	return S_OK;
}
