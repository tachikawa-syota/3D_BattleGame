/**
* @file DirectSound.h
* @brief ダイレクトサウンド
* @author Syota Tachikawa
*/

#ifndef ___DIRECTSOUND_H
#define ___DIRECTSOUND_H

#include <dsound.h>
#include "..\Common\CommonLib.h"

/**
* @brief ダイレクトサウンドクラス
*/
class DirectSound
{
public:
	/**
	* @brief コンストラクタ
	*/
	DirectSound();

	/**
	* @brief デストラクタ
	*/
	~DirectSound();

	/**
	* @brief ロード
	* @param ロードするサウンドファイルの名前
	*/
	BOOL Load(LPSTR fileName);

	/**
	* @brief 再生
	* @param サウンドファイル名
	*/
	HRESULT Play(LPSTR fileName);

	/**
	* @brief 停止
	* @param サウンドファイル名
	*/
	HRESULT Stop(LPSTR fileName);

	/**
	* @brief ループ再生
	* @param サウンドファイル名
	*/
	HRESULT LoopPlay(LPSTR fileName);

	/**
	* @brief 再生中かどうかをチェックする
	* @param サウンドファイル名
	*/
	BOOL IsPlay(LPSTR fileName);

	/**
	* @brief 音量の設定
	* @param サウンドファイル名
	*/
	void SetVolume(LPSTR fileName, int volume);

	/**
	* @brief 指定したサウンドを解放する
	* @param サウンドファイル名
	*/
	void SoundSingleFree(LPSTR fileName);

private:
	/// サウンドデバイス
	LPDIRECTSOUND8 pDS8;

	/// サウンドバッファー
	map<string, LPDIRECTSOUNDBUFFER> m_soundBuffer;


};

#endif