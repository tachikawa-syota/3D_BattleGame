#include "AnimMesh.h"
#include "Direct3DData.h"

/**
* @brief �R���X�g���N�^
*/
SkinMesh::SkinMesh()
{
	Initialize();
}

/**
* @brief ������
*/
void SkinMesh::Initialize()
{
	// �f�o�C�X�̎擾
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
* @brief �f�X�g���N�^
*/
SkinMesh::~SkinMesh()
{
	FreeAnim(GetpFrameroot());
	DestroyFrame(GetpFrameroot());
	SafeRelease(m_pAnimController);
	SafeDeleteArray(m_pAnimSet);
}

/**
* @brief �A�j���[�V�������
*/
void SkinMesh::FreeAnim(LPD3DXFRAME pFrame)
{
	// ���b�V���R���e�i�[�̔j��
	if (pFrame->pMeshContainer != NULL){
		this->DestroyMeshContainer(pFrame->pMeshContainer);
	}
	// �e�t���[���̑S�Ẳ��(�ċA�Ăяo��)
	if (pFrame->pFrameSibling != NULL){
		this->FreeAnim(pFrame->pFrameSibling);
	}
	// �q�t���[���̑S�Ẳ��(�ċA�Ăяo��)
	if (pFrame->pFrameFirstChild != NULL){
		this->FreeAnim(pFrame->pFrameFirstChild);
	}
}

/**
* @brief X�t�@�C������A�j���[�V�������b�V����ǂݍ��ݍ쐬����
* @param szFileName �t�@�C���̖��O
*/
bool SkinMesh::LoadAnimeXMesh(string path, LPSTR xFileName)
{
	string modelName = path + xFileName;

	// X�t�@�C���A�j���[�V�����̓ǂݍ���
	if (FAILED(D3DXLoadMeshHierarchyFromX(modelName.c_str(), D3DXMESH_MANAGED, m_pDevice,
		this, NULL, &m_pFrameRoot, &m_pAnimController))){
		OutputDebugString("***compile error***\n");
		OutputDebugString((LPCSTR)m_pFrameRoot);
		OutputDebugString((LPCSTR)m_pAnimController);
		OutputDebugString("***compile error***\n");
		return false;
	}
	// �{�[���s��̏�����
	AllocateAllBoneMatrices(m_pFrameRoot);
	// �A�j���[�V�����g���b�N�𓾂�
	m_uNumAnimSet = m_pAnimController->GetNumAnimationSets();
	m_pAnimSet = new LPD3DXANIMATIONSET[m_uNumAnimSet];
	D3DXTRACK_DESC	TrackDesc;
	TrackDesc.Weight = 1;
	TrackDesc.Speed = 1;
	TrackDesc.Enable = 1;
	// �A�j���[�V�����̐ݒ�
	for (DWORD i = 0; i < m_uNumAnimSet; i++){
		// �ύX����g���b�N��ݒ�
		m_pAnimController->SetTrackDesc(i, &TrackDesc);
		// �A�j���[�V�����̐ݒ�
		m_pAnimController->GetAnimationSet(i, &m_pAnimSet[i]);

		m_pAnimController->SetTrackEnable(i, true);
	}

	return true;
}

/**
* @brief �{�[���s��̏�����
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
		// �q�t���[��(�{�[��)�̃A�h���X����������pFrame�Ɋi�[
		pFrame = (Frame*)D3DXFrameFind(m_pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(i));
		// �q�t���[�����Ȃ���Ώ����I��
		if (pFrame == NULL){
			return E_FAIL;
		}
		pMeshContainer->m_ppBoneMatrix[i] = &pFrame->m_combinedTransformationMatrix;
	}

	return S_OK;
}

/**
* @brief �S�Ẵ{�[���s��
*/
HRESULT SkinMesh::AllocateAllBoneMatrices(LPD3DXFRAME pFrame)
{
	// �S�Ẵ{�[���s��(�ċA�Ăяo��)
	if (pFrame->pMeshContainer != NULL){
		if (FAILED(AllocateBoneMatrix(pFrame->pMeshContainer))){
			return E_FAIL;
		}
	}
	// �e�t���[���̑S�Ẵ{�[��(�ċA�Ăяo��)
	if (pFrame->pFrameSibling != NULL){
		if (FAILED(AllocateAllBoneMatrices(pFrame->pFrameSibling))){
			return E_FAIL;
		}
	}
	// �q�t���[���̑S�Ẵ{�[��(�ċA�Ăяo��)
	if (pFrame->pFrameFirstChild != NULL){
		if (FAILED(AllocateAllBoneMatrices(pFrame->pFrameFirstChild))){
			return E_FAIL;
		}
	}

	return S_OK;
}

/**
* @brief �A�j���[�V�������b�V���̕`��
*/
void SkinMesh::RenderAnimeXMesh()
{
	// �]���s��ݒ�
	m_pDevice->SetTransform(D3DTS_WORLD, &m_transMatrix);
	// �t���[���s��X�V
	UpdateFrameMatrices(m_pFrameRoot, &m_transMatrix);
	// �e�q�֌W���[���h�s��X�V�ƕ`��
	DrawFrame(m_pFrameRoot);
}

/**
* @brief �A�j���[�V�������b�V���̕`��(�V�F�[�_�[)
*/
void SkinMesh::RenderAnimeXMesh(const Matrix& view, const Matrix& proj)
{
	// �]���s��ݒ�
	m_pDevice->SetTransform(D3DTS_WORLD, &m_transMatrix);
	// �t���[���s��X�V
	UpdateFrameMatrices(m_pFrameRoot, &m_transMatrix);
	// �e�q�֌W���[���h�s��X�V�ƕ`��
	DrawFrame(view, proj, m_pFrameRoot);
}

/**
* @brief �e��`�悷��
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
//	// �`��
//	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//	for (unsigned long i = 0; i < pMeshContainer->m_dwBoneNum; ++i)
//	{
//
//		// �{�[���̍��W�ϊ��ɕK�v�ȍs�������
//		for (k = 0, m = 0; k < pMeshContainer->m_dwWeight; k++){
//			iMatrixIndex = pBoneCombination[i].BoneId[k];
//			if (iMatrixIndex != UINT_MAX){
//				// �{�[���I�t�Z�b�g�s��Ƃ́A���[�J�����W�n����{�[�����W�n�ւ̕ϊ��s��
//				// ���d���ς́A���_���W'=�d��(0.0�`1.0)*(���_���W*�{�[���I�t�Z�b�g�s��)
//				// ���|����s��S�ĂŌv�Z������̍��v�l�ŋ��܂�B
//				mStacks[m] = pMeshContainer->m_pBoneOffsetMatrices[iMatrixIndex] *
//					//�v�Z�ȗ��̂��߂Ƀ{�[���I�t�Z�b�g�s��Ƀ��[���h�s����|���Ă���
//					(*pMeshContainer->m_ppBoneMatrix[iMatrixIndex]);
//				m_pDevice->SetTransform(D3DTS_WORLDMATRIX(k), &mStack);
//				// �s��X�^�b�N�ɓo�^
//				mStack = mStacks[m];
//				m++;
//			}
//		}
//	
//		ShaderManager* sdrMgr = ShaderManager::GetInstance();
//		sdrMgr->GetSkinShadowMapShader()->GetEffect()->SetMatrixArray("WStack", mStacks, m);
//		sdrMgr->GetSkinShadowMapShader()->GetEffect()->SetInt("StackNum", dwBlendMatrixNum);
//
//		// �p�X�O�J�n
//		sdrMgr->GetSkinShadowMapShader()->BeginePass(0);
//		m_lpMesh->DrawSubset(i);					// �T�u�Z�b�g�̕`��
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
* @brief �t���[���X�V
* @note ����pFrameBase(���[�g�t���[��)�ApParentMatrix(���[���h�s��)��n��
*/
void SkinMesh::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	Frame *pFrame = (Frame*)pFrameBase;
	// �t���[���ɂ���s�񂩂烏�[���h�ϊ��s��𐶐�
	if (pParentMatrix != NULL){
		D3DXMatrixMultiply(&pFrame->m_combinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	}
	else{
		pFrame->m_combinedTransformationMatrix = pFrame->TransformationMatrix;
	}
	// �e�t���[���̍s������[���h���W�s��ɕϊ�(�ċA�Ăяo��)
	if (pFrame->pFrameSibling != NULL){
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}
	// �q�t���[���̍s������[���h���W�s��ɕϊ�(�ċA�Ăяo��)
	if (pFrame->pFrameFirstChild != NULL){
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->m_combinedTransformationMatrix);
	}
}

