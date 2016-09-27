#include "Bomb.h"
#include "..\..\Common\CommonParam.h"
#include "..\..\DirectX\Direct3DData.h"
#include "..\..\Common\FileData.h"

/**
* @brief コンストラクタ
*/
Bomb::Bomb(const Vector3& pos)
{
	// モデルのロード(クローン化未実装のため毎回ロードする・・・)
	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->LoadNonAnimMesh("Data/Model/Bomb/", "inseki.x");
	
	// コピー
	m_obj = mesh;
	m_pos = pos;

	// 座標の設定
	m_obj->SetPos(m_pos);
	// 大きさの設定
	m_obj->SetScale(0.6f);

	// 速度の設定
	m_spd = Vector3(0.0f, GetComPrm->Gravity, 0.0f);

	m_gravity = 0.0f;

	m_isExplosion = false;
	m_isGround = false;
	m_id = -1;
}

/**
* @brief デストラクタ
*/
Bomb::~Bomb()
{

}

/**
* @brief 更新
*/
bool Bomb::Update()
{
	// 接地していたら減速処理をする(重力を消す)
	if (m_isGround){
		m_gravity = 0.0f;
		m_spd.y = m_gravity;
		m_spd *= 0.7f;
	}
	// 重力をかける
	else{
		m_gravity = GetComPrm->Gravity;
	}

	// 座標に速度を加算する
	m_spd.y += m_gravity;
	m_pos += m_spd;

	// 更新
	m_obj->SetPos(m_pos);
	m_obj->Update();

	// 更新
	return true;
}

/**
* @brief 描画(シェーダーを使用する)
*/
void Bomb::Render(const Matrix& view, const Matrix& proj)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = D3DIns->GetDevice();
	// Zバッファ
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_obj->RenderXMesh(view, proj);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
}

/**
* @brief ワールド行列を取得する
*/
const Matrix& Bomb::GetWorld()
{
	return m_obj->GetTransMatrix();
}

/**
* @brief 座標の取得
*/
const Vector3& Bomb::GetPos() const
{
	return m_pos;
}

/**
* @brief ダメージフラグ
*/
bool Bomb::IsDamage() const
{
	return m_isExplosion;
}

/**
* @brief 地面に付いているかのフラグを設定する
*/
void Bomb::SetGround(bool bFlag)
{
	m_isGround = bFlag;
}

/**
* @brief ダメージフラグを設定する
* @param bFlag - セットするか否かのフラグ
* @param playerID - プレイヤーの番号
*/
void Bomb::SetDamage(bool bFlag, unsigned int playerID)
{
	m_isExplosion = bFlag;
	m_id = playerID;
}

/**
* @brief 座標を設定する
*/
void Bomb::SetPos(const Vector3& pos)
{
	m_pos = pos;
}

/**
* @brief 速度を設定する
*/
void Bomb::SetSpeed(const Vector3& speed)
{
	m_spd = speed;
}

/**
* @brief メッシュを取得する
*/
const MeshPtr& Bomb::GetMesh() const
{
	return m_obj;
}

/**
* @brief 最後に触れたプレイヤー番号を取得する
*/
const unsigned int Bomb::GetID() const
{
	return m_id;
}