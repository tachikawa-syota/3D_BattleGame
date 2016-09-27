/**
* @file   AnimMesh.h
* @brief  �A�j���[�V�������b�V��
* @author Syota Tachikawa
*/

#ifndef ___ANIMMESH_H
#define ___ANIMMESH_H

#include "Mesh.h"
#include "AnimHierarchy.h"

/// �A�j���[�V�������x
#define ANIM_SPEED	1.0f/60

/**
* @brief �X�L�����b�V���N���X
*/
class SkinMesh : public Mesh, public Hierachy
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	SkinMesh();

	/**
	* @brief �f�X�g���N�^
	*/
	~SkinMesh();

	/**
	* @brief �A�j���[�V�������b�V���̓ǂݍ���
	* @param path - �p�X
	* @param xFileName - �w�t�@�C���̖��O
	*/
	bool LoadAnimeXMesh(string path, LPSTR xFileName);

	/**
	* @brief �A�j���[�V�������b�V���̕`��
	*/
	void RenderAnimeXMesh();

	/**
	* @brief �A�j���[�V�������b�V���̕`��(�V�F�[�_�[)
	*/
	void RenderAnimeXMesh(const Matrix& view, const Matrix& proj);

	/**
	* @brief �e��`�悷��
	*/
	void DrawShadow(LPD3DXFRAME pFrameBase);

	/**
	* @brief �A�j���[�V�����t���[���̍X�V
	*/
	void UpdateFrame();

	/**
	* @brief �A�j���[�V�����t���[���̍X�V
	*/
	void UpdateFrame(float speed);

	/**
	* @brief �A�j���[�V�������Ō�܂ōĐ��ς݂�
	*/
	bool IsAnimPeriod();

	/**
	* @brief �A�j���[�V�����R���g���[���[�̎擾
	*/
	LPD3DXANIMATIONCONTROLLER GetController();

	/**
	* @brief �A�j���[�V�����^�C�����擾
	*/
	float GetTime();

	/**
	* @brief �A�j���[�V�����ԍ����擾
	*/
	int GetNowAnimNo();

	/**
	* @brief �A���t�@�l�̎擾
	*/
	float GetAlpha();

	/**
	* @brief ���O�Ń{�[���p�[�c������
	* @param name �A�j���[�V������
	*/
	bool GetPartsMatrixByName(Matrix *pOut, LPCSTR name);

	/**
	* @brief �A���t�@�l��ݒ�
	*/
	void SetAlpha(float alpha);

	/**
	* @brief �A�j���[�V�����؂�ւ�
	* @param dwSetNo �A�j���[�V�����ԍ�
	* @param bLoop �A�j�����[�v�t���O
	*/
	void SetAnimNo(DWORD dwAnimNo, bool bLoop);

	/**
	* @brief �A�j���[�V�������O����
	* @param dwSetNo �A�j���[�V�����ԍ�
	* @param bLoop �A�j�����[�v�t���O
	*/
	void SetAnimName(LPCSTR szName, bool bLoop);

	/**
	* @brief �A�j���[�V�����̃V�t�g���Ԃ�ݒ�
	*/
	void SetAnimShiftTime(float fTime);

private:

	static const int MaxBoneMatrix = 12;

	/// �f�o�C�X
	LPDIRECT3DDEVICE9 m_pDevice;
	
	/// �A�j���[�V�����R���g���[���[
	LPD3DXANIMATIONCONTROLLER m_pAnimController;
	
	/// �A�j���[�V�����Z�b�^�[
	LPD3DXANIMATIONSET* m_pAnimSet;

	/// ���݂̃A�j���[�V�����ԍ�
	int m_nNowAnimeNo;
	
	/// �A�j���[�V�����Z�b�g
	DWORD m_uNumAnimSet;

	/// �A���t�@�l
	float m_alpha;

	float m_animCurWeight;

	/// �A�j���[�V�����̃V�t�g����
	float m_totalShiftTime;

	/// �A�j���[�V�����I������
	float m_animPeriod;

	/// �؂�ւ��t���O
	bool m_bChanging;

	/**
	* @brief ������
	*/
	void Initialize();
	
	/**
	* @brief �A�j���[�V�������
	*/
	void FreeAnim(LPD3DXFRAME pFrame);

	/**
	* @brief �`��t���[��
	*/
	void DrawFrame(LPD3DXFRAME pFrameBase);

	/**
	* @brief �`��t���[��(�V�F�[�_�[)
	*/
	void DrawFrame(const Matrix& view, const  Matrix& proj, LPD3DXFRAME pFrameBase);

	/**
	* @brief �t���[���X�V
	*/
	void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);
	
	/**
	* @brief �����_�����O�R���e�i
	*/
	void RenderMeshContainer(const MeshContainer *pMeshContainer, const Frame *pFrame);

	/**
	* @brief �����_�����O�R���e�i(�V�F�[�_�[)
	*/
	void RenderMeshContainer(const Matrix& view, const Matrix& proj, const MeshContainer *pMeshContainer, const Frame *pFrame);

	/**
	* @brief ���O�Ńp�[�c�����ċA�����p
	* @param name �A�j���[�V������
	*/
	bool _GetPartsMatrixByName(Matrix *pOut, LPD3DXFRAME pFrameBase, LPCSTR szName);
	
	/**
	* @brief �{�[���s��̏�����
	*/
	HRESULT AllocateBoneMatrix(LPD3DXMESHCONTAINER pMeshContainerBase);

	/**
	* @brief �S�Ẵ{�[���s��
	*/
	HRESULT AllocateAllBoneMatrices(LPD3DXFRAME pFrame);
};

/// �X�L�����b�V���̃X�}�[�g�|�C���^
using SkinMeshPtr = shared_ptr<SkinMesh>;

#endif