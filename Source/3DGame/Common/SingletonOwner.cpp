#include "SingletonOwner.h"

/**
* @brief 初期化
*/
bool SingletonOwner::Initialize()
{
	m_deviceManager = DeviceManager::GetInstance();
	// デバイスマネージャーの初期化
	if (FAILED(m_deviceManager->Initialize())){
		return false;
	}
	// デバイスマネージャーのセットアップ
	if (FAILED(m_deviceManager->SetUp())){
		return false;
	}

	// テクスチャマネージャーの初期化
	m_textureManager = TextureManager::GetInstance();
	if (FAILED(m_textureManager->Initialize())){
		return false;
	}

	// サウンドプレイヤーの初期化
	m_soundPlayer = SoundPlayer::GetInstance();
	if (FAILED(m_soundPlayer->Initialize())){
		return false;
	}

	// シェーダーマネージャーの初期化
	m_shaderManager = ShaderManager::GetInstance();
	if (FAILED(m_shaderManager->Create())){
		return false;
	}

	// シークエンスチェンジャーの初期化
	m_sequenceChanger = SequenceChanger::GetInstance();
	m_sequenceChanger->Initialize();

	// スクリーンエフェクトの初期化
	m_screenEffect = ScreenEffect::GetInstance();
	m_screenEffect->Initialize();

	// 吹き飛ばし補正値クラスの初期化
	m_correctionValue = CorrectionValue::GetInstance();
	m_correctionValue->Initialize();

	return true;
}