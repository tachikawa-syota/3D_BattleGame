#include "Timer.h"

/**
* @brief �R���X�g���N�^
*/
Timer::Timer()
{
	// �p�����[�^������
	m_time = 99;
	m_timeTensPlace = 9;
	m_timeOnesPlace = 9;
}

/**
* @brief �f�X�g���N�^
*/
Timer::~Timer()
{

}

/**
* @brief �X�V
*/
void Timer::Update()
{
	static short frame = 0;

	// �O�������珈�������Ȃ�
	if (m_timeTensPlace == 0 && m_timeOnesPlace == 0) return;

	// 60�t���[����1��
	if (frame > 60)
	{
		// 1�̈ʂ�"0"�̂Ƃ�
		if (m_timeOnesPlace == 0){
			// 10�̈ʂ����炵��
			m_timeTensPlace--;
			// 1�̈ʂ�"9"�ɂ���
			m_timeOnesPlace = 9;
		}

		// ����ȊO��
		else{
			// 1�̈ʂ����炷
			m_timeOnesPlace--;
		}

		// "0"�ɖ߂�
		frame = 0;
		// �^�C�������Z����
		m_time--;
	}

	// �t���[�����Z
	frame++;
}

/**
* @brief �^�C���̎擾
*/
DWORD Timer::GetTime() const
{
	return m_time;
}

/**
* @brief �^�C����"10"�̈ʎ擾
*/
int Timer::GetTenId() const
{
	return m_timeTensPlace;
}

/**
* @brief �^�C����"1"�̈ʎ擾
*/
int Timer::GetOneId() const
{
	return m_timeOnesPlace;
}