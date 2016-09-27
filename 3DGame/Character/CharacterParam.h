/**
* @file   CharacterParam.h
* @brief  �L�����N�^�[�̃p�����[�^���Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___CHARACTERPARAM_H
#define ___CHARACTERPARAM_H

#include "..\iniFileLoader\iniFileLoaderBase.h"

// �C���X�^���X�擾
#define GetChrPrm CharacterParamLoader::GetInstance()

/**
* @brief ini�t�@�C���ŊǗ����Ă���L�����N�^�[�p�����[�^�[��ǂݍ��ރN���X
*/
class CharacterParamLoader : public iniFileLoaderBase
{
public:
	/**
	* @brief �C���X�^���X�̎擾
	*/
	static CharacterParamLoader* GetInstance(){
		static CharacterParamLoader singleton;
		return &singleton;
	}

// �L�����N�^�["���b�N" ------------------------------
	// ���b�N�̑傫��
	float RockScale;
	// ���b�N�̍U����
	int RockNormalAttackPower;
	// ���b�N�̃X�}�b�V���U����
	int RockSmashAttackPower;
	// ���b�N�̏�K�E�Z�̍U����
	int RockSpecialAttackUpPower;

private:
	/**
	* @brief �R���X�g���N�^
	*/
	CharacterParamLoader() :iniFileLoaderBase("CharacterParam.ini")
	{
		RockScale = GetNextParameterFloat();
		RockNormalAttackPower = GetNextParameterInt();
		RockSmashAttackPower = GetNextParameterInt();
		RockSpecialAttackUpPower = GetNextParameterInt();
	}
};

#endif