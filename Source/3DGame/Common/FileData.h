/**
* @file   FileData.h
* @brief  キャラクター"ロック"を管理する
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
* @brief サウンドファイルやテクスチャファイルデータを管理する
*/
class FileData
{
public:
	/**
	* @brief インスタンスを取得する（シングルトン）
	*/
	static FileData* GetInstance(){
		static FileData singleton;
		return &singleton;
	}

	/**
	* @brief テクスチャデータを読み込む
	*/
	HRESULT LoadTexture();

	/**
	* @brief サウンドデータを読み込む
	*/
	HRESULT LoadSound();

	/**
	* @brief モデルデータを読み込む
	*/
	HRESULT LoadModel();

	/**
	* @brief 読み込んだテクスチャを取得
	*/
	int GetTexture(int index) { return m_texture[index]; }
	
	/**
	* @brief 読み込んだテクスチャを取得
	*/
	LPDIRECT3DTEXTURE9& GetD3Texture(int index) { return m_d3Texture[index]; }

	/**
	* @brief 読み込んだブロック型の数字テクスチャを取得
	*/
	int GetNumberTexture(int index){ return numberTex[index]; }

	/**
	* @brief 読み込んだブロック型のヒットエフェクトを取得
	*/
	int GetHitEffectTexture(int index){ return hitEffect[index]; }

	/**
	* @brief 読み込んだブロック型のプレイヤーフォントテクスチャを取得
	*/
	int GetPlayerTexture(int index) { return m_playerTextTex[index]; }

	/**
	* @brief 読み込んだブロック型のリザルトランクテクスチャを取得
	*/
	int GetResultRankTexture(int index) { return m_resultRankTex[index]; }

	/**
	* @brief 読み込んだブロック型の死亡エフェクトテクスチャを取得
	*/
	int GetDeadEffectTexture(int index){ return m_deadEffectTex[index]; }

	/**
	* @brief 読み込んだブロック型の「Start」「Finish」テクスチャを取得
	*/
	int GetStartFinishTexture(int index){ return m_startFinishTex[index]; }

	/**
	* @brief モデルデータを取得(通常のメッシュ)
	*/
	MeshPtr GetModel(int index) { return m_mesh[index]; }

private:
	/// 通常テクスチャ格納
	int m_texture[TEXTURE_MAX];

	/// D3型テクスチャ格納
	LPDIRECT3DTEXTURE9 m_d3Texture[2];

	/// 数字テクスチャ
	int numberTex[NUMBER_TEXTURE_MAX];

	/// ヒットエフェクト
	int hitEffect[HITEFFECT_ANIM_MAX];

	/// スタートフィニッシュテクスチャ
	int m_startFinishTex[STARTFINISH_TEXTURE_MAX];

	/// プレイヤーテキストテクスチャ
	int m_playerTextTex[PLAYER_TEXTURE_MAX];

	/// リザルトランクテキストテクスチャ
	int m_resultRankTex[RESULT_RANK_TEXTURE_MAX];

	/// 死亡エフェクトテクスチャ
	int m_deadEffectTex[DEADEFFECT_TEXTURE_MAX];

	/// メッシュデータ
	vector<MeshPtr> m_mesh;
};

#endif