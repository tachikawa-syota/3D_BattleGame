/**
* @file   Camera.h
* @brief  カメラを管理する
* @author Syota Tachikawa
*/

#ifndef ___CAMERA_H
#define ___CAMERA_H

#include "..\DirectX\View.h"

/// カメラと注視点の最大距離
#define MAX_DIST_CAMERA 100.0f
/// カメラと注視点の最小距離
#define MIN_DIST_CAMERA -100.0f

/**
* @brief カメラクラス
*/
class Camera
{
public:
	/**
	* @brief コンストラクタ
	*/
	Camera(DWORD width, DWORD height);
	
	/**
	* @brief デストラクタ
	*/
	~Camera();

	/**
	* @brief 更新
	*/
	void Update(const Vector3& target);
	
	/**
	* @brief 描画準備
	*/
	void Active();

	/**
	* @brief 座標を取得する
	*/
	const Vector3& GetPos();
	
	/**
	* @brief カメラの行列を取得する
	*/
	const Matrix& GetView();

	/**
	* @brief カメラの逆行列を取得する
	*/
	const Matrix& GetInverseView();

	/**
	* @brief 投影を取得する
	*/
	const Matrix& GetProj();

	/**
	* @brief 座標を設定する
	*/
	void SetPos(const Vector3& pos);

	/**
	* @brief ターゲットを設定する
	*/
	void SetTarget(const Vector3& target);

	/**
	* @brief Y座標を設定する
	*/
	void SetY(float y);

private:
	/// ビュー
	unique_ptr<View> m_view;
	
	/// カメラの座標
	Vector3 m_pos;

	/// ターゲットの座標
	Vector3 m_target;

	/// 角度
	Vector3 m_angle;

	/// 最大距離
	float m_maxDist;

	/// 最小距離
	float m_minDist;

	/**
	* @brief カメラタイプの切り替え
	*/
	void Normal(const Vector3& pPos);
};

/// カメラオブジェクトのスマートポインタ
using CameraPtr = shared_ptr<Camera>;

#endif