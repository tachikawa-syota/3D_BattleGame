/**
* @file   GUIManager.h
* @brief  �O���t�B�J�����[�U�C���^�t�F�[�X�Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___GUIMANAGER_H
#define ___GUIMANAGER_H

#include "..\..\Common\FileData.h"
#include "..\..\Common\STDXDefine.h"
#include "..\..\Common\STDXStruct.h"
#include "..\..\GameSystem\DamageData.h"
#include "..\..\Character\IPlayer.h"

// �O���錾
class Timer;
class Stock;
class Camera;

/**
* @brief �O���t�B�J���C���^�[�t�F�C�X�N���X
*/
class GUI
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	GUI(int playerCount);

	/**
	* @brief �f�X�g���N�^
	*/
	~GUI();

	/**
	* @brief �`��
	*/
	void Render(shared_ptr<Timer>& time, shared_ptr<Stock>& stock, vector<IPlayerPtr> player, CameraPtr camera, DamageData damage);

private:
	/// �O���t�B�b�N�X�}�l�[�W��
	TextureManager* m_2dTex;

	/// �t�@�C���f�[�^
	FileData* m_fileData;

	/// �����e�N�X�`��
	int m_numberTex[NUMBER_TEXTURE_MAX];

	/// �v���C���[�e�L�X�g�e�N�X�`��
	int m_playerTextTex[PLAYER_TEXTURE_MAX];
	
	/// �v���C���[�̐�
	int m_playerCount;

	// �`��ʒu
	vector<Vector2> m_playerTextPos;
	vector<Vector2> m_stockPos;
	vector<Vector2> m_damagePos;

	/// �t�@�C�g�e�N�X�`���A�j��
	int m_fightAnimation;

	/// �t���[��
	DWORD m_frame;
};

#endif