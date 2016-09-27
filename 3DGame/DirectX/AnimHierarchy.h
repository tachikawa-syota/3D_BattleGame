/**
* @file   AnimHierarchy.h
* @brief  �q�G�����L�[
* @author Syota Tachikawa
*/

#ifndef ___ANIMHIERARCHYH_H
#define ___ANIMHIERARCHYH_H

#include "..\Common\CommonLib.h"

/**
* @brief ���b�V���R���e�i
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
* @brief X�t�@�C�����̃A�j���[�V�����K�w��ǂ݉����Ă����N���X��h��������B
* @note ID3DXAllocateHierarchy�C���^�[�t�F�[�X�F�t���[���K�w��ǂݍ��݂���єj�����s��
* @note	ID3DXAllocateHierarchy�͔h�����邱�Ƒz�肵�Đ݌v����Ă���B
*/
class Hierachy : public ID3DXAllocateHierarchy
{
public:
	//�R���X�g���N�^
	Hierachy(){};
	//�f�X�g���N�^
	~Hierachy(){};
	//STDMETHOD(FuncName) -->COM�}�N�� = virtual HRESULT _stdcall FuncName
	STDMETHOD(CreateFrame)(THIS_ LPCSTR, LPD3DXFRAME *);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCTSTR, CONST D3DXMESHDATA*, CONST D3DXMATERIAL*,
		CONST D3DXEFFECTINSTANCE*, DWORD, CONST DWORD *, LPD3DXSKININFO, LPD3DXMESHCONTAINER*);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);
};

/**
* @brief �h���t���[���\����(���ꂼ��̃��b�V���p�̍ŏI���[���h�s���ǉ�����j
* @note D3DXFRAME�Ɋi�[�����̂͐e->�q�ւ̍��W�ϊ��s��
* @note �q->�e�ւ̋t�����̕ϊ� = �C���o�[�X�L�l�}�e�B�N�X
*/
class Frame : public D3DXFRAME
{
public:
	D3DXMATRIXA16 m_combinedTransformationMatrix;
	D3DXMATRIXA16 m_localCombinedTransformationMatrix;
};

#endif
