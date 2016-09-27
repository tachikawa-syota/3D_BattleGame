/**
* @file   Math.h
* @brief  ���w�\���̃N���X���Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___MATH_H
#define ___MATH_H

#include "..\Common\CommonLib.h"

/// �s��\����
typedef D3DXVECTOR4	Vector4;
typedef D3DXMATRIX	Matrix;
/// �R�����x�N�g����{�\����
typedef struct Vector { float x, y, z; }Vector;

/**
* @brief �R�����x�N�g��
*/
struct Vector3 : public Vector
{
public:
	//�R���X�g���N�^
	Vector3(){ this->x = 0.0f, this->y = 0.0f, this->z = 0.0f; };
	Vector3(float x, float y, float z){ this->x = x, this->y = y, this->z = z; }
	Vector3(const Vector& v){ this->x = v.x, this->y = v.y, this->z = v.z; }
	// �����v�Z
	float Length(){ return sqrtf(x*x + y*y + z*z); }
	float LengthSq(){ return x*x + y*y + z*z; }
	// ���K��
	void Normalize(){
		float l = Length();
		if (l != .0f){ x /= l; y /= l; z /= l; }
	}
	// ������Z�q�̒�`
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
* @brief �N�H�[�^�j�I���N���X
*/
class Quaternion
{
public:
	// �p�����[�^
	float	x, y, z, w;
	//�R���X�g���N�^
	Quaternion(){}
	Quaternion(float sx, float sy, float sz, float sw) : x(sx), y(sy), z(sz), w(sw) {}
	//�P�ʃN�H�[�^�j�I������
	void Identity(){ x = y = z = 0; w = 1; }
	//���K��
	inline void Normalize(){
		float legnth = getLength();
		if (legnth == .0f) return;
		float invL = 1.0f / legnth;
		(*this) *= invL;
	}
	//���擾
	inline float getLength() const{ return sqrtf(x*x + y*y + z*z + w*w); }
	void toMatrix(Matrix& m);
	//�I�y���[�^�[
	inline Quaternion operator + () const { Quaternion ret(x, y, z, w); return ret; }
	inline Quaternion operator - () const { Quaternion ret(-x, -y, -z, -w); return ret; }
	//VS �N�H�[�^�j�I��
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
	//VS �l
	inline Quaternion& operator *=(float v){ x *= v; y *= v; z *= v; w *= v; return *this; }
	inline Quaternion& operator /=(float v){ x /= v; y /= v; z /= v; w /= v; return *this; }

	inline Quaternion operator *(float v) const{ return Quaternion(x*v, y*v, z*v, w*v); }
	inline Quaternion operator /(float v) const{ return Quaternion(x / v, y / v, z / v, w / v); }
};


/**
* @brief �O��
* @param out - �o��
* @param v1  - ���W1
* @param v2  - ���W2
*/
inline void Vector3Cross(Vector& out, Vector& v1, Vector& v2)
{
	out.x = v1.y*v2.z - v1.z*v2.y;
	out.y = v1.z*v2.x - v1.x*v2.z;
	out.z = v1.x*v2.y - v1.y*v2.x;
}

/**
* @brief ����
* @param v1  - ���W1
* @param v2  - ���W2
*/
inline float Vector3Dot(Vector& v1, Vector& v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

/**
* @brief XYZ��]
*/
void SetTransformMatrixXYZ(Matrix *Mat, float x, float y, float z, float ax, float ay, float az);

/**
* @brief �r���[�ϊ��\��
* @param mat �s��o��
* @param eye ���W
* @param at  �^�[�Q�b�g
* @param up  ���_�̍���
*/
void LookAtLH(Matrix& mat, Vector& eye, Vector& at, Vector& up);

/**
* @brief �p�[�X���e�ϊ�
*/
void PerspectiveLH(Matrix& mat, float fovY, float aspect, float zn, float zf);

/**
* @brief ���s���e�ϊ�
*/
void OlthoLH(Matrix& mat, float w, float h, float zn, float zf);

/**
* @brief ���ʐ��`���
* @return �v�Z����
*/
Quaternion QuaternionSlerp(Quaternion& q, Quaternion& r, float t);

#endif