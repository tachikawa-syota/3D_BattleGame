#include "Mesh.h"
#include "..\DirectX\Direct3DData.h"

/**
* @brief �R���X�g���N�^
*/
Mesh::Mesh()
{
	m_shader = nullptr;
	//���b�V����{�ϐ�
	m_lpMesh = NULL;
	m_lpMeshTextures = NULL;
	m_lpMeshMaterials = NULL;
	m_numMaterials = 0;
	//�����ݒ�
	SetPos(Vector3(0, 0, 0));
	SetScale(1.0f);
	SetAngle(0.0f);
	Update();
}

/**
* @brief �f�X�g���N�^
*/
Mesh::~Mesh()
{
}

/**
* @brief �N���[���𐶐�����
*/
Mesh* Mesh::Clone()
{
	//�I�u�W�F�N�g����
	Mesh* obj = new Mesh(*this);
	obj->bLoad = false;
	return 0;
}

/**
* @brief X�t�@�C���̓ǂݍ���
* @param path      - �p�X
* @param xFileName - �t�@�C���̖��O
*/
bool Mesh::LoadNonAnimMesh(string path, string xFileName)
{
	// �f�o�C�X�̎擾
	m_pDevice = D3DIns->GetDevice();

	//�}�e���A�������󂯎��ϐ�
	LPD3DXBUFFER pD3DXAdjacencyBuffer = NULL;
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;
	LPD3DVERTEXELEMENT9 pD3DXElement = NULL;

	// X�t�@�C����ǂݍ���
	string modelName = path + xFileName;

	//X�t�@�C�����烁�b�V�������[�h����
	if (FAILED(D3DXLoadMeshFromX(modelName.c_str(), D3DXMESH_MANAGED, m_pDevice, &pD3DXAdjacencyBuffer,
		&pD3DXMtrlBuffer, NULL, &m_numMaterials, &m_lpMesh))){
		return false;
	}
	//�œK��
	if (FAILED(m_lpMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)pD3DXAdjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL))){
		return false;
	}
	SafeRelease(pD3DXAdjacencyBuffer);
	//�}�e���A���ƃ}�e���A���ɑ�����e�N�X�`���̓ǂݍ���
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	//�}�e���A���ƃe�N�X�`���̃I�u�W�F�N�g����
	m_lpMeshMaterials = new D3DMATERIAL9[m_numMaterials];
	m_lpMeshTextures = new LPDIRECT3DTEXTURE9[m_numMaterials];
	m_lpNormal = new  LPDIRECT3DTEXTURE9[m_numMaterials];
	m_lpSpecular = new  LPDIRECT3DTEXTURE9[m_numMaterials];
	m_lpHeight = new  LPDIRECT3DTEXTURE9[m_numMaterials];

	//�}�e���A�����ƃe�N�X�`�����擾
	for (DWORD i = 0; i < m_numMaterials; i++){
		//�}�e���A�����
		m_lpMeshMaterials[i] = d3dxMaterials[i].MatD3D;
		m_lpMeshMaterials[i].Ambient = m_lpMeshMaterials[i].Diffuse;
		//�e�N�X�`�����
		m_lpMeshTextures[i] = NULL;
		m_lpNormal[i] = NULL;
		//�e�N�X�`���ǂݍ���
		if (d3dxMaterials[i].pTextureFilename != NULL){

			// �e�N�X�`���̖��O
			string texFileName;
			texFileName = path + d3dxMaterials[i].pTextureFilename;

			if (FAILED(D3DXCreateTextureFromFile(m_pDevice, texFileName.c_str(), &m_lpMeshTextures[i]))){
				m_lpMeshTextures[i] = NULL;
			}
		}
	}
	//�s��o�b�t�@�̉��
	SafeRelease(pD3DXMtrlBuffer);

	// ����
	return true;
}


void Mesh::SetPos(float x, float y, float z)
{
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}
void Mesh::SetPos(const Vector3& pos)
{
	m_pos = pos;
}

void Mesh::SetAngle(float x, float y, float z)
{
	m_angle.x = x;
	m_angle.y = y;
	m_angle.z = z;
}
void Mesh::SetAngle(float angle)
{
	m_angle.x = 0.0f;
	m_angle.y = angle;
	m_angle.z = 0.0f;
}
void Mesh::SetAngle(const Vector3& angle)
{
	m_angle = angle;
}


void Mesh::SetScale(float x, float y, float z)
{
	m_scale.x = x;
	m_scale.y = y;
	m_scale.z = z;
}
void Mesh::SetScale(float scale)
{
	m_scale.x = scale;
	m_scale.y = scale;
	m_scale.z = scale;
}
void Mesh::SetScale(const Vector3& scale)
{
	m_scale = scale;
}

