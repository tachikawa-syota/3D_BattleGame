/**
* @file   Title.h
* @brief  �^�C�g���̏������Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___TITLE_H
#define ___TITLE_H

#include "ISequence.h"
#include "SelectCursor.h"
#include "..\DirectX\Direct3DSystem.h"
#include "..\DirectX\Direct3DData.h"

/**
* @brief �Q�[�����C���N���X
*/
class Title : public ISequence
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

	/// �J�[�\��
	SelectCursorPtr m_cursor;

	/// �o�g�������̑傫��
	float m_battleFontScale;

	/// �I�v�V���������̑傫��
	float m_optionFontScale;

	/// �N���W�b�g�����̑傫��
	float m_creditTextScale;

	/// �I�������̑傫��
	float m_endTextScale;

	/// �g��t���O
	bool m_isAddScale;

	/// �T�u���[�N
	int m_subWork;

	/// ���x���I�����[�N
	int m_levelWork;

	/// �R���s���[�^�[�ԍ�
	int m_index;

	/// �R���s���[�^�[�̃��x��
	vector<int> m_computerLevel;

	/// �v���C���[����
	int m_playerText[PLAYER_TEXTURE_MAX];

	/// �����e�N�X�`��
	int m_numTex[NUMBER_TEXTURE_MAX];

	/// �A���t�@�l
	int m_alpha;

	/// BGM��~�t���O
	bool m_isEndBGM;

	/**
	* @brief �^�C�g���̍X�V
	*/
	void UpdateTitleWork();

	/**
	* @brief �R���s���[�^�[�̃��x���I���̍X�V
	*/
	bool UpdateComputerLevelSelect();
};

#endif