/**
* @file   SingletonOwner.h
* @brief  シングルトンをまとめて管理する
* @author Syota Tachikawa
*/

#ifndef ___SINGLETONOWNER_H
#define ___SINGLETONOWNER_H

#include "..\DirectX\DeviceManager.h"
#include "..\DirectX\SoundPlayer.h"
#include "..\Sequence\ScreenEffect.h"
#include "..\Sequence\SequenceChanger.h"
#include "..\GameSystem\CorrectionValue.h"
#include "..\Shader\ShaderManager.h"

/**
* @brief シングルトンをまとめて管理するクラス
*/
class SingletonOwner
{
public:
	/**
	* @brief 初期化
	* @return true - 成功 ： false - 失敗
	*/
	bool Initialize();

private:
	/// デバイス
	DeviceManager* m_deviceManager;

	/// グラフィックス
	TextureManager* m_textureManager;

	/// サウンド
	SoundPlayer* m_soundPlayer;

	/// シェーダー
	ShaderManager* m_shaderManager;

	/// シーンチェンジャー
	SequenceChanger* m_sequenceChanger;

	/// スクリーンエフェクト
	ScreenEffect* m_screenEffect;

	/// 補正値データ
	CorrectionValue* m_correctionValue;
};

#endif