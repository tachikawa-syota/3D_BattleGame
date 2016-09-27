/**
* @file   CommonParam.h
* @brief  共通のパラメータを管理する
* @author Syota Tachikawa
*/

#ifndef ___COMMONPARAM_H
#define ___COMMONPARAM_H

#include "..\iniFileLoader\iniFileLoaderBase.h"

// インスタンス取得
#define GetComPrm CommonParamLoader::GetInstance()

/**
* @brief iniファイルで管理しているキャラクターパラメーターを読み込むクラス
*/
class CommonParamLoader : public iniFileLoaderBase
{
public:
	/**
	* @brief インスタンスの取得
	*/
	static CommonParamLoader* GetInstance(){
		static CommonParamLoader singleton;
		return &singleton;
	}

	// 初期座標
	float InitPosY;
	float InitPosZ;
	// プレイヤー１の初期座標
	float InitPlayer1PosX;
	// プレイヤー２の初期座標
	float InitPlayer2PosX;
	// プレイヤー３の初期座標
	float InitPlayer3PosX;
	// プレイヤー４の初期座標
	float InitPlayer4PosX;
	// 重力
	float Gravity;
	// ジャンプの最大数
	int JumpMax;
	// 復帰座標
	float Pattern1ReturnPosX;
	float Pattern2ReturnPosX;
	float Pattern3ReturnPosX;
	float Pattern4ReturnPosX;
	float ReturnPosY;
	/// UI描画位置
	float DamageRatePosX1;
	float DamageRatePosX2;
	float DamageRatePosX3;
	float DamageRatePosX4;
	/// プレイヤーテキスト描画位置
	float PlayerTextPosX;

private:
	/**
	* @brief コンストラクタ
	*/
	CommonParamLoader() :iniFileLoaderBase("Param.ini")
	{
		InitPosY = GetNextParameterFloat();
		InitPosZ = GetNextParameterFloat();
		InitPlayer1PosX = GetNextParameterFloat();
		InitPlayer2PosX = GetNextParameterFloat();
		InitPlayer3PosX = GetNextParameterFloat();
		InitPlayer4PosX = GetNextParameterFloat();
		Gravity = GetNextParameterFloat();
		JumpMax = GetNextParameterInt();
		Pattern1ReturnPosX = GetNextParameterFloat();
		Pattern2ReturnPosX = GetNextParameterFloat();
		Pattern3ReturnPosX = GetNextParameterFloat();
		Pattern4ReturnPosX = GetNextParameterFloat();
		ReturnPosY = GetNextParameterFloat();
		DamageRatePosX1 = GetNextParameterFloat();
		DamageRatePosX2 = GetNextParameterFloat();
		DamageRatePosX3 = GetNextParameterFloat();
		DamageRatePosX4 = GetNextParameterFloat();
		PlayerTextPosX = GetNextParameterFloat();
	}
};

#endif