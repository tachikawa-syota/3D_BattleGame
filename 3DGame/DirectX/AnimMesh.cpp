#include "AnimMesh.h"
#include "Direct3DData.h"

/**
* @brief コンストラクタ
*/
SkinMesh::SkinMesh()
{
	Initialize();
}

/**
* @brief 初期化
*/
void SkinMesh::Initialize()
{
	// デバイスの取得
	m_pDevice = D3DIns->GetDevice();
	m_pFrameRoot = NULL;
	m_pAnimController = NULL;
	m_uNumAnimSet = 0;
	m_pAnimSet = NULL;
	m_alpha = 1.0f;
	m_nNowAnimeNo = 0;
	m_totalShiftTime = 0.0f;
	m_animPeriod = 0.0f;
	m_bChanging = false;
}

/**
* @brief デストラクタ
*/
SkinMesh::~SkinMesh()
{
	FreeAnim(GetpFrameroot());
	DestroyFrame(GetpFrameroot());
	SafeRelease(m_pAnimController);
	SafeDeleteArray(m_pAnimSet);
}

/**
* @brief アニメーション解放
*/
void SkinMesh::FreeAnim(LPD3DXFRAME pFrame)
{
	// メッシュコンテナーの破壊
	if (pFrame->pMeshContainer != NULL){
		this->DestroyMeshContainer(pFrame->pMeshContainer);
	}
	// 親フレームの全ての解放(再帰呼び出し)
	if (pFrame->pFrameSibling != NULL){
		this->FreeAnim(pFrame->pFrameSibling);
	}
	// 子フレームの全ての解放(再帰呼び出し)
	if (pFrame->pFrameFirstChild != NULL){
		this->FreeAnim(pFrame->pFrameFirstChild);
	}
}

/**
* @brief Xファイルからアニメーションメッシュを読み込み作成する
* @param szFileName ファイルの名前
*/
bool SkinMesh::LoadAnimeXMesh(string path, LPSTR xFileName)
{
	string modelName = path + xFileName;

	// Xファイルアニメーションの読み込み
	if (FAILED(D3DXLoadMeshHierarchyFromX(modelName.c_str(), D3DXMESH_MANAGED, m_pDevice,
		this, NULL, &m_pFrameRoot, &m_pAnimController))){
		OutputDebugString("***compile error***\n");
		OutputDebugString((LPCSTR)m_pFrameRoot);
		OutputDebugString((LPCSTR)m_pAnimController);
		OutputDebugString("***compile error***\n");
		return false;
	}
	// ボーン行列の初期化
	AllocateAllBoneMatrices(m_pFrameRoot);
	// アニメーショントラックを得る
	m_uNumAnimSet = m_pAnimController->GetNumAnimationSets();
	m_pAnimSet = new LPD3DXANIMATIONSET[m_uNumAnimSet];
	D3DXTRACK_DESC	TrackDesc;
	TrackDesc.Weight = 1;
	TrackDesc.Speed = 1;
	TrackDesc.Enable = 1;
	// アニメーションの設定
	for (DWORD i = 0; i < m_uNumAnimSet; i++){
		// 変更するトラックを設定
		m_pAnimController->SetTrackDesc(i, &TrackDesc);
		// アニメーションの設定
		m_pAnimController->GetAnimationSet(i, &m_pAnimSet[i]);

		m_pAnimController->SetTrackEnable(i, true);
	}

	return true;
}

