/**
* @file   Timer.h
* @brief  �^�C�}�[�������Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___TIMER_H
#define ___TIMER_H

#include "..\Common\FileData.h"

/**
* @brief �o�g�����Ɏg�p����^�C�}�[�N���X
*/
class Timer
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	Timer();

	/**
	* @brief �f�X�g���N�^
	*/
	~Timer();

	/**
	* @brief �X�V
	*/
	void Update();

	/**
	* @brief �^�C����"10"�̈ʎ擾
	*/
	int GetTenId() const;

	/**
	* @brief �^�C����"1"�̈ʎ擾
	*/
	int GetOneId() const;

	/**
	* @brief �^�C���̎擾
	*/
	DWORD GetTime() const;

private:
	/// �^�C�}�[��1�̈�
	int m_timeOnesPlace;

	/// �^�C�}�[��10�̈�
	int m_timeTensPlace;

	/// �^�C��
	DWORD m_time;
};

#endif