/**
* @brief �`��t���[��
*/
void SkinMesh::DrawFrame(LPD3DXFRAME pFrameBase)
{
	Frame* pFrame = (Frame*)pFrameBase;
	MeshContainer* pMeshContainer = (MeshContainer*)pFrame->pMeshContainer;
	// �A�����郁�b�V�����Ȃ��Ȃ�܂ŕ`��
	while (pMeshContainer != NULL){
		RenderMeshContainer(pMeshContainer, pFrame);
		// ���̃��b�V���ɃX�C�b�`
		pMeshContainer = (MeshContainer*)pMeshContainer->pNextMeshContainer;
	}
	// �e�t���[����`��
	if (pFrame->pFrameSibling != NULL){
		DrawFrame(pFrame->pFrameSibling);
	}
	// �q�t���[����`��
	if (pFrame->pFrameFirstChild != NULL){
		DrawFrame(pFrame->pFrameFirstChild);
	}
}

/**
* @brief �`��t���[��(�V�F�[�_�[)
*/
void SkinMesh::DrawFrame(const Matrix& view, const Matrix& proj, LPD3DXFRAME pFrameBase)
{
	Frame* pFrame = (Frame*)pFrameBase;
	MeshContainer* pMeshContainer = (MeshContainer*)pFrame->pMeshContainer;
	// �A�����郁�b�V�����Ȃ��Ȃ�܂ŕ`��
	while (pMeshContainer != NULL){
		RenderMeshContainer(view, proj, pMeshContainer, pFrame);
		//���̃��b�V���ɃX�C�b�`
		pMeshContainer = (MeshContainer*)pMeshContainer->pNextMeshContainer;
	}
	// �e�t���[����`��
	if (pFrame->pFrameSibling != NULL){
		DrawFrame(view, proj, pFrame->pFrameSibling);
	}
	// �q�t���[����`��
	if (pFrame->pFrameFirstChild != NULL){
		DrawFrame(view, proj, pFrame->pFrameFirstChild);
	}
}