/**
* @brief ボーン行列の初期化
*/
HRESULT SkinMesh::AllocateBoneMatrix(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	DWORD boneNum;
	Frame *pFrame = NULL;
	MeshContainer *pMeshContainer = (MeshContainer*)pMeshContainerBase;
	if (pMeshContainer->pSkinInfo == NULL){
		return S_OK;
	}
	boneNum = pMeshContainer->pSkinInfo->GetNumBones();
	pMeshContainer->m_ppBoneMatrix = new D3DXMATRIX*[boneNum];
	for (DWORD i = 0; i < boneNum; i++){
		// 子フレーム(ボーン)のアドレスを検索してpFrameに格納
		pFrame = (Frame*)D3DXFrameFind(m_pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(i));
		// 子フレームがなければ処理終了
		if (pFrame == NULL){
			return E_FAIL;
		}
		pMeshContainer->m_ppBoneMatrix[i] = &pFrame->m_combinedTransformationMatrix;
	}

	return S_OK;
}

/**
* @brief 全てのボーン行列
*/
HRESULT SkinMesh::AllocateAllBoneMatrices(LPD3DXFRAME pFrame)
{
	// 全てのボーン行列(再帰呼び出し)
	if (pFrame->pMeshContainer != NULL){
		if (FAILED(AllocateBoneMatrix(pFrame->pMeshContainer))){
			return E_FAIL;
		}
	}
	// 親フレームの全てのボーン(再帰呼び出し)
	if (pFrame->pFrameSibling != NULL){
		if (FAILED(AllocateAllBoneMatrices(pFrame->pFrameSibling))){
			return E_FAIL;
		}
	}
	// 子フレームの全てのボーン(再帰呼び出し)
	if (pFrame->pFrameFirstChild != NULL){
		if (FAILED(AllocateAllBoneMatrices(pFrame->pFrameFirstChild))){
			return E_FAIL;
		}
	}

	return S_OK;
}

/**
* @brief アニメーションメッシュの描画
*/
void SkinMesh::RenderAnimeXMesh()
{
	// 転送行列設定
	m_pDevice->SetTransform(D3DTS_WORLD, &m_transMatrix);
	// フレーム行列更新
	UpdateFrameMatrices(m_pFrameRoot, &m_transMatrix);
	// 親子関係ワールド行列更新と描画
	DrawFrame(m_pFrameRoot);
}

/**
* @brief アニメーションメッシュの描画(シェーダー)
*/
void SkinMesh::RenderAnimeXMesh(const Matrix& view, const Matrix& proj)
{
	// 転送行列設定
	m_pDevice->SetTransform(D3DTS_WORLD, &m_transMatrix);
	// フレーム行列更新
	UpdateFrameMatrices(m_pFrameRoot, &m_transMatrix);
	// 親子関係ワールド行列更新と描画
	DrawFrame(view, proj, m_pFrameRoot);
}

