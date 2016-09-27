/**
* @file   Object2D.h
* @brief  2Dオブジェクトを管理する
* @author Syota Tachikawa
*/

#ifndef ___OBJECT2D_H
#define ___OBJECT2D_H

#include "IObject.h"

/**
* @brief 2Dオブジェクトを管理するクラス
*/
class Object2D
{
public:
	/**
	* @brief インスタンスを取得する（シングルトン）
	*/
	static Object2D* GetInstance(){
		static Object2D singleton;
		return &singleton;
	}

	/**
	* @brief 更新
	*/
	void Update();

	/**
	* @brief 描画
	*/
	void Render();

	/**
	* @brief 前に追加
	*/
	void AddFront(const I2DObjectPtr& obj);

	/**
	* @brief 後ろに追加
	*/
	void AddBack(const I2DObjectPtr& obj);

	/**
	* @brief 削除
	*/
	void Clear();

private:
	/// 2Dオブジェクトリスト
	list<I2DObjectPtr> m_objList;
};

#endif