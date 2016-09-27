/**
* @file   ScreenEffect.h
* @brief  �X�N���[���G�t�F�N�g�������Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___SCREANEFFECT_H
#define ___SCREANEFFECT_H

#include "..\DirectX\TextureManager.h"

/**
* @enum Fade
* @brief �t�F�[�h
*/
enum class Fade
{
	/// �t�F�[�h���������Ă��Ȃ�
	None,

	/// �t�F�[�h�C��
	In,

	/// �t�F�[�h�A�E�g
	Out
};

/**
* @enum FadeColor
* @brief �t�F�[�h�F
*/
enum class FadeColor
{
	/// ��
	Black,

	/// ��
	White
};

/**
* @brief �X�N���[���G�t�F�N�g�N���X
*/
class ScreenEffect
{
public:
	// �C���X�^���X�擾
	static ScreenEffect* GetInstance(){
		static ScreenEffect singleton;
		return &singleton;
	}

	/**
	* @brief ������
	*/
	void Initialize();

	/**
	* @brief �t�F�[�h�̐ݒ�
	*/
	bool SetFade(Fade type, FadeColor color, int frame);

	/**
	* @brief �t�F�[�h���s�������`�F�b�N
	*/
	bool isPlayFade();

	/**
	* @brief �X�V
	*/
	void Update();

	/**
	* @brief �`��
	*/
	void Draw();

private:
	/// ���Z�l
	int m_add;

	/// �F
	FadeColor m_color;

	/// �t���[��
	int m_frame;

	/// �J�E���g
	int m_fade_cnt;

	/// �t�F�[�h���
	Fade m_state;

};

#endif