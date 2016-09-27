#include "FPSCounter.h"

/**
* @brief �R���X�g���N�^
* @param FPS���[�h�̎w��
*/
FPSCounter::FPSCounter(FPSMode fpsMode)
{
	// FPS���[�h�̐ݒ�
	m_FPSMode = fpsMode;
	
	// �Q�[���t���[���̏�����
	m_dwGameFrame = 0;

	// �t���[���^�C���̏�����
	m_dwFrameTime = clock();
}

/**
* @brief �f�X�g���N�^
*/
FPSCounter::~FPSCounter()
{

}

BOOL FPSCounter::Update()
{
	static DWORD dwSec = 0;
	DWORD CurrentTime = clock() * 10;

	// �t���[������
	if (CurrentTime < m_dwFrameTime + 167) {
		return FALSE;
	}

	// �o�ߎ���
	DWORD dTime = CurrentTime - m_dwFrameTime;
	if (dTime > 2000) m_dwFrameTime = CurrentTime;

	// �X�L�b�v�^�C�v 
	switch (m_FPSMode){
	case FPSMode::FPS_60:	bRender = TRUE;	break;
	case FPSMode::FPS_30:	if (m_dwGameFrame & 0x01) bRender = TRUE; else bRender = FALSE;
		break;
	case FPSMode::FPS_FLEX:	if (dTime > 167 * 2) bRender = FALSE; else bRender = TRUE;
		break;
	}

	// �t���[�����ԍX�V
	if (GetKeyState(VK_LCONTROL) < 0) m_dwFrameTime += 300;
	m_dwFrameTime += 167;

	// �b�ԃt���[�����ۑ�
	if (dwSec < CurrentTime){
		m_dwFPS = m_dwCurFrame;
		m_dwRenderFPS = m_dwRCurFrame;
		m_dwCurFrame = m_dwRCurFrame = 0;
		dwSec += 10000;
	}
	// �t���[�����X�V
	m_dwCurFrame++;

	// �Q�[���t���[�����X�V
	m_dwGameFrame++;

	// �`��t���[�����X�V
	if (bRender) m_dwRCurFrame++;

	return TRUE;
}


/**
* @brief �`��t���O�̎擾
*/
BOOL FPSCounter::IsRender()
{
	return bRender;
}

/**
* @brief �t���[�����[�g�̎擾
*/
DWORD FPSCounter::GetFPS()
{
	return m_dwFPS;
}

/**
* @brief �`��t���[�����[�g�̎擾
*/
DWORD FPSCounter::GetRenderFPS()
{
	return m_dwRenderFPS;
}