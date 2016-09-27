/**
* @file   TextureManager.h
* @brief  2D�e�N�X�`�����Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___TEXTUREMANAGER_H
#define ___TEXTUREMANAGER_H

#include "TextureLib.h"

/**
* @brief �e�N�X�`�����Ǘ�����N���X
*/
class TextureManager
{
private:
	/// �R���X�g���N�^(�V���O���g��)
	TextureManager(){
		m_p2Dtex = nullptr;
	};

	/// 2D�e�N�X�`��
	Texture2D* m_p2Dtex;

	/// �f�o�C�X
	LPDIRECT3DDEVICE9 m_device;

public:
	/**
	* @brief �Q�b�g�C���X�^���X
	*/
	static TextureManager* GetInstance(){
		static TextureManager singleton;
		return &singleton;
	}
	//�f�X�g���N�^
	~TextureManager(){};

	/**
	* @brief ������
	*/
	HRESULT Initialize();


#pragma region �`�惁�\�b�h

	/**
	* @brief �e�N�X�`���𒆐S�ɕ`�悷��
	* @param fileName - �t�@�C���̖��O
	* @param posX - X���W�̎w��
	* @param posY - Y���W�̎w��
	* @param angle - �p�x
	* @param scale - �傫��
	* @param alpha - �A���t�@�l
	* @param add - ���Z�����̗L��
	* @param reverse - ���]���ĕ`�悷�邩
	*
	* @note �g�����F"fileName"��"LoadTexture"�œǂݍ��񂾃f�[�^���w�肵�Ă�������
	*				�e�N�X�`������]���ĕ`�悵�����ꍇ��"angle"�Ɋp�x���w�肵�Ă�������
	*				�p�x�E�傫���E���Z�E���]�Ɋւ��Ă̓f�t�H���g�ň������w�肵�Ă��܂�
	*/
	HRESULT DrawTexture(string fileName, float posX, float posY, float angle = 0.0f, float scale = 1.0f, int alpha = 255, bool add = false, bool reverse = false){
		return m_p2Dtex->DrawTexture(fileName, posX, posY, angle, scale, alpha, add, reverse);
	}
	/**
	* @brief �u���b�N�^�œǂݍ��񂾃e�N�X�`����`�悷��
	*/
	HRESULT DrawTexture(int index, float posX, float posY, float angle = 0.0f, float scale = 1.0f, int alpha = 255, bool add = false, bool reverse = false){
		return m_p2Dtex->DrawTexture(index, posX, posY, angle, scale, alpha, add, reverse);
	}
	/**
	* @brief ��ʂ����ς��Ƀe�N�X�`����`�悷��
	* @param �e�N�X�`���f�[�^
	* @note �V�F�[�_�[�ŗ��p����
	*/
	HRESULT DrawTexture(LPDIRECT3DTEXTURE9& texture){
		return m_p2Dtex->DrawTexture(texture);
	}

	/**
	* @brief �e�N�X�`���𒆐S�ɕ`�悷��
	* @param x1 - ���[X���W
	* @param y1 - ���[Y���W
	* @param x2 - �E�[X���W
	* @param y2 - �E�[Y���W
	* @param pTex - �e�N�X�`���f�[�^�iD3DLoadTexture�œǂݍ��񂾂�j
	* @param z	- z��
	* @param add - ���Z�����̗L��
	*
	* @note �g�����F"pTex"��"D3DLoadTexture"�œǂݍ��񂾃f�[�^���w�肵�Ă�������
	*/
	HRESULT DrawTextureEx(float x1, float y1, float x2, float y2, const LPDIRECT3DBASETEXTURE9& pTex, float z = 0.0f, bool add = false){
		return m_p2Dtex->DrawTextureEx(x1, y1, x2, y2, pTex, z, add);
	}

