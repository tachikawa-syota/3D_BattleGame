/**
* @file TextureLib.h
* @brief �e�N�X�`���̓ǂݍ��݂ƕ`�揈��
* @author Syota Tachikawa
*/

#ifndef ___TEXTURELIB_H
#define ___TEXTURELIB_H

#include "..\Math\Math.h"

// �摜�i�[�ő吔
#define MAX_TEXTURE		270


#pragma region �\���̒�`

/**
* @struct TEXTURE
* @brief �e�N�X�`���f�[�^
*/
typedef struct{
	LPDIRECT3DTEXTURE9	pDxTex;
	DWORD				nWidth;
	DWORD				nHeight;
	float				uv1[2], uv2[2];
	bool				used;
}TEXTURE;

/**
* @struct COLOR_POINT
* @brief �J���[�|�C���g
*/
typedef struct{
	float	x, y, z;
	float	rhw;
	DWORD	dwColor;
}COLOR_POINT;

/**
* @struct TEXTURE_DATA
* @brief CreateTexture�ō쐬�����f�[�^���i�[
*
* @note�F�V�F�[�_�[�Ŏg�p����
*/
typedef struct{
	/// �쐬�����e�N�X�`���f�[�^
	LPDIRECT3DTEXTURE9 pTexture;
	/// �쐬�����T�[�t�F�C�X�f�[�^
	LPDIRECT3DSURFACE9 pSurface;
}TEXTURE_DATA;

/**
* @struct D2D_POLYGON
* @brief 2D�|���S��
*/
typedef struct{
	/// ���_�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9 p2DVertexBuffer;
	/// ����
	UINT width;
	/// �c��
	UINT height;
}D2D_POLYGON;

/**
* @struct D2D_TEXTURE
* @brief 2D�e�N�X�`��
*/
typedef struct{
	float	x, y, z;
	float	rhw;
	DWORD	uDiffuse;
	float	tu, tv;
}D2D_TEXTURE;

/**
* @struct D3D_TEXTURE
* @brief �r���{�[�h�Ŏg�p����
*/
typedef struct{
	float	x, y, z;
	DWORD	uDiffuse;
	float	tu, tv;
}D3D_TEXTURE;

/**
* @struct ARGB
* @brief �A���t�@�A�ԁA�΁A��
*/
typedef struct{
	DWORD	alpha;
	DWORD	red;
	DWORD	green;
	DWORD	blue;
}ARGB;

#pragma endregion


/**
* @brief 2D�e�N�X�`���N���X
*/
class Texture2D
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	Texture2D();

	/**
	* @brief �f�X�g���N�^
	*/
	~Texture2D();

	/**
	* @brief �e�N�X�`���ǂݍ���
	*/
	BOOL LoadTexture(string fileName);
	
	/**
	* @brief �e�N�X�`���ǂݍ���
	*/
	LPDIRECT3DTEXTURE9 LoadD3DTexture(string fileName);

	/**
	* @brief �u���b�N�^�e�N�X�`���̓ǂݍ���
	*/
	bool LoadBlkTexture(string fileName, int sizeX, int sizeY, int numX, int numY, int num, int *data);

	/**
	* @brief �e�N�X�`�����쐬
	* @param �t�H�[�}�b�g
	* @return �쐬�����e�N�X�`���f�[�^
	*
	* @note�F��ɃV�F�[�_�[�Ŏg�p����
	*/
	TEXTURE_DATA& CreateTexture(UINT width, UINT height, DWORD Usage, D3DFORMAT format);

	/**
	* @brief 2D�|���S���𐶐�����
	* @param width - �|���S���̉���
	* @param height - �|���S���̏c��
	*
	* @note �F �V�F�[�_�[�Ŏg�p����
	*/
	BOOL Create2DPolygon(UINT width, UINT height);

	/**
	* @brief �e�N�X�`����`��(������)
	*/
	HRESULT DrawTexture(string fileName, float posX, float posY, float angle = 0.0f, float scale = 1.0f, int alpha = 255, bool add = false, bool reverse = false);
	HRESULT DrawTexture(int index, float posX, float posY, float angle = 0.0f, float scale = 1.0f, int alpha = 255, bool add = false, bool reverse = false);
	/**
	* @brief ��ʂ����ς��Ƀe�N�X�`����`�悷��
	* @param �e�N�X�`���f�[�^
	*
	* @note �F �V�F�[�_�[�ŗ��p����
	*/
	HRESULT DrawTexture(LPDIRECT3DTEXTURE9& texture);

	/**
	* @brief �e�N�X�`����`��(���E�����L�΂�)
	*/
	HRESULT DrawTextureEx(float x1, float y1, float x2, float y2, const LPDIRECT3DBASETEXTURE9& pTex, float z = 0.0f, bool add = false);

	/**
	* @brief �e�N�X�`����`��(�r���{�[�h)
	*/
	HRESULT Draw3DTex(const Matrix& matView, string fileName, const Vector3& pos, const Vector3& angle, float scale, int alpha, bool add = false, bool bAlpha = true);
	HRESULT Draw3DTex(const Matrix& matView, int index, const Vector3& pos, const Vector3& angle, float scale, int alpha, bool add = false);

	/**
	* @brief �e�N�X�`����`��(�~���̑��ʂ̂�)
	*/
	HRESULT Draw3DBandTexture(const Matrix& matView, string fileName, const  Vector3& pos, const Vector3& angle, float scale, float upRadius, float downRadius, float height, int alpha);

	/**
	* @brief �e�N�X�`����`��(�J���[�ݒ�L��)
	*/
	HRESULT DrawTexARGB(string fileName, float posX, float posY, ARGB argb, bool add = true, float angle = 0.0f, float scale = 1.0f, bool reverse = false);
	
	/**
	* @brief �l�p�`��`��
	*/
	HRESULT DrawBox(float nMinX, float nMinY, float nMaxX, float nMaxY, DWORD color);

	/**
	* @brief 2D�|���S����`��
	*/
	HRESULT Draw2DPolygon();

	/**
	* @brief �e�N�X�`���̉������擾����
	*/
	const DWORD GetWidth(string fileName);

	/**
	* @brief �e�N�X�`���̉������擾����
	*/
	const DWORD GetWidth(int index);

	/**
	* @brief �e�N�X�`���̏c�����擾����
	*/
	const DWORD GetHeight(string fileName);

	/**
	* @brief �e�N�X�`���̏c�����擾����
	*/
	const DWORD GetHeight(int index);

	/**
	* @brief 2D�|���S���̉������擾����
	*/
	const UINT Get2DPolygonWidth();

	/**
	* @brief 2D�|���S���̏c�����擾����
	*/
	const UINT Get2DPolygonHeight();

private:
	/// �f�o�C�X
	LPDIRECT3DDEVICE9 m_pDevice;

	/// �e�N�X�`���}�b�v
	map<string, TEXTURE> m_textureMap;

	/// �u���b�N�ǂݍ��ݎ��Ɏg�p
	int numx, numy;

	/// �u���b�N�e�N�X�`���̔z��
	TEXTURE m_tex[MAX_TEXTURE];

	/// �|���S��
	D2D_POLYGON m_polygon;

	/**
	* @brief �󂫔z��̌���
	*/
	int FindEmptyArray();
};

#endif