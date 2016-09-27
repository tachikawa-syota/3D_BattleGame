/**
* @file   Option.h
* @brief  �Q�[���̃��[����ݒ肷��
* @author Syota Tachikawa
*/

#ifndef ___OPTION_H
#define ___OPTION_H

#include "ISequence.h"
#include "SelectCursor.h"
#include "..\DirectX\Direct3DSystem.h"
#include "..\DirectX\Direct3DData.h"
#include "..\Common\UniqueData.h"

/**
* @enum SettingNumber
* @brief �Z�b�e�B���O�ԍ�
*/
enum SettingNumber
{
	/// �ΐ�l��
	SettingBattleNum = 1,

	/// �o�g�����[�h
	SettingBattleMode,

	/// �X�y�V�����o�g��
	SettingSpecialBattle
};

/**
* @brief �Q�[���̃��[�����Ǘ�����I�v�V�����N���X
*/
class Option : public ISequence
{
public:
	/**
	* @brief �쐬
	*/
	void Create();

	/**
	* @brief ������
	*/
	void Initialize();

	/**
	* @brief �X�V
	*/
	void Update();

	/**
	* @brief �`��
	*/
	void Render();

	/**
	* @brief �I��
	*/
	void Finalize();

private:
	/// �O���t�B�b�N�X�}�l�[�W��
	TextureManager* m_2dTex;

	/// �t�@�C���f�[�^
	FileData* m_fileData;

	/// �J�[�\��
	SelectCursorPtr m_cursor;

	/// �o�g�����[�h
	BattleMode m_battleMode;

	/// �����e�N�X�`��
	int m_numTex[NUMBER_TEXTURE_MAX];

	/// �ΐ�l��
	int m_battleNum;
	
	/// �X�y�V�����o�g���t���O
	bool m_isSpecialBattle;

	/// �\���t���O
	bool m_isLeft;
	bool m_isRight;

	/// �J�[�\����Y���W
	float m_cursorY;

	/**
	* @brief ���A�C�R���̕\���t���O���X�V
	*/
	void UpdateDisplayArrowIcon();
};

#endif