#include "..\ApplicationSystem\FrameWork.h"

/**
* @brief �A�v���P�[�V�����̏����G���g���|�C���g
*/
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{ 
	// �C���X�^���X�𐶐�
	auto mainFrame = make_unique<FrameWork>();

	// �t���[�����[�N�̍쐬�Ɏ��s������"0"���Ԃ�
	if (!mainFrame->Create(hInst)) return 0;

	// �X�V�ƕ`��
	mainFrame->Run();

	return 0;
}
