/**
* @file   AnimMesh.h
* @brief  アニメーションメッシュ
* @author Syota Tachikawa
*/

#ifndef ___ANIMMESH_H
#define ___ANIMMESH_H

#include "Mesh.h"
#include "AnimHierarchy.h"

/// アニメーション速度
#define ANIM_SPEED	1.0f/60

/**
* @brief スキンメッシュクラス
*/
class SkinMesh : public Mesh, public Hierachy
{
public:
	/**
	* @brief コンストラクタ
	*/
	SkinMesh();

	/**
	* @brief デストラクタ
	*/
	~SkinMesh();

	/**
	* @brief アニメーションメッシュの読み込み
	* @param path - パス
	* @param xFileName - Ｘファイルの名前
	*/
	bool LoadAnimeXMesh(string path, LPSTR xFileName);

	/**
	* @brief アニメーションメッシュの描画
	*/
	void RenderAnimeXMesh();

	/**
	* @brief アニメーションメッシュの描画(シェーダー)
	*/
	void RenderAnimeXMesh(const Matrix& view, const Matrix& proj);

	/**
	* @brief 影を描画する
	*/
	void DrawShadow(LPD3DXFRAME pFrameBase);

	/**
	* @brief アニメーションフレームの更新
	*/
	void UpdateFrame();

	/**
	* @brief アニメーションフレームの更新
	*/
	void UpdateFrame(float speed);

	/**
	* @brief アニメーションを最後まで再生済みか
	*/
	bool IsAnimPeriod();

	/**
	* @brief アニメーションコントローラーの取得
	*/
	LPD3DXANIMATIONCONTROLLER GetController();

	/**
	* @brief アニメーションタイムを取得
	*/
	float GetTime();

	/**
	* @brief アニメーション番号を取得
	*/
	int GetNowAnimNo();

	/**
	* @brief アルファ値の取得
	*/
	float GetAlpha();

	/**
	* @brief 名前でボーンパーツを検索
	* @param name アニメーション名
	*/
	bool GetPartsMatrixByName(Matrix *pOut, LPCSTR name);

	/**
	* @brief アルファ値を設定
	*/
	void SetAlpha(float alpha);

	/**
	* @brief アニメーション切り替え
	* @param dwSetNo アニメーション番号
	* @param bLoop アニメループフラグ
	*/
	void SetAnimNo(DWORD dwAnimNo, bool bLoop);

	/**
	* @brief アニメーション名前検索
	* @param dwSetNo アニメーション番号
	* @param bLoop アニメループフラグ
	*/
	void SetAnimName(LPCSTR szName, bool bLoop);

	/**
	* @brief アニメーションのシフト時間を設定
	*/
	void SetAnimShiftTime(float fTime);

private:

	static const int MaxBoneMatrix = 12;

	/// デバイス
	LPDIRECT3DDEVICE9 m_pDevice;
	
	/// アニメーションコントローラー
	LPD3DXANIMATIONCONTROLLER m_pAnimController;
	
	/// アニメーションセッター
	LPD3DXANIMATIONSET* m_pAnimSet;

	/// 現在のアニメーション番号
	int m_nNowAnimeNo;
	
	/// アニメーションセット
	DWORD m_uNumAnimSet;

	/// アルファ値
	float m_alpha;

	float m_animCurWeight;

	/// アニメーションのシフト時間
	float m_totalShiftTime;

	/// アニメーション終了時間
	float m_animPeriod;

	/// 切り替えフラグ
	bool m_bChanging;

	/**
	* @brief 初期化
	*/
	void Initialize();
	
	/**
	* @brief アニメーション解放
	*/
	void FreeAnim(LPD3DXFRAME pFrame);

	/**
	* @brief 描画フレーム
	*/
	void DrawFrame(LPD3DXFRAME pFrameBase);

	/**
	* @brief 描画フレーム(シェーダー)
	*/
	void DrawFrame(const Matrix& view, const  Matrix& proj, LPD3DXFRAME pFrameBase);

	/**
	* @brief フレーム更新
	*/
	void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);
	
	/**
	* @brief レンダリングコンテナ
	*/
	void RenderMeshContainer(const MeshContainer *pMeshContainer, const Frame *pFrame);

	/**
	* @brief レンダリングコンテナ(シェーダー)
	*/
	void RenderMeshContainer(const Matrix& view, const Matrix& proj, const MeshContainer *pMeshContainer, const Frame *pFrame);

	/**
	* @brief 名前でパーツ検索再帰処理用
	* @param name アニメーション名
	*/
	bool _GetPartsMatrixByName(Matrix *pOut, LPD3DXFRAME pFrameBase, LPCSTR szName);
	
	/**
	* @brief ボーン行列の初期化
	*/
	HRESULT AllocateBoneMatrix(LPD3DXMESHCONTAINER pMeshContainerBase);

	/**
	* @brief 全てのボーン行列
	*/
	HRESULT AllocateAllBoneMatrices(LPD3DXFRAME pFrame);
};

/// スキンメッシュのスマートポインタ
using SkinMeshPtr = shared_ptr<SkinMesh>;

#endif