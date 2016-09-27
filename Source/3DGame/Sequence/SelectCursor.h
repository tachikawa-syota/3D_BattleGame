/**
* @file   SelectCursor.h
* @brief  �I���J�[�\���̏���
* @author Syota Tachikawa
*/

#ifndef ___SELECTCURSOR_H
#define ___SELECTCURSOR_H

#include "..\Common\FileData.h"
#include "..\Common\STDXStruct.h"
#include "..\DirectX\InputMask.h"

/// �^�C�g���ɕ\������e�L�X�g�̍��W(��ԏ�ɕ`�悷�镨��)
const float TitlePosY = 400.0f;
const float TitlePosX = 1000.0f;
const float TitleCursorAdd = 90.0f;

/**
* @brief �V�[�����Ŏg���I���J�[�\���̃N���X
*/
class SelectCursor
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	SelectCursor(const type_info& type);
	
	/**
	* @brief �f�X�g���N�^
	*/
	~SelectCursor();

	/**
	* @brief �X�V
	* @return true - �I���I���Ffalse - �����̌p��
	*/
	bool Update();

	/**
	* @brief �`��
	*/
	void Render(int alpha = 255);

	/**
	* @brief �J�[�\�����W���擾����
	*/
	const Vector2& GetPos() const;

	/**
	* @brief �J�[�\���ԍ��̎擾
	*/
	int GetNumber() const;

	/**
	* @brief �E�̓��͂����(Option�ł����g��Ȃ�)
	*/
	int GetSelectRight() const;

	/**
	* @brief ���̓��͂����(Option�ł����g��Ȃ�)
	*/
	int GetSelectLeft() const;

	/**
	* @brief ���̓f�o�C�X���擾����
	*/
	const InputPtr& GetInputDevice();

private:
	/// �e�N�X�`���}�l�[�W��
	TextureManager* m_2dTex;

	/// ���̓f�o�C�X
	InputPtr m_dev;

	/// �J�[�\�����W
	Vector2 m_cursorPos;

	/// �J�[�\���ԍ�
	int m_cursorNumber;

	/// �O�t���[���̔ԍ�
	int m_oldCursorNumber;

	/// �J�[�\���𓮂�����
	float m_cursorAdd;

	/// �I�����ڂ̍ő吔
	int m_selectMax;

	/// �I���t���O
	bool m_selectFlag[2];
};

/// �J�[�\����
using SelectCursorPtr = unique_ptr<SelectCursor>;

#endif