#include "AnimHierarchy.h"

//�x���}��
#pragma warning(disable:4996)
#pragma warning(disable:4316)
#pragma warning(disable:4018)

/**
* @brief �t���[�����쐬����
*/
HRESULT Hierachy::CreateFrame(LPCTSTR szName, LPD3DXFRAME *ppNewFrame)
{
	Frame* pFrame;
	*ppNewFrame = NULL;
	//�t���[���|�C���^�I�u�W�F�N�g����
	pFrame = new Frame;
	if (pFrame == NULL){
		return E_OUTOFMEMORY;
	}
	//���O�t���[���I�u�W�F�N�g�̐���(�����̒���+1�������m��)
	pFrame->Name = new TCHAR[lstrlen(szName) + 1];
	if (!pFrame->Name){
		return E_FAIL;
	}
	strcpy(pFrame->Name, szName);
	//������
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->m_combinedTransformationMatrix);
	pFrame->pMeshContainer = NULL;
	pFrame->pFrameSibling = NULL;
	pFrame->pFrameFirstChild = NULL;
	*ppNewFrame = pFrame;
	return S_OK;
}

/**
* @brief ���b�V���R���e�i���쐬����
*/
HRESULT Hierachy::CreateMeshContainer(LPCSTR szName, CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppMeshContainer)
{
	// ���b�V���R���e�i�[
	MeshContainer *pMeshContainer = NULL;
	*ppMeshContainer = NULL;

	// �ʐ�
	int iFacesAmount;

	// �}�e���A����
	int iMaterial;

	// �f�o�C�X�̐ݒ�
	LPDIRECT3DDEVICE9 pDevice = NULL;

	// ���b�V��
	LPD3DXMESH pMesh = NULL;
	
	// �{�[����
	DWORD dwBoneNum = 0;
	
	// ���b�V���f�[�^
	pMesh = pMeshData->pMesh;
	
	// ���b�V���R���e�i�I�u�W�F�N�g�̐���
	pMeshContainer = new MeshContainer;
	
	// ���b�V���R���e�i�쐬���s
	if (pMeshContainer == NULL){ 
		return E_OUTOFMEMORY; 
	}

	ZeroMemory(pMeshContainer, sizeof(MeshContainer));

	// ���b�V���R���e�i���O
	pMeshContainer->Name = new TCHAR[lstrlen(szName) + 1];
	if (!pMeshContainer->Name){ return E_FAIL; }
	strcpy(pMeshContainer->Name, szName);

	pMesh->GetDevice(&pDevice);
	iFacesAmount = pMesh->GetNumFaces();
	pMeshContainer->MeshData.pMesh = pMesh;
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	// �X�L�����
	// �X�L�����b�V���ȊO�̏ꍇ
	if (pSkinInfo == NULL){
		// �Q�ƃJ�E���^��1���₷
		pMesh->AddRef();
	}
	// �}�e���A���f�[�^
	// ���b�V���̃}�e���A���ݒ�
	pMeshContainer->NumMaterials = max(1, NumMaterials);

	// �z��̊m��
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
		// �e�N�X�`���ǂݍ���
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
	// ���Y���b�V�����X�L�����������Ă���ꍇ�i�X�L�����b�V���ŗL�̏����j
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
	// ���[�J���ɐ����������b�V���R���e�i�[���Ăяo�����ɃR�s�[
	*ppMeshContainer = pMeshContainer;
	// �Q�ƃJ�E���^�𑝂₵���̂Ō��炷
	SafeRelease(pDevice);

	// ����
	return S_OK;
}

/**
* @brief �t���[�������
*/
HRESULT Hierachy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	//���O�t���[���z��̉��
	SafeDeleteArray(pFrameToFree->Name);
	//�q�t���[�����ċA���
	if (pFrameToFree->pFrameFirstChild){
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}
	//�e�t���[�����ċA���
	if (pFrameToFree->pFrameSibling){
		DestroyFrame(pFrameToFree->pFrameSibling);
	}
	//�t���[���̉��
	SafeDelete(pFrameToFree);

	// ����
	return S_OK;
}

/**
* @brief ���b�V���R���e�i�����
*/
HRESULT Hierachy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	int iMaterial;
	MeshContainer *pMeshContainer = (MeshContainer*)pMeshContainerBase;
	//���O���b�V���R���e�i�����
	SafeDeleteArray(pMeshContainer->Name);
	//�X�L�����̊J��
	SafeRelease(pMeshContainer->pSkinInfo);
	SafeDeleteArray(pMeshContainer->pAdjacency);
	//�}�e���A���̉��
	SafeDeleteArray(pMeshContainer->pMaterials);
	//�{�[���s��̉��
	SafeDeleteArray(pMeshContainer->m_ppBoneMatrix);
	//�e�N�X�`���z��̉��
	for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++){
		SafeRelease(pMeshContainer->m_ppTextures[iMaterial]);
	}
	//�e�N�X�`���̉��
	SafeDeleteArray(pMeshContainer->m_ppTextures);
	//���b�V���R���e�i�̃��b�V���f�[�^���
	SafeRelease(pMeshContainer->MeshData.pMesh);
	//�{�[���o�b�t�@�̉��
	SafeRelease(pMeshContainer->m_pBoneBuffer);
	SafeDeleteArray(pMeshContainer->m_pBoneOffsetMatrices);
	//���b�V���R���e�i�̉��
	SafeDelete(pMeshContainer);

	// ����
	return S_OK;
}
