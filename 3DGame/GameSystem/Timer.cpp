#include "Timer.h"

/**
* @brief コンストラクタ
*/
Timer::Timer()
{
	// パラメータ初期化
	m_time = 99;
	m_timeTensPlace = 9;
	m_timeOnesPlace = 9;
}

/**
* @brief デストラクタ
*/
Timer::~Timer()
{

}

/**
* @brief 更新
*/
void Timer::Update()
{
	static short frame = 0;

	// ０だったら処理をしない
	if (m_timeTensPlace == 0 && m_timeOnesPlace == 0) return;

	// 60フレームに1回
	if (frame > 60)
	{
		// 1の位が"0"のとき
		if (m_timeOnesPlace == 0){
			// 10の位を減らして
			m_timeTensPlace--;
			// 1の位を"9"にする
			m_timeOnesPlace = 9;
		}

		// それ以外は
		else{
			// 1の位を減らす
			m_timeOnesPlace--;
		}

		// "0"に戻す
		frame = 0;
		// タイムを減算する
		m_time--;
	}

	// フレーム加算
	frame++;
}

/**
* @brief タイムの取得
*/
DWORD Timer::GetTime() const
{
	return m_time;
}

/**
* @brief タイムの"10"の位取得
*/
int Timer::GetTenId() const
{
	return m_timeTensPlace;
}

/**
* @brief タイムの"1"の位取得
*/
int Timer::GetOneId() const
{
	return m_timeOnesPlace;
}