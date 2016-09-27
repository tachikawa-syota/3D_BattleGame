/**
* @file   Stock.h
* @brief  ���[�U�C���^�[�t�F�C�X�̃X�g�b�N���Ǘ�
* @author Syota Tachikawa
*/

#ifndef ___STOCK_H
#define ___STOCK_H

#include "..\Common\FileData.h"

/**
* @brief �X�g�b�N�N���X
*/
class Stock
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	Stock(int size);

	/**
	* @brief �f�X�g���N�^
	*/
	~Stock();

	/**
	* @brief �X�g�b�N�����炷
	* @param �v���C���[�̔ԍ�
	*/
	void MinusStock(int index);

	/**
	* @brief �X�g�b�N�̐����擾����
	* @param �v���C���[�̔ԍ�
	*/
	int GetStock(int index) const;

private:
	/// �X�g�b�N��
	vector<int> m_stockNum;
};

using StockPtr = shared_ptr<Stock>;

#endif