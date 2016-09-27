/**
* @file   FileData.h
* @brief  �L�����N�^�["���b�N"���Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___FILEDATA_H
#define ___FILEDATA_H

#include "..\DirectX\SoundPlayer.h"
#include "..\DirectX\TextureManager.h"
#include "..\DirectX\AnimMesh.h"
#include "StringDefine.h"

#define TEXTURE_MAX 256
#define HITEFFECT_ANIM_MAX 9
#define NUMBER_TEXTURE_MAX 10
#define STARTFINISH_TEXTURE_MAX 2
#define PLAYER_TEXTURE_MAX 4
#define RESULT_RANK_TEXTURE_MAX 4
#define DEADEFFECT_TEXTURE_MAX 10
 
/**
* @brief �T�E���h�t�@�C����e�N�X�`���t�@�C���f�[�^���Ǘ�����
*/
class FileData
{
public:
	/**
	* @brief �C���X�^���X���擾����i�V���O���g���j
	*/
	static FileData* GetInstance(){
		static FileData singleton;
		return &singleton;
	}

	/**
	* @brief �e�N�X�`���f�[�^��ǂݍ���
	*/
	HRESULT LoadTexture();

	/**
	* @brief �T�E���h�f�[�^��ǂݍ���
	*/
	HRESULT LoadSound();

	/**
	* @brief ���f���f�[�^��ǂݍ���
	*/
	HRESULT LoadModel();

	/**
	* @brief �ǂݍ��񂾃e�N�X�`�����擾
	*/
	int GetTexture(int index) { return m_texture[index]; }
	
	/**
	* @brief �ǂݍ��񂾃e�N�X�`�����擾
	*/
	LPDIRECT3DTEXTURE9& GetD3Texture(int index) { return m_d3Texture[index]; }

	/**
	* @brief �ǂݍ��񂾃u���b�N�^�̐����e�N�X�`�����擾
	*/
	int GetNumberTexture(int index){ return numberTex[index]; }

	/**
	* @brief �ǂݍ��񂾃u���b�N�^�̃q�b�g�G�t�F�N�g���擾
	*/
	int GetHitEffectTexture(int index){ return hitEffect[index]; }

	/**
	* @brief �ǂݍ��񂾃u���b�N�^�̃v���C���[�t�H���g�e�N�X�`�����擾
	*/
	int GetPlayerTexture(int index) { return m_playerTextTex[index]; }

	/**
	* @brief �ǂݍ��񂾃u���b�N�^�̃��U���g�����N�e�N�X�`�����擾
	*/
	int GetResultRankTexture(int index) { return m_resultRankTex[index]; }

	/**
	* @brief �ǂݍ��񂾃u���b�N�^�̎��S�G�t�F�N�g�e�N�X�`�����擾
	*/
	int GetDeadEffectTexture(int index){ return m_deadEffectTex[index]; }

	/**
	* @brief �ǂݍ��񂾃u���b�N�^�́uStart�v�uFinish�v�e�N�X�`�����擾
	*/
	int GetStartFinishTexture(int index){ return m_startFinishTex[index]; }

	/**
	* @brief ���f���f�[�^���擾(�ʏ�̃��b�V��)
	*/
	MeshPtr GetModel(int index) { return m_mesh[index]; }

private:
	/// �ʏ�e�N�X�`���i�[
	int m_texture[TEXTURE_MAX];

	/// D3�^�e�N�X�`���i�[
	LPDIRECT3DTEXTURE9 m_d3Texture[2];

	/// �����e�N�X�`��
	int numberTex[NUMBER_TEXTURE_MAX];

	/// �q�b�g�G�t�F�N�g
	int hitEffect[HITEFFECT_ANIM_MAX];

	/// �X�^�[�g�t�B�j�b�V���e�N�X�`��
	int m_startFinishTex[STARTFINISH_TEXTURE_MAX];

	/// �v���C���[�e�L�X�g�e�N�X�`��
	int m_playerTextTex[PLAYER_TEXTURE_MAX];

	/// ���U���g�����N�e�L�X�g�e�N�X�`��
	int m_resultRankTex[RESULT_RANK_TEXTURE_MAX];

	/// ���S�G�t�F�N�g�e�N�X�`��
	int m_deadEffectTex[DEADEFFECT_TEXTURE_MAX];

	/// ���b�V���f�[�^
	vector<MeshPtr> m_mesh;
};

#endif