/**
* @brief ���b�V�������_�����O�R���e�i
*/
void SkinMesh::RenderMeshContainer(const MeshContainer *pMeshContainer, const Frame *pFrame)
{
	DWORD		dwBlendMatrixNum;
	DWORD		dwPrevBoneID;
	UINT		iMatrixIndex;
	D3DXMATRIX	mStack;
	LPD3DXBONECOMBINATION pBoneCombination;
	// �X�L�����b�V���̏ꍇ
	if (pMeshContainer->pSkinInfo != NULL){
		pBoneCombination = (LPD3DXBONECOMBINATION)pMeshContainer->m_pBoneBuffer->GetBufferPointer();
		dwPrevBoneID = UINT_MAX;
		for (DWORD i = 0; i < pMeshContainer->m_dwBoneNum; i++){
			// ���_�u�����h�Ɏg���s�񐔎擾
			dwBlendMatrixNum = 0;
			for (DWORD k = 0; k< pMeshContainer->m_dwWeight; k++){
				if (pBoneCombination[i].BoneId[k] != UINT_MAX){
					dwBlendMatrixNum = k;
				}
			}
			if (dwBlendMatrixNum > MaxBoneMatrix){
				return;
			}
			// ���_�u�����h�g�p
			m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, dwBlendMatrixNum);
			// �{�[���̍��W�ϊ��ɕK�v�ȍs�������
			for (DWORD k = 0; k < pMeshContainer->m_dwWeight; k++){
				iMatrixIndex = pBoneCombination[i].BoneId[k];
				if (iMatrixIndex != UINT_MAX){
					// �{�[���I�t�Z�b�g�s��Ƃ́A���[�J�����W�n����{�[�����W�n�ւ̕ϊ��s��
					// ���d���ς́A���_���W'=�d��(0.0�`1.0)*(���_���W*�{�[���I�t�Z�b�g�s��)
					// ���|����s��S�ĂŌv�Z������̍��v�l�ŋ��܂�B
					mStack = pMeshContainer->m_pBoneOffsetMatrices[iMatrixIndex] *
						// �v�Z�ȗ��̂��߂Ƀ{�[���I�t�Z�b�g�s��Ƀ��[���h�s����|���Ă���
						(*pMeshContainer->m_ppBoneMatrix[iMatrixIndex]);
					m_pDevice->SetTransform(D3DTS_WORLDMATRIX(k), &mStack);
				}
			}
			// �Œ�V�F�[�_�p�ގ��Z�b�g
			m_pDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneCombination[i].AttribId].MatD3D);
			// �e�N�X�`���Z�b�g
			if (pMeshContainer->m_ppTextures[pBoneCombination[i].AttribId] != NULL){
				m_pDevice->SetTexture(0, pMeshContainer->m_ppTextures[pBoneCombination[i].AttribId]);
			}
			else{
				m_pDevice->SetTexture(0, NULL);
			}
			dwPrevBoneID = pBoneCombination[i].AttribId;
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
		// ���_�u�����h�g�p�I��
		m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	}
	else{
		// �ʏ탁�b�V���̏ꍇ
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
* @brief �����_�����O�R���e�i(�V�F�[�_�[)
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
	//�X�L�����b�V���̏ꍇ
	if (pMeshContainer->pSkinInfo != NULL){
		pBoneCombination = (LPD3DXBONECOMBINATION)pMeshContainer->m_pBoneBuffer->GetBufferPointer();
		dwPrevBoneID = UINT_MAX;
		for (DWORD i = 0; i < pMeshContainer->m_dwBoneNum; i++){
			//���_�u�����h�Ɏg���s�񐔎擾
			dwBlendMatrixNum = 0;
			for (k = 0; k < pMeshContainer->m_dwWeight; k++){
				if (pBoneCombination[i].BoneId[k] != UINT_MAX){
					dwBlendMatrixNum = k;
				}
			}
			// �Œ�V�F�[�_�p�̒��_�u�����h�L�� �����̓u�����h�Ɏg���s��
			m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, dwBlendMatrixNum);
			
			// �{�[���̍��W�ϊ��ɕK�v�ȍs�������
			for (k = 0, m = 0; k < pMeshContainer->m_dwWeight; k++){
				iMatrixIndex = pBoneCombination[i].BoneId[k];
				if (iMatrixIndex != UINT_MAX){
					// �{�[���I�t�Z�b�g�s��Ƃ́A���[�J�����W�n����{�[�����W�n�ւ̕ϊ��s��
					// ���d���ς́A���_���W'=�d��(0.0�`1.0)*(���_���W*�{�[���I�t�Z�b�g�s��)
					// ���|����s��S�ĂŌv�Z������̍��v�l�ŋ��܂�B
					mStacks[m] = pMeshContainer->m_pBoneOffsetMatrices[iMatrixIndex] *
						//�v�Z�ȗ��̂��߂Ƀ{�[���I�t�Z�b�g�s��Ƀ��[���h�s����|���Ă���
						(*pMeshContainer->m_ppBoneMatrix[iMatrixIndex]);
					m_pDevice->SetTransform(D3DTS_WORLDMATRIX(k), &mStack);
					// �s��X�^�b�N�ɓo�^
					mStack = mStacks[m];
					m++;
				}
			}
			//�V�F�[�_�[�J�n
			UINT pass = m_shader->Begine();
			Vector3 camera = Vector3(view._41, view._42, view._43);
			m_shader->SetValue("EyePos", camera);
			//�V�F�[�_�Ƀp�����[�^�Z�b�g
			//���[�J��-�ˉe�ϊ��s��
			m_shader->SetValue("W", m_transMatrix);
			m_shader->GetEffect()->SetMatrix("V", &view);
			m_shader->GetEffect()->SetMatrix("P", &proj);
			m_shader->SetValue("AmbientMat", &Vector4(1.0f, 1.0f, 1.0f, 1));
			m_shader->SetValue("DiffuseMat", &Vector4(0.6f, 0.6f, 0.6f, 1));
			m_shader->GetEffect()->SetMatrixArray("WStack", mStacks, m);
			m_shader->GetEffect()->SetInt("StackNum", dwBlendMatrixNum);
			Matrix mat = m_transMatrix * view *proj;
			m_shader->SetMatrix(&mat);
			//�R�~�b�g�`�F���W
			m_shader->CommitChanges();
			dwPrevBoneID = pBoneCombination[i].AttribId;
			//�`�揈�����V�F�[�_��Begin-End�ň͂ނ���
			for (DWORD k = 0; k < pass; k++){
				//�V�F�[�_�[�p�X�J�n
				m_shader->BeginePass(k);
				//�Œ�V�F�[�_�p�ގ��Z�b�g
				m_pDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneCombination[i].AttribId].MatD3D);
				//�e�N�X�`���Z�b�g
				if (pMeshContainer->m_ppTextures[pBoneCombination[i].AttribId] != NULL){
					m_pDevice->SetTexture(0, pMeshContainer->m_ppTextures[pBoneCombination[i].AttribId]);
				}
	//			else{ m_pDevice->SetTexture(0, NULL); }
				//�����_�����O
				pMeshContainer->MeshData.pMesh->DrawSubset(i);
				//�V�F�[�_�[�p�X�I��
				m_shader->EndPass();
			}
			//�V�F�[�_�[�I��
			m_shader->End();
		}
		//���_�u�����h�g�p�I��
		m_pDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	}
}