/**
* @brief �X�V
*/
void Mesh::Update()
{
	Matrix matScale;
	//�T�C�Y�ݒ�
	D3DXMatrixScaling(&matScale, m_scale.x, m_scale.y, m_scale.z);
	//���W�E��]�ݒ� 
	SetTransformMatrixXYZ(&m_transMatrix, m_pos.x, m_pos.y, m_pos.z, m_angle.x, m_angle.y, m_angle.z);
	//�]���s��쐬 
	m_transMatrix = matScale * m_transMatrix;
}

/**
* @brief �`��
*/
void Mesh::RenderXMesh()
{
	// �f�o�C�X�̎擾
	m_pDevice = D3DIns->GetDevice();

	//�]���s��ݒ�
	m_pDevice->SetTransform(D3DTS_WORLD, &m_transMatrix);
	//�}�e���A���Z�b�g���ƂɃ��b�V���`��
	for (DWORD i = 0; i < m_numMaterials; i++){
		//�ގ��ݒ�
		m_pDevice->SetMaterial(&m_lpMeshMaterials[i]);
		//�e�N�X�`���ݒ�
		m_pDevice->SetTexture(0, m_lpMeshTextures[i]);
		//�ގ��O���[�v�`��
		m_lpMesh->DrawSubset(i);
	}
}

/**
* @brief �����_�����O(�V�F�[�_�[�̓K�p)
* @param shader - �V�F�[�_�[�N���X(�X�}�[�g�|�C���^)
* @param name - �V�F�[�_�[�p�X�̖��O
* @param view - �r���[�s��
* @param proj - ���e�s��
*/
void Mesh::RenderXMesh(const Matrix& view, const Matrix& proj)
{
	// �V�F�[�_�[���ݒ肳��Ă��Ȃ�������f�t�H���g�V�F�[�_�[��K������
	if (m_shader == nullptr){
		m_shader = ShaderManager::GetInstance()->GetStandardShader();
		m_shader->GetEffect()->SetTechnique("StandardShader");
	}

	//�V�F�[�_�[�̓K�p
	UINT pass = m_shader->Begine();
	//���[�J��-�ˉe�ϊ��s��
	m_shader->SetValue("W", m_transMatrix);
	m_shader->SetValue("V", view);
	m_shader->SetValue("P", proj);

	for (DWORD p = 0; p<pass; p++){
		//�p�X�̃����_�����O
		m_shader->BeginePass(p);
		for (DWORD i = 0; i < m_numMaterials; i++){
			//�e�N�X�`���w��
			m_shader->SetTexture(m_lpMeshTextures[i]);
			//�@���}�b�v�e�N�X�`���ݒ�
	//		m_shader->SetValue("NormalMapTexture", m_lpNormal[i]);
			//�X�y�L�����}�b�v�e�N�X�`��
	//		m_shader->SetValue("SpecularMap", m_lpMeshTextures[i]);
			//�����}�b�v�e�N�X�`��
	//		m_shader->SetValue("HeightMap", m_lpMeshTextures[i]);
			
	//		SetEffectMaterial(m_shader, m_lpMeshMaterials[i]);
			//�R�~�b�g�`�F���W
			m_shader->CommitChanges();
			//�ގ��O���[�v�`��
			m_lpMesh->DrawSubset(i);
		}
		m_shader->EndPass();
	}
	m_shader->End();
}

