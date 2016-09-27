/**
* @file SoundPlayer.h
* @brief サウンドを管理する
* @author Syota Tachikawa
*/

#ifndef ___SOUNDPLAYER_H
#define ___SOUNDPLAYER_H

#include "DirectSound.h"

/**
* @brief サウンドを再生するクラス
*/
class SoundPlayer
{
public:
	/**
	* @brief ゲットインスタンス
	*/
	static SoundPlayer* GetInstance(){
		static SoundPlayer singleton;
		return &singleton;
	}

	/**
	* @brief 初期化
	*/
	HRESULT Initialize();

	/**
	* @brief ロード
	* @param サウンドファイル名
	*/
	BOOL Load(LPSTR fileName){
		return m_dSound->Load(fileName);
	}

	/**
	* @brief 再生
	* @param サウンドファイル名
	*/
	HRESULT Play(LPSTR fileName){
		return m_dSound->Play(fileName);
	}

	/**
	* @brief 停止
	* @param サウンドファイル名
	*/
	HRESULT Stop(LPSTR fileName){
		return m_dSound->Stop(fileName);
	}

	/**
	* @brief ループ再生
	* @param サウンドファイル名
	*/
	HRESULT LoopPlay(LPSTR fileName){
		return m_dSound->LoopPlay(fileName);
	}

	/**
	* @brief 再生中かどうかをチェックする
	* @param サウンドファイル名
	*/
	BOOL IsPlay(LPSTR fileName){
		return m_dSound->IsPlay(fileName);
	}

private:
	/// ダイレクトサウンド
	unique_ptr<DirectSound> m_dSound;
};

#endif