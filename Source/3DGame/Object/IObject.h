/**
* @file   IObject.h
* @brief  オブジェクトのインターフェイス
* @author Syota Tachikawa
*/

#ifndef ___IOBJECT_H
#define ___IOBJECT_H

#include "..\DirectX\AnimMesh.h"

/**
* @interface IObject
* @brief 3Dオブジェクトインターフェイス
*
* @note：全ての3Dゲームオブジェクトはこのインターフェイスを継承して使う
*/
__interface IObject
{
	/**
	* @brief 更新
	* @return true - 更新 ： false - オブジェクト削除
	*/
	bool Update();

	/**
	* @brief 描画(シェーダーを使用する)
	* @param view - ビュー行列
	* @param proj - 投影行列
	*/
	void Render(const Matrix& view, const Matrix& proj);
};

/**
* @interface IMeshObject
* @brief 3D剛体メッシュオブジェクトインターフェイス
*
* @note：全ての3D剛体ゲームオブジェクトはこのインターフェイスを継承して使う
*/
__interface IMeshObject : public IObject
{
	/**
	* @brief メッシュの取得
	*/
	const MeshPtr& GetMesh() const;
};

/**
* @interface ISkinObject
* @brief 3Dスキンメッシュオブジェクトインターフェイス
*
* @note：全ての3Dスキンゲームオブジェクトはこのインターフェイスを継承して使う
*/
__interface ISkinObject : public IObject
{
	/**
	* @brief スキンメッシュの取得
	*/
	const SkinMeshPtr& GetSkinMesh() const;
};

/// 剛体メッシュオブジェクトのスマートポインタ
using IMeshObjectPtr = shared_ptr<IMeshObject>;
/// スキンメッシュオブジェクトのスマートポインタ
using ISkinObjectPtr = shared_ptr<ISkinObject>;

/**
* @interface Item
* @brief アイテムインターフェイス
*/
__interface IItem : public IMeshObject
{
	/**
	* @brief 座標の取得
	*/
	const Vector3& GetPos() const;

	/**
	* @brief ダメージフラグを取得する
	*/
	bool IsDamage() const;

	/**
	* @brief ダメージフラグを設定する
	* @param bFlag - セットするか否かのフラグ
	* @param playerID - プレイヤーの番号
	*/
	void SetDamage(bool bFlag, unsigned int playerID);

	/**
	* @brief 地面に付いているかのフラグを設定する
	*/
	void SetGround(bool bFlag);

	/**
	* @brief 座標を設定する
	*/
	void SetPos(const Vector3& pos);

	/**
	* @brief 速度を設定する
	*/
	void SetSpeed(const Vector3& speed);

	/**
	* @brief 最後に触れたプレイヤー番号を取得する
	*/
	const unsigned int GetID() const;
};


/**
* @interface IStage
* @brief ステージインターフェイス
*/
__interface IStage : public IMeshObject
{
	/**
	* @brief 座標の取得
	*/
	const Vector3& GetPos() const;
};


/**
* @interface ISkyDome
* @brief スカイドームインターフェイス
*/
__interface ISkyDome : public IMeshObject
{
	/**
	* @brief アンビエントを取得する
	*/
	const Vector4& GetAmbient();
};


/**
* @interface I2DObject
* @brief 2Dオブジェクトインターフェイス
*
* @note：全ての2Dゲームオブジェクトはこのインターフェイスを継承して使う
*/
__interface I2DObject
{
	/**
	* @brief 更新
	* @return true - 更新 ： false - オブジェクト削除
	*/
	bool Update();

	/**
	* @brief 描画(シェーダーを使用しない)
	*/
	void Render();
};

/// 2Dオブジェクトのスマートポインタ
using I2DObjectPtr = shared_ptr<I2DObject>;

/**
* @interface IEffect
* @brief エフェクトインターフェイス
*/
__interface IEffect : public I2DObject
{

};


/**
* @interface IParticle
* @brief パーティクルインターフェイス
*/
__interface IParticle : public I2DObject
{

};


/// アイテムのスマートポインタ
using IItemPtr = shared_ptr<IItem>;

/// ステージのスマートポインタ
using IStagePtr = shared_ptr<IStage>;

/// スカイドームのスマートポインタ
using ISkyDomePtr = shared_ptr<ISkyDome>;

#endif