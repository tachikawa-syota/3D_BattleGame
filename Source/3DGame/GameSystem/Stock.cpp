#include "Stock.h"

/**
* @brief コンストラクタ
*/
Stock::Stock(int size)
{
	// ストック数
	m_stockNum.resize(size);

	for (int i = 0; i != m_stockNum.size(); i++){
		m_stockNum[i] = 3;
	}
}

/**
* @brief デストラクタ
*/
Stock::~Stock()
{

}

/**
* @brief ストックを減らす
* @param プレイヤーの番号
*/
void Stock::MinusStock(int index)
{
	m_stockNum[index]--;
}

/**
* @brief ストックを取得する
* @param プレイヤーの番号
*/
int Stock::GetStock(int index) const
{
	return m_stockNum[index];
}