/**
* @file   Utility.h
* @brief  有用性のある処理を管理する
* @author Syota Tachikawa
*/

#ifndef ___UTILITY_H
#define ___UTILITY_H

#include "..\GameSystem\CorrectionValue.h"
#include "..\GameSystem\DamageData.h"

// 警告抑制
#pragma warning(disable:4172)

using std::mt19937;
using std::random_device;
using std::uniform_int_distribution;
using std::uniform_real_distribution;

/**
* @brief ユーティリティクラス
*/
class Utility
{
public:
	/**
	* @brief 整数型乱数を返す
	* @param min - 最小値
	* @param max - 最大値
	*
	* @note：メルセンヌ・ツイスタ
	*/
	static int Random(int min, int max)
	{
		// メルセンヌ・ツイスタ
		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> rad(min, max);
		int result = rad(mt);

		return result;
	}

	/**
	* @brief 浮動小数点乱数を返す
	* @param min - 最小値
	* @param max - 最大値
	*
	* @note：メルセンヌ・ツイスタ
	*/
	static float Random(float min, float max)
	{
		random_device rd;
		mt19937 mt(rd());
		uniform_real_distribution<float> rad(min, max);
		float result = rad(mt);

		return result;
	}

	/**
	* @brief 吹き飛ぶ速度を計算する
	* @param obj1 - 攻撃者のプレイヤー番号
	* @param obj2 - 被弾者のプレイヤー番号
	* @param rate - ダメージ率
	* @return 計算された速度
	*/
	static Vector3 BlownOffSpeed(const type_info& type, vector<IPlayerPtr>& player, int obj1, int obj2, DWORD rate)
	{
		// 初期化しておく
		Vector3 result = Vector3();
		// 差分取得
		result = Utility::DiffVector(player[obj1]->GetPos(), player[obj2]->GetPos());
		
		// 正規化
		result.Normalize();

		// 被弾者が地面と接していたら上向きに飛ばす
		if (player[obj2]->IsGround()){
			result.y += 0.5f;
			// 正規化
			result.Normalize();
		}

		// キャラクターと攻撃に応じて補正値を取得する
		float value = CorrectionValue::GetInstance()->GetValue(type, player, obj1);

		// ダメージ率と補正値を乗算する
		result = result * value * static_cast<float>(rate);

		// 結果
		return result;
	}

	/**
	* @brief 吹き飛ぶ速度を計算する(プレイヤーとアイテム)
	* @param player - プレイヤークラス
	* @param index - プレイヤー番号
	* @param item - アイテムのイテレータ
	* @param value - 吹き飛び補正値
	* @return 計算された速度
	*/
	static Vector3 BlownOffSpeed(vector<IPlayerPtr>& player, int index, Vector3& itemPos, float value)
	{
		// 初期化しておく
		Vector3 result = Vector3();
		// 差分取得
		result = Utility::DiffVector(player[index]->GetPos(), itemPos);
		
		// 正規化
		result.Normalize();

		// y値の補正をかける
		result.y += 0.4f;

		// 正規化
		result.Normalize();

		float speed = static_cast<float>(player[index]->GetPower());
		if (speed <= 0.0f){
			speed -= value;
		}
	
		// 速度の補正
		result = result * 0.04f * speed;

		// 結果
		return result;
	}

	/**
	* @brief ベクトルの差分を取得する
	*/
	static Vector3&& DiffVector(const Vector3& vector1, const Vector3& vector2)
	{
		Vector3 vec1 = vector1;
		Vector3 vec2 = vector2;

		Vector3 vec3 = vec2 - vec1;

		return Vector3(vec3.x, vec3.y, vec3.z);
	}

	/**
	* @brief 一番ダメージを喰らっているプレイヤーを取得
	* @param player - プレイヤーインターフェイスのスマートポインタ
	* @param damage - ダメージデータクラス
	* @param index - 自身の番号
	* @return 一番ダメージを喰らっているプレイヤー
	*/
	static IPlayerPtr GetMostDamagePlayer(vector<IPlayerPtr>& player, DamageData damage, int index)
	{
		DWORD minDamage = 0;
		IPlayerPtr result = nullptr;

		for (int i = 0; i != player.size(); i++)
		{
			// 自分は除く
			if (i == index){
				continue;
			}

			// ダメージ率を取得する
			DWORD temp = damage.GetDamageRate(i);
			
			// 比較する
			if (temp > minDamage){
				minDamage = temp;
				result = player[i];
			}
		}

		return result;
	}

	/**
	* @brief 自分と一番近いプレイヤーを取得
	* @param player - プレイヤーインターフェイスのスマートポインタ
	* @param index - 自身の番号
	* @return 自分と一番近いプレイヤー
	*/
	static IPlayerPtr GetNearPlayer(vector<IPlayerPtr>& player, int index)
	{
		float minDistance = 999.0f;

		IPlayerPtr result = nullptr;

		for (int i = 0; i != player.size(); i++)
		{
			// 自分は除く
			if (i == index){
				continue;
			}
			// リタイアしているプレイヤーも除く
			if (player[i]->IsRetire()){
				continue;
			}

			// 自分の位置
			Vector3 plPos = player[index]->GetPos();

			// 相手の位置
			Vector3 emPos = player[i]->GetPos();

			// 距離
			Vector3 diff = DiffVector(plPos, emPos);

			// 距離
			float distance = diff.Length();

			if (distance < minDistance){
				minDistance = distance;
				result = player[i];
			}
		}

		return result;
	}

	/**
	* @brief 一番離れているプレイヤー同士の距離を測る
	* @param プレイヤーのスマートポインタ
	*/
	static float GetFarPlayerDistance(const vector<IPlayerPtr>& player)
	{
		// 存在するプレイヤー分の配列を確保
		vector<float> posX;
		posX.resize(player.size());

		// プレイヤーのX座標をコピーする
		for (int i = 0; i != player.size(); i++){
			posX[i] = player[i]->GetPos().x;
			
			// リタイアしてるプレイヤーは計算に含めない
			if (player[i]->IsRetire()){
				posX[i] = 0.0f;
			}
		}

		// X座標の最小値
		float min = *min_element(posX.begin(), posX.end());
		// X座標の最大値
		float max = *max_element(posX.begin(), posX.end());
		
		Vector3 minVec, maxVec;
		// X座標が最小と最大のプレイヤーのベクトルを取得する
		for (int i = 0; i != player.size(); i++)
		{
			if (player[i]->GetPos().x == min){
				minVec = player[i]->GetPos();
			}
			if (player[i]->GetPos().x == max){
				maxVec = player[i]->GetPos();
			}
		}

		// 差分
		Vector3 diff = Utility::DiffVector(maxVec, minVec);

		// 距離
		float len = diff.Length();

		// 距離を返す
		return len;
	}

	/**
	* @brief ダイアログボックスを表示しながらアプリケーションを終了する
	*/
	static void abortWithMsg(char* msg)
	{
		MessageBox(NULL, msg, "意図しない終了", MB_OK | MB_ICONERROR);
		exit(0);
	}
};

#endif