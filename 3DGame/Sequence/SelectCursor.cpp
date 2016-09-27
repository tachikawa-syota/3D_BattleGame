#include "SelectCursor.h"
#include "Title.h"
#include "Option.h"

/**
* @brief �R���X�g���N�^
*/
SelectCursor::SelectCursor(const type_info& type)
{
	// �C���X�^���X���擾
	m_2dTex = TextureManager::GetInstance();

	// �f�o�C�X�̍쐬
	m_dev = make_unique<InputMask>(0);

	// �^�C�v�ɂ���ăp�����[�^��ݒ�
	if (type == typeid(Title)){
		m_cursorPos = Vector2(750.0f, TitlePosY);
		m_cursorAdd = TitleCursorAdd;
		m_selectMax = 4;
	}
	else if (type == typeid(Option)){
		m_cursorPos = Vector2(100.0f, 200.0f);
		m_cursorAdd = 200.0f;
		m_selectMax = 3;
	}

	// �����̃J�[�\���̔ԍ���"1"
	m_cursorNumber = 1;
	m_oldCursorNumber = 1;
}

/**
* @brief �f�X�g���N�^
*/
SelectCursor::~SelectCursor()
{

}

/**
* @brief �X�V
*/
bool SelectCursor::Update()
{
	// ����t���O��������
	for (int i = 0; i < 2; i++){
		m_selectFlag[i] = false;
	}

	// �f�o�C�X������͂��擾����
	m_dev->GetInput();

	// ���ړ�
	if (m_dev->CheckInputMask(INPUT_DOWN)){
		m_cursorNumber++;
	}
	// ��ړ�
	if (m_dev->CheckInputMask(INPUT_UP)){
		m_cursorNumber--;
	}

	// �E����
	if (m_dev->CheckInputMask(INPUT_RIGHT)){
		m_selectFlag[0] = true;
	}
	// ������
	if (m_dev->CheckInputMask(INPUT_LEFT)){
		m_selectFlag[1] = true;
	}

	// �J�[�\�������Ɉړ�������
	if (m_cursorNumber > m_oldCursorNumber){
		m_cursorPos.y += m_cursorAdd;
	}
	// �J�[�\������Ɉړ�������
	else if (m_cursorNumber < m_oldCursorNumber){
		m_cursorPos.y -= m_cursorAdd;
	}

	// ��[���z������
	if (m_cursorNumber < 1){
		// ���[�Ɉړ�
		m_cursorPos.y += (m_cursorAdd * m_selectMax);
		m_cursorNumber = m_selectMax;
	}
	// ���[���z������
	if (m_cursorNumber > m_selectMax){
		// ��[�Ɉړ�
		m_cursorPos.y -= (m_cursorAdd * m_selectMax);
		m_cursorNumber = 1;
	}

	// ���o�^
	m_oldCursorNumber = m_cursorNumber;

	// ����L�[
	if (m_dev->CheckInputMask(INPUT_01))
	{
		// ���ʉ����Đ�
		SoundPlayer::GetInstance()->Play(DECISION_SE);
		return true;
	}

	return false;
}

/**
* @brief �`��
*/
void SelectCursor::Render(int alpha)
{
	m_2dTex->DrawTexture(CURSOL_PNG, m_cursorPos.x, m_cursorPos.y, 0.0f, 1.0f, alpha);
}

/**
* @brief �J�[�\�����W���擾����
*/
const Vector2& SelectCursor::GetPos() const
{
	return m_cursorPos;
}

/**
* @brief �J�[�\���ԍ��̎擾
*/
int SelectCursor::GetNumber() const
{
	return m_cursorNumber;
}

/**
* @brief �E�̓��͂����(Option�ł����g��Ȃ�)
*/
int SelectCursor::GetSelectRight() const
{
	return m_selectFlag[0];
}

/**
* @brief ���̓��͂����(Option�ł����g��Ȃ�)
*/
int SelectCursor::GetSelectLeft() const
{
	return m_selectFlag[1];
}

/**
* @brief ���̓f�o�C�X���擾����
*/
const InputPtr& SelectCursor::GetInputDevice()
{
	return m_dev;
}