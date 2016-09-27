#include "Camera.h"
#include "..\DirectX\Direct3DData.h"

/**
* @brief コンストラクタ
*/
Camera::Camera(DWORD width, DWORD height)
{
	//ビューオブジェクト生成
	m_view = make_unique<View>(width, height);
	//カメラ初期座標
	m_pos = Vector3(0.0f, 6.0f, 15.0f);
	//ターゲット
	m_target = Vector3(0.0f, 0.0f, 0.0f);
	//角度
	m_angle = Vector3(0.0f, 3.0f, 0.0f);
	//カメラから注視点までの最大、最小距離
	m_maxDist = MAX_DIST_CAMERA;
	m_minDist = MIN_DIST_CAMERA;
}

/**
* @brief デストラクタ
*/
Camera::~Camera()
{

}

/**
* @brief カメラタイプの切り替え
*/
void Camera::Normal(const Vector3& target)
{
	//位置更新
	Vector3	v = target - m_pos;
	v.y = 0;
	float dist = v.Length();
	//最大距離設定
	if (dist > m_maxDist){ m_pos = target - v / dist * m_maxDist; }
	//最小距離設定
	if (dist < m_minDist){ m_pos = target - v / dist * m_minDist; }
	m_pos.y = target.y + 5.0f;
	//注視点設定
//	target += +Vector3(0, 0, 0);
}

/**
* @brief 更新
*/
void Camera::Update(const Vector3& target)
{
	//カメラ設定(距離、角度)
	Normal(target);
	//設定
	m_view->Set(Vector3(m_pos.x, m_pos.y - 1.0f, m_pos.z - 15.0f), target);

	//カメラ設定
	Active();
}

/**
* @brief 描画準備
*/
void Camera::Active()
{
	//カメラ視点設定
	m_view->Activate(0);

	//画面クリア
	m_view->Clear();
}

/**
* @brief 座標を取得する
*/
const Vector3& Camera::GetPos()
{
	return m_pos;
}

/**
* @brief カメラの行列を取得する
*/
const Matrix& Camera::GetView()
{
	return m_view->GetView();
}

/**
* @brief カメラの逆行列を取得する
*/
const Matrix& Camera::GetInverseView()
{
	return m_view->GetInverseView();
}

/**
* @brief 投影を取得する
*/
const Matrix& Camera::GetProj()
{
	return m_view->GetProj();
}

/**
* @brief 座標を設定する
*/
void Camera::SetPos(const Vector3& pos)
{
	m_pos = pos;
}

/**
* @brief ターゲットを設定する
*/
void Camera::SetTarget(const Vector3& target)
{
	m_target = target;
}

/**
* @brief Y座標を設定する
*/
void Camera::SetY(float y)
{
	m_pos.y = y;
}