	/**
	* @brief 2D�e�N�X�`����3D�`��
	*
	* @param matView �J�����̃}�g���b�N�X
	* @param fileName �e�N�X�`���t�@�C���̖��O
	* @param pos ���W
	* @param angle �p�x
	* @param scale �傫��
	* @param alpha �A���t�@�l
	*
	* @note - �g�����F���̊֐��͒P�̂œǂݍ��񂾃e�N�X�`���Ŏg�p����B
	*                 "fileName"�ɂ̓e�N�X�`���̖��O���w�肵�Ă��������B
	*                 textureMap�Ɋi�[���ꂽ�e�N�X�`�����`�悳��܂��B
	*/
	HRESULT Draw3DTexture(const Matrix& matView, string fileName, const Vector3& pos, const Vector3& angle, float scale, int alpha, bool add = false, bool bAlpha = true) {
		return m_p2Dtex->Draw3DTex(matView, fileName, pos, angle, scale, alpha, add, bAlpha);
	}

	/**
	* @brief 2D�e�N�X�`����3D�`��
	* @param matView �J�����̃r���[�s��
	* @param index �e�N�X�`���̔ԍ�
	* @param pos ���W
	* @param angle �p�x
	* @param scale �傫��
	* @param alpha �A���t�@�l
	*
	* @note - �g�����F���̊֐��̓u���b�N�^�œǂݍ��񂾃e�N�X�`���Ŏg�p����B
	*                 "index"�ɂ̓e�N�X�`���̔ԍ����w�肵�Ă��������B
	*/
	HRESULT Draw3DTexture(const Matrix& matView, int index, const Vector3& pos, const Vector3& angle, float scale, int alpha, bool add = false){
		return m_p2Dtex->Draw3DTex(matView, index, pos, angle, scale, alpha, add);
	}

	/**
	* @brief �e�N�X�`����`��(�~���̑��ʂ̂�)
	* @param matView �J�����̃}�g���b�N�X
	* @param fileName �e�N�X�`���t�@�C���̖��O
	* @param pos ���W
	* @param angle �p�x
	* @param scale �傫��
	* @param radius ���a
	* @param height ����
	* @param alpha �A���t�@�l
	*/
	HRESULT Draw3DBandTexture(const Matrix& matView, string fileName, const Vector3& pos, const Vector3& angle, float scale, float upRadius, float downRadius, float height, int alpha){
		return m_p2Dtex->Draw3DBandTexture(matView, fileName, pos, angle, scale, upRadius, downRadius, height, alpha);
	}

	/**
	* @brief �e�N�X�`����`�悷��i�J���[�̕ύX���\�j
	* @param fileName - �t�@�C���̖��O
	* @param posX - X���W�̎w��
	* @param posY - Y���W�̎w��
	* @param argb - �J���[�f�[�^�̎w��
	* @param add - ���Z�����̗L��
	* @param angle - �p�x
	* @param scale - �傫��
	* @param reverse - ���]���ĕ`�悷�邩
	*/
	HRESULT DrawTexARGB(string fileName, float posX, float posY, ARGB argb, bool add = true, float angle = 0.0f, float scale = 1.0f, bool reverse = false){
		return m_p2Dtex->DrawTexARGB(fileName, posX, posY, argb, add, angle, scale, reverse);
	}

	/**
	* @brief �l�p�`��`�悷��
	*/
	HRESULT DrawBox(float nMinX, float nMinY, float nMaxX, float nMaxY, DWORD color){
		return m_p2Dtex->DrawBox(nMinX, nMinY, nMaxX, nMaxY, color);
	}

	/**
	* @brief 2D�|���S����`��
	*/
	HRESULT Draw2DPolygon(){
		return m_p2Dtex->Draw2DPolygon();
	}


#pragma endregion


#pragma region �ǂݍ��݂ƍ쐬���\�b�h

	/**
	* @brief �e�N�X�`���̓ǂݍ���
	* @param szFileName - �e�N�X�`���t�@�C���̖��O
	* @return �e�N�X�`���̔ԍ�
	*/
	BOOL LoadTexture(string fileName){
		return m_p2Dtex->LoadTexture(fileName);
	}