/**
* @brief �t���[���X�V����(�Œ�t���[��)
*/
void SkinMesh::UpdateFrame()
{
	// �A�j���[�V�����^�C���̎擾
	float time = (float)m_pAnimController->GetTime();
	// �f�o�b�O
	char szTmp[256];
	sprintf_s(szTmp, "animation time=%f\n", time);
	OutputDebugString(szTmp);
	m_animCurWeight += ANIM_SPEED;
	if (m_animCurWeight < m_totalShiftTime){
		float fWeight = m_animCurWeight / m_totalShiftTime;
		// �g���b�N�̏d�ݐݒ�
		m_pAnimController->SetTrackWeight(0, fWeight);
		m_pAnimController->SetTrackWeight(1, 1.0f - fWeight);
		m_pAnimController->AdvanceTime(0, NULL);
	}
	else{
		m_animCurWeight = m_totalShiftTime;
		m_pAnimController->SetTrackWeight(0, 1.0f);
		// �~�L�V���O�𖳌�
		m_pAnimController->SetTrackEnable(1, FALSE);
		if (!m_bChanging){
			// ���[�v����
			if (time < m_animPeriod){
				m_pAnimController->AdvanceTime(ANIM_SPEED, NULL);
			}
		}
		else{
			// ���[�v�L��
			m_pAnimController->AdvanceTime(ANIM_SPEED, NULL);
		}
	}
}

