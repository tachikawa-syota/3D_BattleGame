/**
* @file   GameMain.h
* @brief  ゲーム中の処理を管理する
* @author Syota Tachikawa
*/

#ifndef ___GAMEMAIN_H
#define ___GAMEMAIN_H

#include "ISequence.h"
// DirectX
#include "..\DirectX\Direct3DSystem.h"
#include "..\DirectX\Direct3DData.h"
#include "..\DirectX\InputMask.h"
#include "..\DirectX\Font.h"
// Object
#include "..\Object\ObjectManager.h"
#include "..\Object\Stage\Tree.h"
#include "..\Object\GUI\GUIManager.h"
// System
#include "..\GameSystem\ScoreManager.h"
#include "..\GameSystem\Computer.h"
// Shader
#include "..\Shader\WaveShader.h"

/**
* @enum GameMainState
* @brief ゲームメインの状態
*/
enum class GameMainState : BYTE
{
	// スタート
	Start = 0,

	// バトル中
	Play,
	
	// 終了
	Finish
};

/**
* @brief ゲームメインクラス
*/
class GameMain : public ISequence
{
public:
	/**
	* @brief 作成
	*/
	void Create();

	/**
	* @brief 初期化
	*/
	void Initialize();

	/**
	* @brief 更新
	*/
	void Update();

	/**
	* @brief 描画
	*/
	void Render();

	/**
	* @brief 終了
	*/
	void Finalize();

private:

#pragma region メンバ変数
	/// オブジェクトマネージャー
	ObjectManager* m_objMgr;

	/// 波シェーダー
	WaveShaderPtr m_waveShader;

	/// パララックスマッピング
	ShaderPtr m_PM;

	/// スカイドームシェーダー
	ShaderPtr m_skyDomeShader;

	/// ライトブルームシェーダー
	ShaderPtr m_lightBloomShader;

	/// スキンテスト
	ShaderPtr m_skinTest;

	/// シェーダーマネージャー
	ShaderManager* m_shaderMgr;

	/// プレイヤー
	vector<IPlayerPtr> m_player;

	/// コンピューター
	vector<Computer> m_computer;

	/// 入力デバイス
	vector<InputPtr> m_dev;

	/// ステージ
	IStagePtr m_stage;

	/// 木
	TreePtr m_tree;

	/// スカイドーム
	ISkyDomePtr m_skyDome;

	/// カメラ
	CameraPtr m_camera;

	/// インターフェイス
	shared_ptr<GUI> m_gui;

	/// タイマー
	shared_ptr<Timer> m_timer;

	/// ストック
	StockPtr m_stock;

	/// スコア管理
	ScoreManagerPtr m_scoreMgr;

	/// フォント
	unique_ptr<Font> m_font;

	/// ゲームメインの状態
	GameMainState m_gameState;

	/// ダメージデータ
	DamageData m_damageData;

	/// デバイスの状態
	vector<DeviceState> m_deviceState;

	/// 前フレームのカメラ座標(ポーズ画面で使用する)
	Vector3 m_oldCameraPos;

	/// ゲーム用のフレーム
	UINT m_gameFrame;

	/// サブワーク
	int m_subWork;

	/// 対戦人数
	int m_playerNum;

	/// 順位
	int m_rank;

	/// StartFinishテクスチャをコピーする変数
	int m_startFinishTex[STARTFINISH_TEXTURE_MAX];

	/// ポーズフラグ
	bool m_isPause;

#pragma endregion


#pragma region メンバ関数

	/**
	* @brief リストにオブジェクトを追加する
	*/
	void AddObject();

	/**
	* @brief シェーダーを作成する
	*/
	void CreateShader();

	/**
	* @brief 雪を降らす
	*/
	void UpdateSnowParticle();

	/**
	* @brief アイテムの更新
	*/
	void UpdateItem();

	/**
	* @brief カメラの更新
	*/
	void UpdateCamera();

	/**
	* @brief バトル開始状態の更新
	* @return true - 次の状態に移行：false - 現在の処理を継続
	*/
	bool UpdateGameStart();

	/**
	* @brief バトル状態の更新
	* @return true - 次の状態に移行：false - 現在の処理を継続 
	*/
	bool UpdateGamePlay();

	/**
	* @brief バトル終了の更新
	*/
	void UpdateGameFinish();

	/**
	* @brief 入力デバイスを更新する
	*/
	void UpdateInputDevice();

	/**
	* @brief コンピューターを更新する
	*/
	void UpdateComputer();

	/**
	* @brief キャラクターの更新をする
	*/
	void UpdateCharacter();

	/**
	* @brief タイムモードの更新をする
	*/
	bool UpdateBattleModeTime();

	/**
	* @brief ストックモードの更新をする
	*/
	bool UpdateBattleModeStock();

	/**
	* @brief すべての当たり判定をチェックする
	*/
	void AllCollisionCheck();

	/**
	* @brief 煙エフェクトを生成するかどうかを確認する
	*/
	void CreateCheckSmokeEffect();

	/**
	* @brief デバイスの入力をチェックする
	* @param flag - 入力フラグ
	* @param index - チェックする番号
	*/
	bool CheckDeviceState(unsigned int FLAG, int index);

	/**
	* @brief ポーズの更新
	*/
	bool UpdatePause();

	/**
	* @brief UIの描画
	*/
	void RenderUI(LPDIRECT3DTEXTURE9& bumpMap);

#pragma endregion
};

#endif