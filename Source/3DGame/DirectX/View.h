/**
* @file   View.h
* @brief  視点の処理管理する
* @author Syota Tachikawa
*/

#ifndef ___VIEW_H
#define ___VIEW_H

#include "..\Math\Math.h"

/**
* @brief 視点クラス
*/
class View
{
public:
	/**
	* @brief コンストラクタ
	*/
	View(DWORD width, DWORD height);

	/**
	* @brief デストラクタ
	*/
	~View();

	/**
	* @brief 適用
	*/
	void Activate(int type);

	/**
	* @brief 画面クリア
	*/
	void Clear();

	/**
	* @brief 視点設定
	*/
	void Set(const Vector3& pos, const Vector3& target);
	/**
	* @brief 視点設定
	*/
	void Set(float x, float y, float z, float ax, float ay, float az);
	
	/**
	* @brief 位置設定
	*/
	void SetPos(float x, float y, float z);
	
	/**
	* @brief 角度設定
	*/
	void SetAngle(float x, float y, float z);

	/**
	* @brief 投影平面設定
	*/
	void SetViewport();

	/**
	* @brief 投影平面設定
	*/
	void SetViewport(int x, int y, int w, int h);

	/**
	* @brief 投影設定
	*/
	void SetProjection(float FovY, float Near, float Far);

	/**
	* @brief 投影設定
	*/
	void SetProjection(float FovY, float Near, float Far, float asp);
	
	/**
	* @brief 視点取得
	*/
	const Matrix& GetView();

	/**
	* @brief 投影行列取得
	*/
	const Matrix& GetProj();

	/**
	* @brief 視点の逆行列取得
	*/
	const Matrix& GetInverseView();

	/**
	* @brief 座標取得
	*/
	const Vector3& GetPos();

	/**
	* @brief 注視点取得
	*/
	const Vector3& GetTarget();
	
	/**
	* @brief 横幅取得
	*/
	DWORD GetWidth();

	/**
	* @brief 縦幅取得
	*/
	DWORD GetHeight();

private:
	/// 横幅
	DWORD m_width;

	/// 縦幅
	DWORD m_height;

	/// 視点行列
	Matrix m_matView;

	/// 投影行列
	Matrix m_matProj;

	/// 座標
	Vector3 m_pos;

	/// 注視点
	Vector3 m_target;

	/// 表示領域
	D3DVIEWPORT9 m_viewport;

	// 投影パラメータ
	float m_fovY;
	float m_near, m_far;
	float m_aspect;
	RECT m_rViewport;
};

#endif