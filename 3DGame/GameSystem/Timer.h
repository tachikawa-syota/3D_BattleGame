/**
* @file   Timer.h
* @brief  タイマー処理を管理する
* @author Syota Tachikawa
*/

#ifndef ___TIMER_H
#define ___TIMER_H

#include "..\Common\FileData.h"

/**
* @brief バトル中に使用するタイマークラス
*/
class Timer
{
public:
	/**
	* @brief コンストラクタ
	*/
	Timer();

	/**
	* @brief デストラクタ
	*/
	~Timer();

	/**
	* @brief 更新
	*/
	void Update();

	/**
	* @brief タイムの"10"の位取得
	*/
	int GetTenId() const;

	/**
	* @brief タイムの"1"の位取得
	*/
	int GetOneId() const;

	/**
	* @brief タイムの取得
	*/
	DWORD GetTime() const;

private:
	/// タイマーの1の位
	int m_timeOnesPlace;

	/// タイマーの10の位
	int m_timeTensPlace;

	/// タイム
	DWORD m_time;
};

#endif