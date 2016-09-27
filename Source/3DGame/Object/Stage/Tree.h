/**
* @file  Tree.h
* @brief ステージ１に登場する木オブジェクト
* @author Syota Tachikawa
*/

#ifndef ___TREE_H
#define ___TREE_H

#include "..\IObject.h"

/// 木オブジェクトの最大数
const int TREE_OBJECT_MAX = 2;

/**
* @brief ステージ１の木オブジェクトクラス
*/
class Tree : public IMeshObject
{
public:
	/**
	* @brief コンストラクタ
	*/
	Tree(const Vector3& pos);

	/**
	* @brief デストラクタ
	*/
	~Tree();

	/**
	* @brief 更新
	*/
	bool Update();

	/**
	* @brief 描画
	*/
	void Render(const Matrix& view, const Matrix& proj);

	/**
	* @brief メッシュ情報を取得する
	*/
	const MeshPtr& GetMesh() const;

private:
	/// 木オブジェクト
	MeshPtr m_obj;

	// 座標
	Vector3 m_pos;
};

/// 木オブジェクトのスマートポインタ
using TreePtr = shared_ptr<Tree>;

#endif