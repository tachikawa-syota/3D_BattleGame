/**
* @file   Mesh.h
* @brief  ���b�V���f�[�^
* @author Syota Tachikawa
*/

#ifndef ___MESH_H
#define ___MESH_H

#include "..\Shader\ShaderManager.h"

/**
* @brief ���b�V���N���X
*/
class Mesh
{
public:
	/// ���[�h�t���O
	bool bLoad;

	/**
	* @brief �R���X�g���N�^
	*/
	Mesh();

	/**
	* @brief �f�X�g���N�^
	*/
	virtual ~Mesh();

	/**
	* @brief �N���[������
	*/
	Mesh* Clone();

	/**
	* @brief ���b�V���ǂݍ���
	*/
	virtual bool LoadNonAnimMesh(string path, string xFileName);

	/**
	* @brief �X�V
	*/
	virtual void Update();

	/**
	* @brief �����_�����O
	*/
	virtual void RenderXMesh();

	/**
	* @brief �����_�����O(�V�F�[�_�[�̓K�p)
	* @param view - �r���[�s��
	* @param proj - ���e�s��
	*/
	virtual void RenderXMesh(const Matrix& view, const Matrix& proj);
	
	/**
	* @brief �e��`�悷��
	*/
	virtual void DrawShadow();

	/**
	* @brief �����蔻��
	*/
	int	RayPick(Vector3* out, Vector3* pos, Vector3* vec, float *Dist);
	int	RayPickUD(Vector3* out, Vector3* pos, Vector3* vec, float *Dist);

private:
	/// �e�N�X�`����
	static const int NumTexture = 1024;
	/// �f�o�C�X
	LPDIRECT3DDEVICE9 m_pDevice;

protected:
	unsigned long m_dwFlags;
	unsigned char m_bChanged;

	/// �V�F�[�_�[
	ShaderPtr m_shader;

	// ���b�V�����
	LPD3DXMESH m_lpMesh;

	/// �}�e���A�����
	D3DMATERIAL9* m_lpMeshMaterials;
	
	/// �}�e���A���̐�
	DWORD m_numMaterials;
	
	/// �e�N�X�`��
	LPDIRECT3DTEXTURE9*	m_lpMeshTextures;
	
	/// �m�[�}���}�b�v
	LPDIRECT3DTEXTURE9*	m_lpNormal;
	
	/// �X�y�L�����[
	IDirect3DTexture9**	m_lpSpecular;

	/// �n�C�g�}�b�v
	IDirect3DTexture9**	m_lpHeight;

	LPD3DXFRAME	m_pFrameRoot;

	//�]���s��
	Matrix m_transMatrix;

	/// ���W
	Vector3	m_pos;

	/// �p�x
	Vector3	m_angle;

	/// �傫��
	Vector3	m_scale;
public:
	/**
	* @brief �G�t�F�N�g�}�e���A���̃Z�b�g
	* @param shader - �V�F�[�_�[
	* @param material - ���b�V���̃}�e���A�����
	*
	* @note �V�F�[�_�[�`������鎞�Ɏg�p����
	*/
	void SetEffectMaterial(const ShaderPtr& shader, const D3DMATERIAL9& material);

	/**
	* @brief �V�F�[�_�[�̐ݒ�
	*/
	void SetShader(const ShaderPtr& shader);

	/**
	* @brief ���[���h���W���擾����
	*/
	const Matrix& GetTransMatrix(){ return m_transMatrix; }
	
	/**
	* @brief ���W���擾����
	*/
	const Vector3& GetPos(){ return m_pos; }

	/**
	* @brief �p�x���擾����
	*/
	const Vector3& GetAngle(){ return m_angle; }
	
	/**
	* @brief �傫�����擾����
	*/
	const Vector3& GetScale(){ return m_scale; }
	
	/**
	* @brief ���b�V�������擾����
	*/
	const LPD3DXMESH& GetMesh(){ return m_lpMesh; }

	LPD3DXFRAME GetpFrameroot() { return m_pFrameRoot; }
	
	/**
	* @brief ���W�̐ݒ�
	*/
	void SetPos(const Vector3& pos);
	void SetPos(float x, float y, float z);

	/**
	* @brief �p�x�̐ݒ�
	*/
	void SetAngle(const Vector3& angle);
	void SetAngle(float angle);
	void SetAngle(float x, float y, float z);
	
	/**
	* @brief �傫���̐ݒ�
	*/
	void SetScale(const Vector3& scale);
	void SetScale(float scale);
	void SetScale(float x, float y, float z);
};

/// ���b�V���̃X�}�[�g�|�C���^
using MeshPtr = shared_ptr<Mesh>;

#endif