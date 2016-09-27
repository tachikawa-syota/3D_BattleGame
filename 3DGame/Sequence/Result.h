/**
* @file   Result.h
* @brief  ���U���g��ʂ��Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___RESULT_H
#define ___RESULT_H

#include "ISequence.h"
#include "SelectCursor.h"
#include "..\Common\UniqueData.h"

/**
* @brief ���U���g��ʂ��Ǘ�����N���X
*/
class Result : public ISequence
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

	/// ���̓f�o�C�X
	InputPtr m_dev;

	/// ����
	vector<int> m_rank;

	/// ���U���g�f�[�^
	vector<RESULT_DATA> m_data;

	/// Winner�`��ʒu
	Vector2 m_winnerPos;

	/// ���̊p�x
	float m_lightAngle;

	/// �����e�N�X�`��
	int m_numberTex[NUMBER_TEXTURE_MAX];

	/// �v���C���[�e�L�X�g�e�N�X�`��
	int m_playerTextTex[PLAYER_TEXTURE_MAX];

	/**
	* @brief ���U���g�̍X�V
	*/
	bool UpdateResultWork();
};

#endif