/**
* @brief 影を描画する
*/
void SkinMesh::DrawShadow(LPD3DXFRAME pFrameBase)
{
//	DWORD k, m;
//	DWORD		dwBlendMatrixNum;
////	DWORD		dwPrevBoneID;
//	UINT		iMatrixIndex;
//	D3DXMATRIX mStack;
//	D3DXMATRIX mStacks[MaxBoneMatrix];
//	LPD3DXBONECOMBINATION pBoneCombination;
//
//	Frame *pFrame = (Frame*)pFrameBase;
//	MeshContainer* pMeshContainer = (MeshContainer*)pFrame->pMeshContainer;
//	if (pMeshContainer == NULL){
//		return;
//	}
//	pBoneCombination = (LPD3DXBONECOMBINATION)pMeshContainer->m_pBoneBuffer->GetBufferPointer();
//
//	// 描画
//	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//	for (unsigned long i = 0; i < pMeshContainer->m_dwBoneNum; ++i)
//	{
//
//		// ボーンの座標変換に必要な行列を準備
//		for (k = 0, m = 0; k < pMeshContainer->m_dwWeight; k++){
//			iMatrixIndex = pBoneCombination[i].BoneId[k];
//			if (iMatrixIndex != UINT_MAX){
//				// ボーンオフセット行列とは、ローカル座標系からボーン座標系への変換行列
//				// 加重平均は、頂点座標'=重み(0.0～1.0)*(頂点座標*ボーンオフセット行列)
//				// を掛かる行列全てで計算した後の合計値で求まる。
//				mStacks[m] = pMeshContainer->m_pBoneOffsetMatrices[iMatrixIndex] *
//					//計算省略のためにボーンオフセット行列にワールド行列を掛けておく
//					(*pMeshContainer->m_ppBoneMatrix[iMatrixIndex]);
//				m_pDevice->SetTransform(D3DTS_WORLDMATRIX(k), &mStack);
//				// 行列スタックに登録
//				mStack = mStacks[m];
//				m++;
//			}
//		}
//	
//		ShaderManager* sdrMgr = ShaderManager::GetInstance();
//		sdrMgr->GetSkinShadowMapShader()->GetEffect()->SetMatrixArray("WStack", mStacks, m);
//		sdrMgr->GetSkinShadowMapShader()->GetEffect()->SetInt("StackNum", dwBlendMatrixNum);
//
//		// パス０開始
//		sdrMgr->GetSkinShadowMapShader()->BeginePass(0);
//		m_lpMesh->DrawSubset(i);					// サブセットの描画
//		sdrMgr->GetSkinShadowMapShader()->CommitChanges();
//		sdrMgr->GetSkinShadowMapShader()->EndPass();
//
		//ShaderManager::GetSkinMeshShadowMapShader()->SetMatrixArray("_W", &selectedCombMat[0], numBlendMatrix);

		//ShaderManager::GetSkinMeshShadowMapShader()->CommitChanges();
		//ShaderManager::GetSkinMeshShadowMapShader()->BeginPass();
		//pMesh->DrawSubset(i);
		//ShaderManager::GetSkinMeshShadowMapShader()->EndPass();
//	}
}

/**
* @brief フレーム更新
* @note 引数pFrameBase(ルートフレーム)、pParentMatrix(ワールド行列)を渡す
*/
void SkinMesh::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	Frame *pFrame = (Frame*)pFrameBase;
	// フレームにある行列からワールド変換行列を生成
	if (pParentMatrix != NULL){
		D3DXMatrixMultiply(&pFrame->m_combinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	}
	else{
		pFrame->m_combinedTransformationMatrix = pFrame->TransformationMatrix;
	}
	// 親フレームの行列をワールド座標行列に変換(再帰呼び出し)
	if (pFrame->pFrameSibling != NULL){
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}
	// 子フレームの行列をワールド座標行列に変換(再帰呼び出し)
	if (pFrame->pFrameFirstChild != NULL){
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->m_combinedTransformationMatrix);
	}
}

/**
* @brief 描画フレーム
*/
void SkinMesh::DrawFrame(LPD3DXFRAME pFrameBase)
{
	Frame* pFrame = (Frame*)pFrameBase;
	MeshContainer* pMeshContainer = (MeshContainer*)pFrame->pMeshContainer;
	// 連結するメッシュがなくなるまで描画
	while (pMeshContainer != NULL){
		RenderMeshContainer(pMeshContainer, pFrame);
		// 次のメッシュにスイッチ
		pMeshContainer = (MeshContainer*)pMeshContainer->pNextMeshContainer;
	}
	// 親フレームを描画
	if (pFrame->pFrameSibling != NULL){
		DrawFrame(pFrame->pFrameSibling);
	}
	// 子フレームを描画
	if (pFrame->pFrameFirstChild != NULL){
		DrawFrame(pFrame->pFrameFirstChild);
	}
}

/**
* @brief 描画フレーム(シェーダー)
*/
void SkinMesh::DrawFrame(const Matrix& view, const Matrix& proj, LPD3DXFRAME pFrameBase)
{
	Frame* pFrame = (Frame*)pFrameBase;
	MeshContainer* pMeshContainer = (MeshContainer*)pFrame->pMeshContainer;
	// 連結するメッシュがなくなるまで描画
	while (pMeshContainer != NULL){
		RenderMeshContainer(view, proj, pMeshContainer, pFrame);
		//次のメッシュにスイッチ
		pMeshContainer = (MeshContainer*)pMeshContainer->pNextMeshContainer;
	}
	// 親フレームを描画
	if (pFrame->pFrameSibling != NULL){
		DrawFrame(view, proj, pFrame->pFrameSibling);
	}
	// 子フレームを描画
	if (pFrame->pFrameFirstChild != NULL){
		DrawFrame(view, proj, pFrame->pFrameFirstChild);
	}
}

