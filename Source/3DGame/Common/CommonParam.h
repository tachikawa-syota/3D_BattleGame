/**
* @file   CommonParam.h
* @brief  ���ʂ̃p�����[�^���Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___COMMONPARAM_H
#define ___COMMONPARAM_H

#include "..\iniFileLoader\iniFileLoaderBase.h"

// �C���X�^���X�擾
#define GetComPrm CommonParamLoader::GetInstance()

/**
* @brief ini�t�@�C���ŊǗ����Ă���L�����N�^�[�p�����[�^�[��ǂݍ��ރN���X
*/
class CommonParamLoader : public iniFileLoaderBase
{
public:
	/**
	* @brief �C���X�^���X�̎擾
	*/
	static CommonParamLoader* GetInstance(){
		static CommonParamLoader singleton;
		return &singleton;
	}

	// �������W
	float InitPosY;
	float InitPosZ;
	// �v���C���[�P�̏������W
	float InitPlayer1PosX;
	// �v���C���[�Q�̏������W
	float InitPlayer2PosX;
	// �v���C���[�R�̏������W
	float InitPlayer3PosX;
	// �v���C���[�S�̏������W
	float InitPlayer4PosX;
	// �d��
	float Gravity;
	// �W�����v�̍ő吔
	int JumpMax;
	// ���A���W
	float Pattern1ReturnPosX;
	float Pattern2ReturnPosX;
	float Pattern3ReturnPosX;
	float Pattern4ReturnPosX;
	float ReturnPosY;
	/// UI�`��ʒu
	float DamageRatePosX1;
	float DamageRatePosX2;
	float DamageRatePosX3;
	float DamageRatePosX4;
	/// �v���C���[�e�L�X�g�`��ʒu
	float PlayerTextPosX;

private:
	/**
	* @brief �R���X�g���N�^
	*/
	CommonParamLoader() :iniFileLoaderBase("Param.ini")
	{
		InitPosY = GetNextParameterFloat();
		InitPosZ = GetNextParameterFloat();
		InitPlayer1PosX = GetNextParameterFloat();
		InitPlayer2PosX = GetNextParameterFloat();
		InitPlayer3PosX = GetNextParameterFloat();
		InitPlayer4PosX = GetNextParameterFloat();
		Gravity = GetNextParameterFloat();
		JumpMax = GetNextParameterInt();
		Pattern1ReturnPosX = GetNextParameterFloat();
		Pattern2ReturnPosX = GetNextParameterFloat();
		Pattern3ReturnPosX = GetNextParameterFloat();
		Pattern4ReturnPosX = GetNextParameterFloat();
		ReturnPosY = GetNextParameterFloat();
		DamageRatePosX1 = GetNextParameterFloat();
		DamageRatePosX2 = GetNextParameterFloat();
		DamageRatePosX3 = GetNextParameterFloat();
		DamageRatePosX4 = GetNextParameterFloat();
		PlayerTextPosX = GetNextParameterFloat();
	}
};

#endif