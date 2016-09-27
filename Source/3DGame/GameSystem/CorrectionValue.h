/**
* @file   CorrectionValue.h
* @brief  �␳�l���Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___CORRECTIONVALUE_H
#define ___CORRECTIONVALUE_H

#include "..\Character\Rock.h"

/**
* @brief ������΂����Ɏg�p����␳�l���Ǘ�����
*/
class CorrectionValue
{
public:
	/**
	* @brief �C���X�^���X���擾����i�V���O���g���j
	*/
	static CorrectionValue* GetInstance(){
		static CorrectionValue singleton;
		return &singleton;
	}

	/**
	* @brief ������
	*/
	void Initialize();

	/**
	* @brief �␳�l���擾����
	*/
	float GetValue(const type_info& type, vector<IPlayerPtr>& player, int index) const;

private:
	/// �L�����N�^�["���b�N"�̕␳�l
	float m_rockCorValue[3];
};

#endif