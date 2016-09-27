/**
* @file   IObject.h
* @brief  �I�u�W�F�N�g�̃C���^�[�t�F�C�X
* @author Syota Tachikawa
*/

#ifndef ___IOBJECT_H
#define ___IOBJECT_H

#include "..\DirectX\AnimMesh.h"

/**
* @interface IObject
* @brief 3D�I�u�W�F�N�g�C���^�[�t�F�C�X
*
* @note�F�S�Ă�3D�Q�[���I�u�W�F�N�g�͂��̃C���^�[�t�F�C�X���p�����Ďg��
*/
__interface IObject
{
	/**
	* @brief �X�V
	* @return true - �X�V �F false - �I�u�W�F�N�g�폜
	*/
	bool Update();

	/**
	* @brief �`��(�V�F�[�_�[���g�p����)
	* @param view - �r���[�s��
	* @param proj - ���e�s��
	*/
	void Render(const Matrix& view, const Matrix& proj);
};

/**
* @interface IMeshObject
* @brief 3D���̃��b�V���I�u�W�F�N�g�C���^�[�t�F�C�X
*
* @note�F�S�Ă�3D���̃Q�[���I�u�W�F�N�g�͂��̃C���^�[�t�F�C�X���p�����Ďg��
*/
__interface IMeshObject : public IObject
{
	/**
	* @brief ���b�V���̎擾
	*/
	const MeshPtr& GetMesh() const;
};

/**
* @interface ISkinObject
* @brief 3D�X�L�����b�V���I�u�W�F�N�g�C���^�[�t�F�C�X
*
* @note�F�S�Ă�3D�X�L���Q�[���I�u�W�F�N�g�͂��̃C���^�[�t�F�C�X���p�����Ďg��
*/
__interface ISkinObject : public IObject
{
	/**
	* @brief �X�L�����b�V���̎擾
	*/
	const SkinMeshPtr& GetSkinMesh() const;
};

/// ���̃��b�V���I�u�W�F�N�g�̃X�}�[�g�|�C���^
using IMeshObjectPtr = shared_ptr<IMeshObject>;
/// �X�L�����b�V���I�u�W�F�N�g�̃X�}�[�g�|�C���^
using ISkinObjectPtr = shared_ptr<ISkinObject>;

/**
* @interface Item
* @brief �A�C�e���C���^�[�t�F�C�X
*/
__interface IItem : public IMeshObject
{
	/**
	* @brief ���W�̎擾
	*/
	const Vector3& GetPos() const;

	/**
	* @brief �_���[�W�t���O���擾����
	*/
	bool IsDamage() const;

	/**
	* @brief �_���[�W�t���O��ݒ肷��
	* @param bFlag - �Z�b�g���邩�ۂ��̃t���O
	* @param playerID - �v���C���[�̔ԍ�
	*/
	void SetDamage(bool bFlag, unsigned int playerID);

	/**
	* @brief �n�ʂɕt���Ă��邩�̃t���O��ݒ肷��
	*/
	void SetGround(bool bFlag);

	/**
	* @brief ���W��ݒ肷��
	*/
	void SetPos(const Vector3& pos);

	/**
	* @brief ���x��ݒ肷��
	*/
	void SetSpeed(const Vector3& speed);

	/**
	* @brief �Ō�ɐG�ꂽ�v���C���[�ԍ����擾����
	*/
	const unsigned int GetID() const;
};


/**
* @interface IStage
* @brief �X�e�[�W�C���^�[�t�F�C�X
*/
__interface IStage : public IMeshObject
{
	/**
	* @brief ���W�̎擾
	*/
	const Vector3& GetPos() const;
};


/**
* @interface ISkyDome
* @brief �X�J�C�h�[���C���^�[�t�F�C�X
*/
__interface ISkyDome : public IMeshObject
{
	/**
	* @brief �A���r�G���g���擾����
	*/
	const Vector4& GetAmbient();
};


/**
* @interface I2DObject
* @brief 2D�I�u�W�F�N�g�C���^�[�t�F�C�X
*
* @note�F�S�Ă�2D�Q�[���I�u�W�F�N�g�͂��̃C���^�[�t�F�C�X���p�����Ďg��
*/
__interface I2DObject
{
	/**
	* @brief �X�V
	* @return true - �X�V �F false - �I�u�W�F�N�g�폜
	*/
	bool Update();

	/**
	* @brief �`��(�V�F�[�_�[���g�p���Ȃ�)
	*/
	void Render();
};

/// 2D�I�u�W�F�N�g�̃X�}�[�g�|�C���^
using I2DObjectPtr = shared_ptr<I2DObject>;

/**
* @interface IEffect
* @brief �G�t�F�N�g�C���^�[�t�F�C�X
*/
__interface IEffect : public I2DObject
{

};


/**
* @interface IParticle
* @brief �p�[�e�B�N���C���^�[�t�F�C�X
*/
__interface IParticle : public I2DObject
{

};


/// �A�C�e���̃X�}�[�g�|�C���^
using IItemPtr = shared_ptr<IItem>;

/// �X�e�[�W�̃X�}�[�g�|�C���^
using IStagePtr = shared_ptr<IStage>;

/// �X�J�C�h�[���̃X�}�[�g�|�C���^
using ISkyDomePtr = shared_ptr<ISkyDome>;

#endif