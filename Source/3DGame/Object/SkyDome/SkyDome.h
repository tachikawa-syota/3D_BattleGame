/**
* @file   SkyDome.h
* @brief  スカイドーム
* @author Syota Tachikawa
*/

#ifndef ___SKYDOME_H
#define ___SKYDOME_H

#include "..\IObject.h"

/**
* @brief スカイドームクラス
*/
class SkyDome : public ISkyDome
{
public:
	/**
	* @brief コンストラクタ
	*/
	SkyDome();

	/**
	* @brief デストラクタ
	*/
	~SkyDome();

	/**
	* @brief 更新
	*/
	bool Update();

	/**
	* @brief 描画
	*/
	void Render();

	/**
	* @brief 描画
	*/
	void Render(const Matrix& view, const Matrix& proj);

	/**
	* @brief ワールド行列を取得する
	*/
	const Matrix& GetWorld();

	/**
	* @brief アンビエントを取得する
	*/
	const Vector4& GetAmbient();

	/**
	* @brief メッシュの取得
	*/
	const MeshPtr& GetMesh() const;

private:
	/// オブジェクト
	MeshPtr m_obj;

	/// 座標
	Vector3 m_pos;

	/// メッシュのアンビエント
	Vector4 m_ambient;

	/// サブワーク
	int m_subWork;
};

#endif