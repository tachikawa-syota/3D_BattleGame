/**
* @file   GUIManager.h
* @brief  グラフィカルユーザインタフェース管理する
* @author Syota Tachikawa
*/

#ifndef ___GUIMANAGER_H
#define ___GUIMANAGER_H

#include "..\..\Common\FileData.h"
#include "..\..\Common\STDXDefine.h"
#include "..\..\Common\STDXStruct.h"
#include "..\..\GameSystem\DamageData.h"
#include "..\..\Character\IPlayer.h"

// 前方宣言
class Timer;
class Stock;
class Camera;

/**
* @brief グラフィカルインターフェイスクラス
*/
class GUI
{
public:
	/**
	* @brief コンストラクタ
	*/
	GUI(int playerCount);

	/**
	* @brief デストラクタ
	*/
	~GUI();

	/**
	* @brief 描画
	*/
	void Render(shared_ptr<Timer>& time, shared_ptr<Stock>& stock, vector<IPlayerPtr> player, CameraPtr camera, DamageData damage);

private:
	/// グラフィックスマネージャ
	TextureManager* m_2dTex;

	/// ファイルデータ
	FileData* m_fileData;

	/// 数字テクスチャ
	int m_numberTex[NUMBER_TEXTURE_MAX];

	/// プレイヤーテキストテクスチャ
	int m_playerTextTex[PLAYER_TEXTURE_MAX];
	
	/// プレイヤーの数
	int m_playerCount;

	// 描画位置
	vector<Vector2> m_playerTextPos;
	vector<Vector2> m_stockPos;
	vector<Vector2> m_damagePos;

	/// ファイトテクスチャアニメ
	int m_fightAnimation;

	/// フレーム
	DWORD m_frame;
};

#endif