/**
* @brief メッシュレンダリングコンテナ
*/
void SkinMesh::RenderMeshContainer(const MeshContainer *pMeshContainer, const Frame *pFrame)
{
	DWORD		dwBlendMatrixNum;
	DWORD		dwPrevBoneID;
	UINT		iMatrixIndex;
	D3DXMATRIX	mStack;
	LPD3DXBONECOMBINATION pBoneCombination;
	// スキンメッシュの場合
	if (pMeshContainer->pSkinInfo != NULL){
		pBoneCombination = (LPD3DXBONECOMBINATION)pMeshContainer->m_pBoneBuffer->GetBufferPointer();
		dwPrevBoneID = UINT_MAX;
		for (DWORD i = 0; i < pMeshContainer->m_dwBoneNum; i++){
			// 頂点ブレンドに使う行列数取得
			dwBlendMatrixNum = 0;
			for (DWORD k = 0; k< pMeshContainer->m_dwWeight; k++){
				if (pBoneCombination[i].BoneId[k] != UINT_MAX){
					dwBlendMatrixNum = k;
				}
			}
			if (dwBlendMatrixNum > MaxBoneMatrix){
				return;
			}
			// 頂点ブレンド使用
			m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, dwBlendMatrixNum);
			// ボーンの座標変換に必要な行列を準備
			for (DWORD k = 0; k < pMeshContainer->m_dwWeight; k++){
				iMatrixIndex = pBoneCombination[i].BoneId[k];
				if (iMatrixIndex != UINT_MAX){
					// ボーンオフセット行列とは、ローカル座標系からボーン座標系への変換行列
					// 加重平均は、頂点座標'=重み(0.0～1.0)*(頂点座標*ボーンオフセット行列)
					// を掛かる行列全てで計算した後の合計値で求まる。
					mStack = pMeshContainer->m_pBoneOffsetMatrices[iMatrixIndex] *
						// 計算省略のためにボーンオフセット行列にワールド行列を掛けておく
						(*pMeshContainer->m_ppBoneMatrix[iMatrixIndex]);
					m_pDevice->SetTransform(D3DTS_WORLDMATRIX(k), &mStack);
				}
			}
			// 固定シェーダ用材質セット
			m_pDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneCombination[i].AttribId].MatD3D);
			// テクスチャセット
			if (pMeshContainer->m_ppTextures[pBoneCombination[i].AttribId] != NULL){
				m_pDevice->SetTexture(0, pMeshContainer->m_ppTextures[pBoneCombination[i].AttribId]);
			}
			else{
				m_pDevice->SetTexture(0, NULL);
			}
			dwPrevBoneID = pBoneCombination[i].AttribId;
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
		// 頂点ブレンド使用終了
		m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	}
	else{
		// 通常メッシュの場合
		m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);
		m_pDevice->SetTransform(D3DTS_WORLD, &pFrame->m_combinedTransformationMatrix);
		for (DWORD i = 0; i < pMeshContainer->NumMaterials; i++){
			m_pDevice->SetMaterial(&pMeshContainer->pMaterials[i].MatD3D);
			if (pMeshContainer->m_ppTextures[i] != NULL){
				m_pDevice->SetTexture(0, pMeshContainer->m_ppTextures[i]);
			}
			else{
				m_pDevice->SetTexture(0, NULL);
			}
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
	}
}

