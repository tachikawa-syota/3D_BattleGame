/**
* @file   CorrectionValue.h
* @brief  補正値を管理する
* @author Syota Tachikawa
*/

#ifndef ___CORRECTIONVALUE_H
#define ___CORRECTIONVALUE_H

#include "..\Character\Rock.h"

/**
* @brief 吹き飛ばし時に使用する補正値を管理する
*/
class CorrectionValue
{
public:
	/**
	* @brief インスタンスを取得する（シングルトン）
	*/
	static CorrectionValue* GetInstance(){
		static CorrectionValue singleton;
		return &singleton;
	}

	/**
	* @brief 初期化
	*/
	void Initialize();

	/**
	* @brief 補正値を取得する
	*/
	float GetValue(const type_info& type, vector<IPlayerPtr>& player, int index) const;

private:
	/// キャラクター"ロック"の補正値
	float m_rockCorValue[3];
};

#endif