/**
* @brief ���C�s�b�N�i��ԋ߂����b�V���f�[�^�j
* @param out
* @param pos  - ���胁�b�V���̈ʒu
* @param vec  - �����x�N�g��
* @param Dist - ����
*/
int	Mesh::RayPick(Vector3* out, Vector3* pos, Vector3* vec, float *Dist){
	int ret = -1;
	if (vec->x == 0.0f && vec->z == 0.0f) return RayPickUD(out, pos, vec, Dist);
	Vector3 p = *pos;		//���C�̊J�n�ʒu
	Vector3 vv = *vec;		//�����x�N�g��
	float neart = *Dist;
	float dist = *Dist;
	dist = dist * dist;
	*out = p;
	//���擾(���_�t�H�[�}�b�g)
	ULONG fvf = m_lpMesh->GetFVF();
	//���_�T�C�Y�v�Z
	int VertexSize = D3DXGetFVFVertexSize(fvf) / sizeof(float);
	//�o�b�t�@���b�N
	float*	pVertices;
	WORD*	pIndices;
	ULONG	NumIndices = m_lpMesh->GetNumFaces();
	//���_�o�b�t�@�����b�N
	m_lpMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertices);
	//�C���f�b�N�X�o�b�t�@�����b�N(�|�C���^�ւ̃������擾)
	m_lpMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndices);
	//�ʂ̎O���_
	Vector3 v1, v2, v3;
	Vector3	n;
	//�ӂ̒���
	Vector3	l1, l2, l3;
	Vector3	temp;
	Vector3	cp;
	Vector3 p1, p2, p3;
	for (ULONG j = 0; j < NumIndices; j++){
		//�ʒ��_�擾
		int a = pIndices[j * 3 + 0] * VertexSize;
		v1.x = pVertices[a];
		v1.y = pVertices[a + 1];
		v1.z = pVertices[a + 2];
		int b = pIndices[j * 3 + 1] * VertexSize;
		v2.x = pVertices[b];
		v2.y = pVertices[b + 1];
		v2.z = pVertices[b + 2];
		int c = pIndices[j * 3 + 2] * VertexSize;
		v3.x = pVertices[c];
		v3.y = pVertices[c + 1];
		v3.z = pVertices[c + 2];
		//��������
		//Vector3	ss = (v1 + v2 + v3) / 3.0f - p;
		//if( ss.LengthSq() > dist ) continue;
		l1.x = v2.x - v1.x;	l1.y = v2.y - v1.y;	l1.z = v2.z - v1.z;
		l2.x = v3.x - v2.x;	l2.y = v3.y - v2.y;	l2.z = v3.z - v2.z;
		//�O�ςɂ��@���Z�o		
		Vector3Cross(n, l1, l2);
		//���ς̌��ʂ��v���X�Ȃ�Η�����
		float dot = Vector3Dot(vv, n);
		if (dot >= 0) continue;
		//��_�Z�o
		p1.x = v1.x - p.x;	p1.y = v1.y - p.y;	p1.z = v1.z - p.z;
		float t = Vector3Dot(n, p1) / dot;
		if (t < .0f || t > neart) continue;

		cp.x = vv.x*t + p.x;	cp.y = vv.y*t + p.y;	cp.z = vv.z*t + p.z;
		//���_����
		p1.x = v1.x - cp.x;		p1.y = v1.y - cp.y;	p1.z = v1.z - cp.z;
		Vector3Cross(temp, p1, l1);
		if (Vector3Dot(temp, n) < 0.0f) continue;

		p2.x = v2.x - cp.x;		p2.y = v2.y - cp.y;		p2.z = v2.z - cp.z;
		Vector3Cross(temp, p2, l2);
		if (Vector3Dot(temp, n) < 0.0f) continue;

		l3.x = v1.x - v3.x;		l3.y = v1.y - v3.y;		l3.z = v1.z - v3.z;
		p3.x = v3.x - cp.x;		p3.y = v3.y - cp.y;		p3.z = v3.z - cp.z;
		//�O��
		Vector3Cross(temp, p3, l3);
		//����
		if (Vector3Dot(temp, n) < 0.0f) continue;
		*out = cp;
		*vec = n;
		ret = j;
		neart = t;
	}
	//���_�o�b�t�@�̃��b�N������
	m_lpMesh->UnlockVertexBuffer();
	//�C���f�b�N�X�o�b�t�@�����b�N������
	m_lpMesh->UnlockIndexBuffer();
	*Dist = neart;
	return	ret;
}