/**
* @brief レンダリングコンテナ(シェーダー)
*/
void SkinMesh::RenderMeshContainer(const Matrix& view, const Matrix& proj, const MeshContainer *pMeshContainer, const Frame *pFrame)
{
	DWORD k, m;
	DWORD dwBlendMatrixNum;
	DWORD dwPrevBoneID;
	UINT iMatrixIndex;
	D3DXMATRIX mStack;
	D3DXMATRIX mStacks[MaxBoneMatrix];
	LPD3DXBONECOMBINATION pBoneCombination;
	//スキンメッシュの場合
	if (pMeshContainer->pSkinInfo != NULL){
		pBoneCombination = (LPD3DXBONECOMBINATION)pMeshContainer->m_pBoneBuffer->GetBufferPointer();
		dwPrevBoneID = UINT_MAX;
		for (DWORD i = 0; i < pMeshContainer->m_dwBoneNum; i++){
			//頂点ブレンドに使う行列数取得
			dwBlendMatrixNum = 0;
			for (k = 0; k < pMeshContainer->m_dwWeight; k++){
				if (pBoneCombination[i].BoneId[k] != UINT_MAX){
					dwBlendMatrixNum = k;
				}
			}
			// 固定シェーダ用の頂点ブレンド有効 引数はブレンドに使う行列数
			m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, dwBlendMatrixNum);
			
			// ボーンの座標変換に必要な行列を準備
			for (k = 0, m = 0; k < pMeshContainer->m_dwWeight; k++){
				iMatrixIndex = pBoneCombination[i].BoneId[k];
				if (iMatrixIndex != UINT_MAX){
					// ボーンオフセット行列とは、ローカル座標系からボーン座標系への変換行列
					// 加重平均は、頂点座標'=重み(0.0～1.0)*(頂点座標*ボーンオフセット行列)
					// を掛かる行列全てで計算した後の合計値で求まる。
					mStacks[m] = pMeshContainer->m_pBoneOffsetMatrices[iMatrixIndex] *
						//計算省略のためにボーンオフセット行列にワールド行列を掛けておく
						(*pMeshContainer->m_ppBoneMatrix[iMatrixIndex]);
					m_pDevice->SetTransform(D3DTS_WORLDMATRIX(k), &mStack);
					// 行列スタックに登録
					mStack = mStacks[m];
					m++;
				}
			}
			//シェーダー開始
			UINT pass = m_shader->Begine();
			Vector3 camera = Vector3(view._41, view._42, view._43);
			m_shader->SetValue("EyePos", camera);
			//シェーダにパラメータセット
			//ローカル-射影変換行列
			m_shader->SetValue("W", m_transMatrix);
			m_shader->GetEffect()->SetMatrix("V", &view);
			m_shader->GetEffect()->SetMatrix("P", &proj);
			m_shader->SetValue("AmbientMat", &Vector4(1.0f, 1.0f, 1.0f, 1));
			m_shader->SetValue("DiffuseMat", &Vector4(0.6f, 0.6f, 0.6f, 1));
			m_shader->GetEffect()->SetMatrixArray("WStack", mStacks, m);
			m_shader->GetEffect()->SetInt("StackNum", dwBlendMatrixNum);
			Matrix mat = m_transMatrix * view *proj;
			m_shader->SetMatrix(&mat);
			//コミットチェンジ
			m_shader->CommitChanges();
			dwPrevBoneID = pBoneCombination[i].AttribId;
			//描画処理をシェーダのBegin-Endで囲むだけ
			for (DWORD k = 0; k < pass; k++){
				//シェーダーパス開始
				m_shader->BeginePass(k);
				//固定シェーダ用材質セット
				m_pDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneCombination[i].AttribId].MatD3D);
				//テクスチャセット
				if (pMeshContainer->m_ppTextures[pBoneCombination[i].AttribId] != NULL){
					m_pDevice->SetTexture(0, pMeshContainer->m_ppTextures[pBoneCombination[i].AttribId]);
				}
	//			else{ m_pDevice->SetTexture(0, NULL); }
				//レンダリング
				pMeshContainer->MeshData.pMesh->DrawSubset(i);
				//シェーダーパス終了
				m_shader->EndPass();
			}
			//シェーダー終了
			m_shader->End();
		}
		//頂点ブレンド使用終了
		m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	}
}

