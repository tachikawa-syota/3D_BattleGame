#include "Stock.h"

/**
* @brief �R���X�g���N�^
*/
Stock::Stock(int size)
{
	// �X�g�b�N��
	m_stockNum.resize(size);

	for (int i = 0; i != m_stockNum.size(); i++){
		m_stockNum[i] = 3;
	}
}

/**
* @brief �f�X�g���N�^
*/
Stock::~Stock()
{

}

/**
* @brief �X�g�b�N�����炷
* @param �v���C���[�̔ԍ�
*/
void Stock::MinusStock(int index)
{
	m_stockNum[index]--;
}

/**
* @brief �X�g�b�N���擾����
* @param �v���C���[�̔ԍ�
*/
int Stock::GetStock(int index) const
{
	return m_stockNum[index];
}