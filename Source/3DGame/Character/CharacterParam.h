/**
* @file   CharacterParam.h
* @brief  キャラクターのパラメータを管理する
* @author Syota Tachikawa
*/

#ifndef ___CHARACTERPARAM_H
#define ___CHARACTERPARAM_H

#include "..\iniFileLoader\iniFileLoaderBase.h"

// インスタンス取得
#define GetChrPrm CharacterParamLoader::GetInstance()

/**
* @brief iniファイルで管理しているキャラクターパラメーターを読み込むクラス
*/
class CharacterParamLoader : public iniFileLoaderBase
{
public:
	/**
	* @brief インスタンスの取得
	*/
	static CharacterParamLoader* GetInstance(){
		static CharacterParamLoader singleton;
		return &singleton;
	}

// キャラクター"ロック" ------------------------------
	// ロックの大きさ
	float RockScale;
	// ロックの攻撃力
	int RockNormalAttackPower;
	// ロックのスマッシュ攻撃力
	int RockSmashAttackPower;
	// ロックの上必殺技の攻撃力
	int RockSpecialAttackUpPower;

private:
	/**
	* @brief コンストラクタ
	*/
	CharacterParamLoader() :iniFileLoaderBase("CharacterParam.ini")
	{
		RockScale = GetNextParameterFloat();
		RockNormalAttackPower = GetNextParameterInt();
		RockSmashAttackPower = GetNextParameterInt();
		RockSpecialAttackUpPower = GetNextParameterInt();
	}
};

#endif