/**
* @brief �t���[���X�V����
* @param �A�j���[�V�������x
*/
void SkinMesh::UpdateFrame(float speed)
{
	// �A�j���[�V�����^�C���̎擾
	float time = (float)m_pAnimController->GetTime();
	// �f�o�b�O
	char szTmp[256];
	sprintf_s(szTmp, "animation time=%f\n", time);
	OutputDebugString(szTmp);
	m_animCurWeight += speed / 60.0f;
	if (m_animCurWeight < m_totalShiftTime){
		float fWeight = m_animCurWeight / m_totalShiftTime;
		//�g���b�N�̏d�ݐݒ�
		m_pAnimController->SetTrackWeight(0, fWeight);
		m_pAnimController->SetTrackWeight(1, 1.0f - fWeight);
		m_pAnimController->AdvanceTime(0, NULL);
	}
	else{
		m_animCurWeight = m_totalShiftTime;
		m_pAnimController->SetTrackWeight(0, 1.0f);
		// �~�L�V���O�𖳌�
		m_pAnimController->SetTrackEnable(1, FALSE);
		if (!m_bChanging){
			// ���[�v����
			if (time < m_animPeriod){
				m_pAnimController->AdvanceTime(speed / 60.0f, NULL);
			}
		}
		else{
			// ���[�v�L��
			m_pAnimController->AdvanceTime(speed / 60.0f, NULL);
		}
	}
}

/**
* @brief ���O�Ńp�[�c�����ċA�����p
* @param name �A�j���[�V������
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
* @brief ���O�Ń{�[���p�[�c����
* @param name �A�j���[�V������
*/
bool SkinMesh::GetPartsMatrixByName(Matrix *pOut, LPCSTR name)
{
	return _GetPartsMatrixByName(pOut, m_pFrameRoot, name);
}