/**
* @brief フレーム更新処理(固定フレーム)
*/
void SkinMesh::UpdateFrame()
{
	// アニメーションタイムの取得
	float time = (float)m_pAnimController->GetTime();
	// デバッグ
	char szTmp[256];
	sprintf_s(szTmp, "animation time=%f\n", time);
	OutputDebugString(szTmp);
	m_animCurWeight += ANIM_SPEED;
	if (m_animCurWeight < m_totalShiftTime){
		float fWeight = m_animCurWeight / m_totalShiftTime;
		// トラックの重み設定
		m_pAnimController->SetTrackWeight(0, fWeight);
		m_pAnimController->SetTrackWeight(1, 1.0f - fWeight);
		m_pAnimController->AdvanceTime(0, NULL);
	}
	else{
		m_animCurWeight = m_totalShiftTime;
		m_pAnimController->SetTrackWeight(0, 1.0f);
		// ミキシングを無効
		m_pAnimController->SetTrackEnable(1, FALSE);
		if (!m_bChanging){
			// ループ無し
			if (time < m_animPeriod){
				m_pAnimController->AdvanceTime(ANIM_SPEED, NULL);
			}
		}
		else{
			// ループ有り
			m_pAnimController->AdvanceTime(ANIM_SPEED, NULL);
		}
	}
}

/**
* @brief フレーム更新処理
* @param アニメーション速度
*/
void SkinMesh::UpdateFrame(float speed)
{
	// アニメーションタイムの取得
	float time = (float)m_pAnimController->GetTime();
	// デバッグ
	char szTmp[256];
	sprintf_s(szTmp, "animation time=%f\n", time);
	OutputDebugString(szTmp);
	m_animCurWeight += speed / 60.0f;
	if (m_animCurWeight < m_totalShiftTime){
		float fWeight = m_animCurWeight / m_totalShiftTime;
		//トラックの重み設定
		m_pAnimController->SetTrackWeight(0, fWeight);
		m_pAnimController->SetTrackWeight(1, 1.0f - fWeight);
		m_pAnimController->AdvanceTime(0, NULL);
	}
	else{
		m_animCurWeight = m_totalShiftTime;
		m_pAnimController->SetTrackWeight(0, 1.0f);
		// ミキシングを無効
		m_pAnimController->SetTrackEnable(1, FALSE);
		if (!m_bChanging){
			// ループ無し
			if (time < m_animPeriod){
				m_pAnimController->AdvanceTime(speed / 60.0f, NULL);
			}
		}
		else{
			// ループ有り
			m_pAnimController->AdvanceTime(speed / 60.0f, NULL);
		}
	}
}

/**
* @brief 名前でパーツ検索再帰処理用
* @param name アニメーション名
*/
bool SkinMesh::_GetPartsMatrixByName(Matrix *pOut, LPD3DXFRAME pFrameBase, LPCSTR szName)
{
	Frame *pFrame = (Frame*)pFrameBase;
	if (strcmp(pFrame->Name, szName) == 0){
		*pOut = pFrame->m_combinedTransformationMatrix;
		return true;
	}
	if (pFrame->pFrameSibling != NULL){
		if (_GetPartsMatrixByName(pOut, pFrame->pFrameSibling, szName)){
			return true;
		}
	}
	if (pFrame->pFrameFirstChild != NULL){
		if (_GetPartsMatrixByName(pOut, pFrame->pFrameFirstChild, szName)){
			return true;
		}
	}
	return false;
}