	/**
	* @brief D3D�^�̃e�N�X�`���̓ǂݍ���
	* @return �ǂݍ��񂾃e�N�X�`��
	*
	* @note�FLPDIRECT3DTEXTURE9�^�Ńe�N�X�`����ǂݍ��݂܂�
	*/
	LPDIRECT3DTEXTURE9 D3LoadTexture(string fileName){
		return m_p2Dtex->LoadD3DTexture(fileName);
	}

	/**
	* @brief �u���b�N�^�̃e�N�X�`���̓ǂݍ���
	* @param szFileName - �e�N�X�`���t�@�C���̖��O
	* @param sizeX - ������������1�����̃e�N�X�`���̉���
	* @param sizeY - ������������1�����̃e�N�X�`���̏c��
	* @param numX - ���ɕ��ׂ��Ă���e�N�X�`���̖���
	* @param numY - �c�ɕ��ׂ��Ă���e�N�X�`���̖���
	* @param numMax - �e�N�X�`���̑�����
	* @param *data - �ǂݍ��񂾃e�N�X�`�����i�[����z��|�C���^
	* @return �e�N�X�`���̔ԍ�
	*
	* @note �g�����F"sizeX,sizeY"��1�����̃e�N�X�`���̕����w�肵�Ă�������
	*				�e�N�X�`���̃T�C�Y���S�ē����T�C�Y�̃��m�łȂ��Ɠǂݍ��݂���肭�����܂���i�o���o���ł��ǂݍ��߂�悤�ɂ�����Ή����������j
	*				"numMax"��"numX"�ɓ��ꂽ�l * numY�ɓ��ꂽ�l"�ɂ��Ă�������
	*				*data�̔z��T�C�Y�ƃe�N�X�`���̖������قȂ�ƃo�O�̗v���ɂȂ�̂Œ���
	*/
	int LoadBlkTexture(string fileName, int sizeX, int sizeY, int numX, int numY, int numMax, int *data){
		return m_p2Dtex->LoadBlkTexture(fileName, sizeX, sizeY, numX, numY, numMax, data);
	}

	/**
	* @brief �e�N�X�`�����쐬
	* @return �쐬�����e�N�X�`���f�[�^
	*
	* @note�F��ɃV�F�[�_�[�Ŏg�p���܂�
	*/
	TEXTURE_DATA& CreateTexture(UINT width, UINT height, DWORD Usage = D3DUSAGE_RENDERTARGET, D3DFORMAT format = D3DFMT_A8R8G8B8){
		return m_p2Dtex->CreateTexture(width, height, Usage, format);
	}

	/**
	* @brief 2D�|���S���𐶐�����
	* @param width - �|���S���̉���
	* @param height - �|���S���̏c��
	*
	* @note �g�����F�쐬�������|���S���̕��������Ɏ������Ă�������
	*               ���V�F�[�_�[�Ŏg�p
	*/
	BOOL Create2DPolygon(UINT width, UINT height){
		return m_p2Dtex->Create2DPolygon(width, height);
	}

#pragma endregion


	/**
	* @brief �e�N�X�`���̉������擾����
	*/
	const DWORD GetWidth(string fileName){
		return m_p2Dtex->GetWidth(fileName);
	}

	/**
	* @brief �e�N�X�`���̉������擾����
	*/
	const DWORD GetWidth(int index){
		return m_p2Dtex->GetWidth(index);
	}

	/**
	* @brief �e�N�X�`���̏c�����擾����
	*/
	const DWORD GetHeight(string fileName){
		return m_p2Dtex->GetHeight(fileName);
	}

	/**
	* @brief �e�N�X�`���̏c�����擾����
	*/
	const DWORD GetHeight(int index){
		return m_p2Dtex->GetHeight(index);
	}

	/**
	* @brief 2D�|���S���̉������擾����
	*/
	const UINT Get2DPolygonWidth(){
		return m_p2Dtex->Get2DPolygonWidth();
	}

	/**
	* @brief 2D�|���S���̏c�����擾����
	*/
	const UINT Get2DPolygonHeight(){
		return m_p2Dtex->Get2DPolygonHeight();
	}
};

#endif