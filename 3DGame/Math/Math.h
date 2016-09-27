/**
* @file   Math.h
* @brief  数学構造体クラスを管理する
* @author Syota Tachikawa
*/

#ifndef ___MATH_H
#define ___MATH_H

#include "..\Common\CommonLib.h"

/// 行列構造体
typedef D3DXVECTOR4	Vector4;
typedef D3DXMATRIX	Matrix;
/// ３次元ベクトル基本構造体
typedef struct Vector { float x, y, z; }Vector;

/**
* @brief ３次元ベクトル
*/
struct Vector3 : public Vector
{
public:
	//コンストラクタ
	Vector3(){ this->x = 0.0f, this->y = 0.0f, this->z = 0.0f; };
	Vector3(float x, float y, float z){ this->x = x, this->y = y, this->z = z; }
	Vector3(const Vector& v){ this->x = v.x, this->y = v.y, this->z = v.z; }
	// 距離計算
	float Length(){ return sqrtf(x*x + y*y + z*z); }
	float LengthSq(){ return x*x + y*y + z*z; }
	// 正規化
	void Normalize(){
		float l = Length();
		if (l != .0f){ x /= l; y /= l; z /= l; }
	}
	// 代入演算子の定義
	Vector3& operator =  (const Vector& v)	{ x = v.x; y = v.y; z = v.z; return *this; }
	Vector3& operator += (const Vector3& v)	{ x += v.x; y += v.y; z += v.z; return *this; }
	Vector3& operator -= (const Vector3& v)	{ x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3& operator *= (float v)			{ x *= v; y *= v; z *= v; return *this; }
	Vector3& operator /= (float v)			{ x /= v; y /= v; z /= v; return *this; }

	Vector3 operator + (void) const { Vector3 ret(x, y, z); return ret; }
	Vector3 operator - (void) const { Vector3 ret(-x, -y, -z); return ret; }

	Vector3 operator + (const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
	Vector3 operator - (const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
	Vector3 operator * (float v) const { Vector3 ret(x*v, y*v, z*v); return ret; }
	Vector3 operator / (float v) const { Vector3 ret(x / v, y / v, z / v); return ret; }

	BOOL operator == (const Vector3& v) const { return (x == v.x) && (y == v.y) && (z == v.z); }
	BOOL operator != (const Vector3& v) const { return (x != v.x) || (y != v.y) || (z != v.z); }
};
typedef Vector3 *LPVECTOR3;

/**
* @brief クォータニオンクラス
*/
class Quaternion
{
public:
	// パラメータ
	float	x, y, z, w;
	//コンストラクタ
	Quaternion(){}
	Quaternion(float sx, float sy, float sz, float sw) : x(sx), y(sy), z(sz), w(sw) {}
	//単位クォータニオン生成
	void Identity(){ x = y = z = 0; w = 1; }
	//正規化
	inline void Normalize(){
		float legnth = getLength();
		if (legnth == .0f) return;
		float invL = 1.0f / legnth;
		(*this) *= invL;
	}
	//情報取得
	inline float getLength() const{ return sqrtf(x*x + y*y + z*z + w*w); }
	void toMatrix(Matrix& m);
	//オペレーター
	inline Quaternion operator + () const { Quaternion ret(x, y, z, w); return ret; }
	inline Quaternion operator - () const { Quaternion ret(-x, -y, -z, -w); return ret; }
	//VS クォータニオン
	inline Quaternion& operator +=(const Quaternion& v){ x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	inline Quaternion& operator -=(const Quaternion& v){ x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	inline Quaternion& operator *=(const Quaternion& v){
		x = y * v.z - z * v.y + x * v.w + w * v.x;
		y = z * v.x - x * v.z + y * v.w + w * v.y;
		z = x * v.y - y * v.x + z * v.w + w * v.z;
		w = w * v.w - x * v.x - y * v.y - z * v.z;
		return *this;
	}
	inline Quaternion operator +(const Quaternion& v) const{ return Quaternion(x + v.x, y + v.y, z + v.z, w + v.w); }
	inline Quaternion operator -(const Quaternion& v) const{ return Quaternion(x - v.x, y - v.y, z - v.z, w - v.w); }
	inline Quaternion operator *(const Quaternion& v) const{
		return Quaternion(
			y * v.z - z * v.y + x * v.w + w * v.x,
			z * v.x - x * v.z + y * v.w + w * v.y,
			x * v.y - y * v.x + z * v.w + w * v.z,
			w * v.w - x * v.x - y * v.y - z * v.z
			);
	}
	//VS 値
	inline Quaternion& operator *=(float v){ x *= v; y *= v; z *= v; w *= v; return *this; }
	inline Quaternion& operator /=(float v){ x /= v; y /= v; z /= v; w /= v; return *this; }

	inline Quaternion operator *(float v) const{ return Quaternion(x*v, y*v, z*v, w*v); }
	inline Quaternion operator /(float v) const{ return Quaternion(x / v, y / v, z / v, w / v); }
};


/**
* @brief 外積
* @param out - 出力
* @param v1  - 座標1
* @param v2  - 座標2
*/
inline void Vector3Cross(Vector& out, Vector& v1, Vector& v2)
{
	out.x = v1.y*v2.z - v1.z*v2.y;
	out.y = v1.z*v2.x - v1.x*v2.z;
	out.z = v1.x*v2.y - v1.y*v2.x;
}

/**
* @brief 内積
* @param v1  - 座標1
* @param v2  - 座標2
*/
inline float Vector3Dot(Vector& v1, Vector& v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

/**
* @brief XYZ回転
*/
void SetTransformMatrixXYZ(Matrix *Mat, float x, float y, float z, float ax, float ay, float az);

/**
* @brief ビュー変換表列
* @param mat 行列出力
* @param eye 座標
* @param at  ターゲット
* @param up  視点の高さ
*/
void LookAtLH(Matrix& mat, Vector& eye, Vector& at, Vector& up);

/**
* @brief パース投影変換
*/
void PerspectiveLH(Matrix& mat, float fovY, float aspect, float zn, float zf);

/**
* @brief 平行投影変換
*/
void OlthoLH(Matrix& mat, float w, float h, float zn, float zf);

/**
* @brief 球面線形補間
* @return 計算結果
*/
Quaternion QuaternionSlerp(Quaternion& q, Quaternion& r, float t);

#endif