/**
* @brief �A�j���[�V�����؂�ւ�
* @param dwSetNo �A�j���[�V�����ԍ�
* @param bLoop �A�j�����[�v�t���O
*/
void SkinMesh::SetAnimNo(DWORD dwAnimNo, bool bLoop)
{
	// ���̃A�j���[�V�������Ȃ��ꍇ�߂�
	if (dwAnimNo >= m_uNumAnimSet) return;
	LPD3DXANIMATIONSET	lpAnimSet = m_pAnimSet[dwAnimNo];
	D3DXTRACK_DESC		TrackDesc;
	if (m_totalShiftTime > 0.0f){
		// �O�̃A�j���̃g���b�N�����g���b�N1�ɃR�s�[
		m_pAnimController->GetTrackDesc(0, &TrackDesc);
		m_pAnimController->SetTrackAnimationSet(1, m_pAnimSet[m_nNowAnimeNo]);
		m_pAnimController->SetTrackDesc(1, &TrackDesc);
		// �~�L�V���O��L���ɂ���
		m_pAnimController->SetTrackEnable(1, TRUE);
		// �g���b�N�̏d�݂�0.0f
		m_animCurWeight = 1.0f;
		// �g���b�N�̏d�݂�0.0f
	}
	else{ 
		m_animCurWeight = 1.0f;
	}
	// �g���b�N0�ŃA�j���[�V�����Đ�
	ZeroMemory(&TrackDesc, sizeof(TrackDesc));
	TrackDesc.Weight = 1;
	TrackDesc.Speed = 1;
	TrackDesc.Enable = 1;
	m_pAnimController->SetTrackDesc(0, &TrackDesc);
	m_pAnimController->SetTrackAnimationSet(0, lpAnimSet);
	// �~�L�V���O��L���ɂ���
	m_pAnimController->SetTrackEnable(0, TRUE);
	m_pAnimController->ResetTime();
	m_animPeriod = (float)lpAnimSet->GetPeriod();
	// ���[�v���ǂ����̕ύX
	m_bChanging = bLoop;
	// ���݂̃A�j���[�V�����o�^
	m_nNowAnimeNo = dwAnimNo;
}

/**
* @brief �A�j���[�V�������O����
* @param dwSetNo �A�j���[�V�����ԍ�
* @param bLoop �A�j�����[�v�t���O
*/
void SkinMesh::SetAnimName(LPCSTR szName, bool bLoop)
{
	DWORD dwAnimNo = 0;
	for (; dwAnimNo< m_uNumAnimSet; dwAnimNo++){
		// �o�^���ꂽ���O��莟��
		if (strcmp(m_pAnimSet[dwAnimNo]->GetName(), szName) == 0){ break; }
	}
	if (dwAnimNo >= m_uNumAnimSet) return;
	// �A�j���[�V�����ݒ�
	SetAnimNo(dwAnimNo, bLoop);
}

/**
* @brief �A���t�@�l��ݒ�
*/
void SkinMesh::SetAlpha(float alpha)
{
	m_alpha = alpha;
}

/**
* @brief �A�j���[�V�����̃V�t�g���Ԃ�ݒ�
*/
void SkinMesh::SetAnimShiftTime(float fTime)
{
	m_totalShiftTime = fTime;
}

/**
* @brief �A�j���[�V�������Ō�܂ōĐ��ς݂�
*/
bool SkinMesh::IsAnimPeriod()
{
	if (m_bChanging) return false;

	double time = m_pAnimController->GetTime();
	if (time < m_animPeriod) return false;

	return TRUE;
}

/**
* @brief �A�j���[�V�����R���g���[���[�̎擾
*/
LPD3DXANIMATIONCONTROLLER SkinMesh::GetController()
{
	return m_pAnimController; 
}

/**
* @brief �A�j���[�V�����^�C�����擾
*/
float SkinMesh::GetTime()
{
	return (float)m_pAnimController->GetTime();
}

/**
* @brief �A�j���[�V�����ԍ����擾
*/
int SkinMesh::GetNowAnimNo()
{
	return m_nNowAnimeNo; 
}

/**
* @brief �A���t�@�l�̎擾
*/
float SkinMesh::GetAlpha()
{
	return m_alpha; 
}