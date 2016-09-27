/**
* @file   Stage1.h
* @brief ステージ1
* @author Syota Tachikawa
*/

#ifndef ___STAGE1_H
#define ___STAGE1_H

#include "Tree.h"

/**
* @brief ステージ１クラス
*/
class Stage1 : public IStage
{
public:
	/**
	* @brief コンストラクタ
	*/
	Stage1();

	/**
	* @brief デストラクタ
	*/
	~Stage1();

	/**
	* @brief 更新
	*/
	bool Update();

	/**
	* @brief 描画
	*/
	void Render(const Matrix& view, const Matrix& proj);

	/**
	* @brief 座標の取得
	*/
	const Vector3& GetPos() const;

	/**
	* @brief ステージメッシュの取得
	*/
	const MeshPtr& GetMesh() const;

private:
	/// ステージオブジェクト
	MeshPtr m_stage;

	/// 位置
	Vector3 m_pos;
};

#endif