/**
* @brief �㉺�œK��
* @param out
* @param pos  - ���胁�b�V���̈ʒu
* @param vec  - �����x�N�g��
* @param Dist - ����
*/
int	Mesh::RayPickUD(Vector3* out, Vector3* pos, Vector3* vec, float *Dist){
	float t, neart;
	float vy;
	int ret = -1;

	int	VertexSize;

	Vector3	p = *pos;
	vy = vec->y;

	neart = *Dist;
	//���擾	
	int fvf = m_lpMesh->GetFVF();
	//���_�T�C�Y�v�Z
	VertexSize = D3DXGetFVFVertexSize(fvf) / sizeof(float);
	//�o�b�t�@���b�N
	float*	pVertices;
	WORD*	pIndices;
	int		NumIndices = m_lpMesh->GetNumFaces();
	m_lpMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertices);
	m_lpMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndices);

	Vector	l1, l2, l3;
	Vector	p1, p2, p3;
	Vector v[3];
	Vector n;

	for (int j = 0; j<NumIndices; j++){
		//�ʒ��_�擾
		int a = pIndices[j * 3 + 0] * VertexSize;
		int b = pIndices[j * 3 + 1] * VertexSize;
		int c = pIndices[j * 3 + 2] * VertexSize;

		v[0].x = pVertices[a];	v[1].x = pVertices[b];	v[2].x = pVertices[c];
		if (v[0].x > p.x && v[1].x > p.x && v[2].x > p.x) continue;

		v[0].z = pVertices[a + 2];	v[1].z = pVertices[b + 2];	v[2].z = pVertices[c + 2];
		if (v[0].z > p.z && v[1].z > p.z && v[2].z > p.z) continue;

		v[0].y = pVertices[a + 1];	v[1].y = pVertices[b + 1];	v[2].y = pVertices[c + 1];
		//���_����i�S�O�ς��}�C�i�X�j		
		l1.x = v[1].x - v[0].x;
		l1.z = v[1].z - v[0].z;
		p1.x = v[0].x - p.x;
		p1.z = v[0].z - p.z;
		if ((p1.x*l1.z - p1.z*l1.x)*vy < 0) continue;

		l2.x = v[2].x - v[1].x;
		l2.z = v[2].z - v[1].z;
		p2.x = v[1].x - p.x;
		p2.z = v[1].z - p.z;
		if ((p2.x*l2.z - p2.z*l2.x)*vy < 0) continue;

		l3.x = v[0].x - v[2].x;
		l3.z = v[0].z - v[2].z;
		p3.x = v[2].x - p.x;
		p3.z = v[2].z - p.z;
		if ((p3.x*l3.z - p3.z*l3.x)*vy < 0) continue;
		//�O�ςɂ��@���Z�o		
		l1.y = v[1].y - v[0].y;
		l2.y = v[2].y - v[1].y;
		Vector3Cross(n, l1, l2);
		//�\������
		if (vy*n.y >= 0) continue;
		//��_�Z�o
		p1.y = v[0].y - p.y;
		t = Vector3Dot(n, p1) / (n.y*vy);
		if (t < .0f || t > neart) continue;

		*vec = n;
		ret = j;
		neart = t;
	}
	m_lpMesh->UnlockVertexBuffer();
	m_lpMesh->UnlockIndexBuffer();

	out->y = neart*vy + p.y;
	out->x = pos->x;
	out->z = pos->z;
	*Dist = neart;

	return	ret;
}

/**
* @brief �G�t�F�N�g�}�e���A���̃Z�b�g
* @param shader - �V�F�[�_�[
* @param material - ���b�V���̃}�e���A�����
*
* @note �V�F�[�_�[�`������鎞�Ɏg�p����
*/
void Mesh::SetEffectMaterial(const ShaderPtr& shader, const D3DMATERIAL9& material)
{
	Vector4 tempcolor;
	// �����p�̃}�e���A�����Z�b�g
	tempcolor.x = material.Ambient.r;
	tempcolor.y = material.Ambient.g;
	tempcolor.z = material.Ambient.b;
	tempcolor.w = material.Ambient.a;
	shader->SetValue("AmbientMat", &tempcolor);

	// �f�B�t���[�Y���p�̃}�e���A�����Z�b�g
	tempcolor.x = material.Diffuse.r;
	tempcolor.y = material.Diffuse.g;
	tempcolor.z = material.Diffuse.b;
	tempcolor.w = material.Diffuse.a;
	shader->SetValue("DiffuseMat", &tempcolor);

	tempcolor.x = material.Specular.r;
	tempcolor.y = material.Specular.g;
	tempcolor.z = material.Specular.b;
	tempcolor.w = material.Specular.a;
	shader->SetValue("SpecularMat", &tempcolor);

	tempcolor.x = material.Emissive.r;
	tempcolor.y = material.Emissive.g;
	tempcolor.z = material.Emissive.b;
	tempcolor.w = material.Emissive.a;
	shader->SetValue("EmmisiveMat", &tempcolor);
}

void Mesh::DrawShadow()
{
	ShaderManager* shaderMgr = ShaderManager::GetInstance();
	// �T�u�Z�b�g��`�悷��(�}�e���A���������[�v)
	for (unsigned int i = 0; i < m_numMaterials; i++)
	{
		// �p�X�O�J�n
		shaderMgr->GetShadowMapShader()->BeginePass(0);
		m_lpMesh->DrawSubset(i);					// �T�u�Z�b�g�̕`��
		shaderMgr->GetShadowMapShader()->CommitChanges();
		shaderMgr->GetShadowMapShader()->EndPass();
	}
}

/**
* @brief �V�F�[�_�[�̐ݒ�
*/
void Mesh::SetShader(const ShaderPtr& shader)
{
	m_shader = shader;
}