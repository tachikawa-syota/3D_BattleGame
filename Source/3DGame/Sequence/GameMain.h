/**
* @file   GameMain.h
* @brief  �Q�[�����̏������Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___GAMEMAIN_H
#define ___GAMEMAIN_H

#include "ISequence.h"
// DirectX
#include "..\DirectX\Direct3DSystem.h"
#include "..\DirectX\Direct3DData.h"
#include "..\DirectX\InputMask.h"
#include "..\DirectX\Font.h"
// Object
#include "..\Object\ObjectManager.h"
#include "..\Object\Stage\Tree.h"
#include "..\Object\GUI\GUIManager.h"
// System
#include "..\GameSystem\ScoreManager.h"
#include "..\GameSystem\Computer.h"
// Shader
#include "..\Shader\WaveShader.h"

/**
* @enum GameMainState
* @brief �Q�[�����C���̏��
*/
enum class GameMainState : BYTE
{
	// �X�^�[�g
	Start = 0,

	// �o�g����
	Play,
	
	// �I��
	Finish
};

/**
* @brief �Q�[�����C���N���X
*/
class GameMain : public ISequence
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

#pragma region �����o�ϐ�
	/// �I�u�W�F�N�g�}�l�[�W���[
	ObjectManager* m_objMgr;

	/// �g�V�F�[�_�[
	WaveShaderPtr m_waveShader;

	/// �p�����b�N�X�}�b�s���O
	ShaderPtr m_PM;

	/// �X�J�C�h�[���V�F�[�_�[
	ShaderPtr m_skyDomeShader;

	/// ���C�g�u���[���V�F�[�_�[
	ShaderPtr m_lightBloomShader;

	/// �X�L���e�X�g
	ShaderPtr m_skinTest;

	/// �V�F�[�_�[�}�l�[�W���[
	ShaderManager* m_shaderMgr;

	/// �v���C���[
	vector<IPlayerPtr> m_player;

	/// �R���s���[�^�[
	vector<Computer> m_computer;

	/// ���̓f�o�C�X
	vector<InputPtr> m_dev;

	/// �X�e�[�W
	IStagePtr m_stage;

	/// ��
	TreePtr m_tree;

	/// �X�J�C�h�[��
	ISkyDomePtr m_skyDome;

	/// �J����
	CameraPtr m_camera;

	/// �C���^�[�t�F�C�X
	shared_ptr<GUI> m_gui;

	/// �^�C�}�[
	shared_ptr<Timer> m_timer;

	/// �X�g�b�N
	StockPtr m_stock;

	/// �X�R�A�Ǘ�
	ScoreManagerPtr m_scoreMgr;

	/// �t�H���g
	unique_ptr<Font> m_font;

	/// �Q�[�����C���̏��
	GameMainState m_gameState;

	/// �_���[�W�f�[�^
	DamageData m_damageData;

	/// �f�o�C�X�̏��
	vector<DeviceState> m_deviceState;

	/// �O�t���[���̃J�������W(�|�[�Y��ʂŎg�p����)
	Vector3 m_oldCameraPos;

	/// �Q�[���p�̃t���[��
	UINT m_gameFrame;

	/// �T�u���[�N
	int m_subWork;

	/// �ΐ�l��
	int m_playerNum;

	/// ����
	int m_rank;

	/// StartFinish�e�N�X�`�����R�s�[����ϐ�
	int m_startFinishTex[STARTFINISH_TEXTURE_MAX];

	/// �|�[�Y�t���O
	bool m_isPause;

#pragma endregion


#pragma region �����o�֐�

	/**
	* @brief ���X�g�ɃI�u�W�F�N�g��ǉ�����
	*/
	void AddObject();

	/**
	* @brief �V�F�[�_�[���쐬����
	*/
	void CreateShader();

	/**
	* @brief ����~�炷
	*/
	void UpdateSnowParticle();

	/**
	* @brief �A�C�e���̍X�V
	*/
	void UpdateItem();

	/**
	* @brief �J�����̍X�V
	*/
	void UpdateCamera();

	/**
	* @brief �o�g���J�n��Ԃ̍X�V
	* @return true - ���̏�ԂɈڍs�Ffalse - ���݂̏������p��
	*/
	bool UpdateGameStart();

	/**
	* @brief �o�g����Ԃ̍X�V
	* @return true - ���̏�ԂɈڍs�Ffalse - ���݂̏������p�� 
	*/
	bool UpdateGamePlay();

	/**
	* @brief �o�g���I���̍X�V
	*/
	void UpdateGameFinish();

	/**
	* @brief ���̓f�o�C�X���X�V����
	*/
	void UpdateInputDevice();

	/**
	* @brief �R���s���[�^�[���X�V����
	*/
	void UpdateComputer();

	/**
	* @brief �L�����N�^�[�̍X�V������
	*/
	void UpdateCharacter();

	/**
	* @brief �^�C�����[�h�̍X�V������
	*/
	bool UpdateBattleModeTime();

	/**
	* @brief �X�g�b�N���[�h�̍X�V������
	*/
	bool UpdateBattleModeStock();

	/**
	* @brief ���ׂĂ̓����蔻����`�F�b�N����
	*/
	void AllCollisionCheck();

	/**
	* @brief ���G�t�F�N�g�𐶐����邩�ǂ������m�F����
	*/
	void CreateCheckSmokeEffect();

	/**
	* @brief �f�o�C�X�̓��͂��`�F�b�N����
	* @param flag - ���̓t���O
	* @param index - �`�F�b�N����ԍ�
	*/
	bool CheckDeviceState(unsigned int FLAG, int index);

	/**
	* @brief �|�[�Y�̍X�V
	*/
	bool UpdatePause();

	/**
	* @brief UI�̕`��
	*/
	void RenderUI(LPDIRECT3DTEXTURE9& bumpMap);

#pragma endregion
};

#endif