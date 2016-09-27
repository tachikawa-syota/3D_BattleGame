/**
* @file   STDXStruct.h
* @brief  �\���̂��Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___STDXSTRUCT_H
#define ___STDXSTRUCT_H

#include "CommonLib.h"
#include "STDXEnum.h"

/**
* @struct DeviceState
* @brief �f�o�C�X�̏��
*/
struct DeviceState
{
	DeviceState() : flag(NULL), prevFlag(NULL), axis(NULL){};

	/// ���݂̃t���O
	unsigned int flag;

	/// ��O�̃t���O
	unsigned int prevFlag;

	/// �ړ��Ɏg���t���O������
	unsigned int axis;
};


/**
* @struct Vector2
* @brief �Q�����x�N�g��
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
* @brief ��`
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
* @brief 2D�̕`��ɕK�v�Ȃ��̂������Ă���
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

	/// ����
	Direction direction;

	/// �傫��
	float scale;

	/// ��]�p�x�i���W�A���j
	float rot;

	/// ���]
	bool isReverse;

	/// �A���t�@�l
	int alpha;

	/// �`��ɗp����n���h��
	int handle;

	/// �ʒu
	Vector2 pos;

};

#endif