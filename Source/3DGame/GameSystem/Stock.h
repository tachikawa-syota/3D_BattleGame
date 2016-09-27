/**
* @file   Stock.h
* @brief  ユーザインターフェイスのストックを管理
* @author Syota Tachikawa
*/

#ifndef ___STOCK_H
#define ___STOCK_H

#include "..\Common\FileData.h"

/**
* @brief ストッククラス
*/
class Stock
{
public:
	/**
	* @brief コンストラクタ
	*/
	Stock(int size);

	/**
	* @brief デストラクタ
	*/
	~Stock();

	/**
	* @brief ストックを減らす
	* @param プレイヤーの番号
	*/
	void MinusStock(int index);

	/**
	* @brief ストックの数を取得する
	* @param プレイヤーの番号
	*/
	int GetStock(int index) const;

private:
	/// ストック数
	vector<int> m_stockNum;
};

using StockPtr = shared_ptr<Stock>;

#endif