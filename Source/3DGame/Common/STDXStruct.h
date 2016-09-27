/**
* @file   STDXStruct.h
* @brief  構造体を管理する
* @author Syota Tachikawa
*/

#ifndef ___STDXSTRUCT_H
#define ___STDXSTRUCT_H

#include "CommonLib.h"
#include "STDXEnum.h"

/**
* @struct DeviceState
* @brief デバイスの状態
*/
struct DeviceState
{
	DeviceState() : flag(NULL), prevFlag(NULL), axis(NULL){};

	/// 現在のフラグ
	unsigned int flag;

	/// 一つ前のフラグ
	unsigned int prevFlag;

	/// 移動に使うフラグが入る
	unsigned int axis;
};


/**
* @struct Vector2
* @brief ２次元ベクトル
*/
struct Vector2
{
	Vector2() : x(0), y(0) {};

	Vector2(float in) : x(in), y(in) {};

	Vector2(float x, float y) : x(x), y(y) {};

	Vector2 operator+(const Vector2& rh)
	{
		return Vector2(x + rh.x, y + rh.y);
	}

	Vector2 operator+=(const Vector2& rh)
	{
		x += rh.x;
		y += rh.y;
		return *this;
	}

	Vector2 operator-(const Vector2& rh)
	{
		return Vector2(x - rh.x, y - rh.y);
	}

	Vector2 operator-=(const Vector2& rh)
	{
		x -= rh.x;
		y -= rh.y;
		return *this;
	}

	Vector2 operator*(const float& rh)
	{
		return Vector2(x * rh, y * rh);
	}

	Vector2 operator*=(const float& rh)
	{
		x *= rh;
		y *= rh;
		return *this;
	}

	Vector2 operator/(const float& rh)
	{
		return Vector2(x / rh, y / rh);
	}

	Vector2 operator/=(const float& rh)
	{
		x /= rh;
		y /= rh;
		return *this;
	}

	float x;
	float y;
};


/**
* @struct Rect
* @brief 矩形
*/
struct Rect
{
	Rect() : left(0), top(0), right(0), bottom(0) {};

	Rect(float in) : left(in), top(in), right(in), bottom(in) {};

	Rect(float left, float top, float right, float bottom) :
		left(left), top(top), right(right), bottom(bottom) {};

	Rect operator+(const Rect& rh)
	{
		return Rect(left + rh.left, top + rh.top, right + rh.right, bottom + rh.bottom);
	}

	Rect operator+=(const Rect& rh)
	{
		left += rh.left;
		top += rh.top;
		right += rh.right;
		bottom += rh.bottom;
		return *this;
	}

	Rect operator-(const Rect& rh)
	{
		return Rect(left - rh.left, top - rh.top, right - rh.right, bottom - rh.bottom);
	}

	Rect operator-=(const Rect& rh)
	{
		left -= rh.left;
		top -= rh.top;
		right -= rh.right;
		bottom -= rh.bottom;
		return *this;
	}

	Rect operator*(const float& rh)
	{
		return Rect(left * rh, top * rh, right * rh, bottom * rh);
	}

	Rect operator*=(const float& rh)
	{
		left *= rh;
		top *= rh;
		right *= rh;
		bottom *= rh;
		return *this;
	}

	Rect operator/(const float& rh)
	{
		return Rect(left / rh, top / rh, right / rh, bottom / rh);
	}

	Rect operator/=(const float& rh)
	{
		left /= rh;
		top /= rh;
		right /= rh;
		bottom /= rh;
		return *this;
	}

	float left;
	float top;
	float right;
	float bottom;
};

/**
* @struct Object2DPacket
* @brief 2Dの描画に必要なものが入っている
*/
struct Object2DPacket
{
	Object2DPacket()
	{
		scale = 1.0f;
	};
	Object2DPacket(const Direction& direction, const float scale, const float rot, const int alpha, const Vector2& pos, const bool reverse) :
		direction(direction), scale(scale), rot(rot), alpha(alpha), pos(pos), isReverse(reverse)
	{};

	/// 向き
	Direction direction;

	/// 大きさ
	float scale;

	/// 回転角度（ラジアン）
	float rot;

	/// 反転
	bool isReverse;

	/// アルファ値
	int alpha;

	/// 描画に用いるハンドル
	int handle;

	/// 位置
	Vector2 pos;

};

#endif