/**
* @brief 名前でボーンパーツ検索
* @param name アニメーション名
*/
bool SkinMesh::GetPartsMatrixByName(Matrix *pOut, LPCSTR name)
{
	return _GetPartsMatrixByName(pOut, m_pFrameRoot, name);
}

/**
* @brief アニメーション切り替え
* @param dwSetNo アニメーション番号
* @param bLoop アニメループフラグ
*/
void SkinMesh::SetAnimNo(DWORD dwAnimNo, bool bLoop)
{
	// 次のアニメーションがない場合戻す
	if (dwAnimNo >= m_uNumAnimSet) return;
	LPD3DXANIMATIONSET	lpAnimSet = m_pAnimSet[dwAnimNo];
	D3DXTRACK_DESC		TrackDesc;
	if (m_totalShiftTime > 0.0f){
		// 前のアニメのトラック情報をトラック1にコピー
		m_pAnimController->GetTrackDesc(0, &TrackDesc);
		m_pAnimController->SetTrackAnimationSet(1, m_pAnimSet[m_nNowAnimeNo]);
		m_pAnimController->SetTrackDesc(1, &TrackDesc);
		// ミキシングを有効にする
		m_pAnimController->SetTrackEnable(1, TRUE);
		// トラックの重みは0.0f
		m_animCurWeight = 1.0f;
		// トラックの重みは0.0f
	}
	else{ 
		m_animCurWeight = 1.0f;
	}
	// トラック0でアニメーション再生
	ZeroMemory(&TrackDesc, sizeof(TrackDesc));
	TrackDesc.Weight = 1;
	TrackDesc.Speed = 1;
	TrackDesc.Enable = 1;
	m_pAnimController->SetTrackDesc(0, &TrackDesc);
	m_pAnimController->SetTrackAnimationSet(0, lpAnimSet);
	// ミキシングを有効にする
	m_pAnimController->SetTrackEnable(0, TRUE);
	m_pAnimController->ResetTime();
	m_animPeriod = (float)lpAnimSet->GetPeriod();
	// ループかどうかの変更
	m_bChanging = bLoop;
	// 現在のアニメーション登録
	m_nNowAnimeNo = dwAnimNo;
}

/**
* @brief アニメーション名前検索
* @param dwSetNo アニメーション番号
* @param bLoop アニメループフラグ
*/
void SkinMesh::SetAnimName(LPCSTR szName, bool bLoop)
{
	DWORD dwAnimNo = 0;
	for (; dwAnimNo< m_uNumAnimSet; dwAnimNo++){
		// 登録された名前より次の
		if (strcmp(m_pAnimSet[dwAnimNo]->GetName(), szName) == 0){ break; }
	}
	if (dwAnimNo >= m_uNumAnimSet) return;
	// アニメーション設定
	SetAnimNo(dwAnimNo, bLoop);
}

/**
* @brief アルファ値を設定
*/
void SkinMesh::SetAlpha(float alpha)
{
	m_alpha = alpha;
}

/**
* @brief アニメーションのシフト時間を設定
*/
void SkinMesh::SetAnimShiftTime(float fTime)
{
	m_totalShiftTime = fTime;
}

/**
* @brief アニメーションを最後まで再生済みか
*/
bool SkinMesh::IsAnimPeriod()
{
	if (m_bChanging) return false;

	double time = m_pAnimController->GetTime();
	if (time < m_animPeriod) return false;

	return TRUE;
}

/**
* @brief アニメーションコントローラーの取得
*/
LPD3DXANIMATIONCONTROLLER SkinMesh::GetController()
{
	return m_pAnimController; 
}

/**
* @brief アニメーションタイムを取得
*/
float SkinMesh::GetTime()
{
	return (float)m_pAnimController->GetTime();
}

/**
* @brief アニメーション番号を取得
*/
int SkinMesh::GetNowAnimNo()
{
	return m_nNowAnimeNo; 
}

/**
* @brief アルファ値の取得
*/
float SkinMesh::GetAlpha()
{
	return m_alpha; 
}