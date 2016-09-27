/**
* @file   ObjectManager.h
* @brief  �S�I�u�W�F�N�g���Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___OBJECTMANAGER_H
#define ___OBJECTMANAGER_H

#include "Object2D.h"
#include "Object3D.h"

/**
* @brief �I�u�W�F�N�g���Ǘ�����N���X
*/
class ObjectManager
{
public:
	/**
	* @brief �C���X�^���X���擾����i�V���O���g���j
	*/
	static ObjectManager* GetInstance(){
		static ObjectManager singleton;
		return &singleton;
	}

	/**
	* @brief �R���X�g���N�^
	*/
	ObjectManager();

	/**
	* @brief 2D�I�u�W�F�N�g�X�V
	*/
	void Update2D();

	/**
	* @brief 2D�I�u�W�F�N�g�X�V
	*/
	void Update3D();

	/**
	* @brief �`��
	*/
	void Render2D();

	/**
	* @brief �`��(�V�F�[�_�[���g�p����)
	*/
	void Render3D(CameraPtr& camera);

	/**
	* @brief �e��`��(�V���h�E�}�b�v)
	*/
	void RenderShadow(CameraPtr& camera);

	/**
	* @brief �I�u�W�F�N�g��ǉ�����
	* @param obj - �I�u�W�F�N�g�C���^�[�t�F�C�X
	*/
	void Add(const IMeshObjectPtr& obj);

	/**
	* @brief �O�ɒǉ�
	*/
	void AddFront(const I2DObjectPtr& obj);

	/**
	* @brief ���ɒǉ�
	*/
	void AddBack(const I2DObjectPtr& obj);

	/**
	* @brief �v���C���[�I�u�W�F�N�g��ǉ�����
	* @param obj - �I�u�W�F�N�g�C���^�[�t�F�C�X
	*/
	void AddPlayer(const IPlayerPtr& obj);

	/**
	* @brief �A�C�e���I�u�W�F�N�g��ǉ�����
	* @param obj - �I�u�W�F�N�g�C���^�[�t�F�C�X
	*/
	void AddItem(const IItemPtr& obj, ShaderPtr& shader);

	/**
	* @brief �S�ẴI�u�W�F�N�g�̓����蔻����`�F�b�N����
	*/
	void CheckAllCollision(CameraPtr& camera, StockPtr& stock, ScoreManagerPtr& score, DamageData& damage);

	/**
	* @brief �A�C�e�����X�g���擾����
	*/
	list<IItemPtr>& GetItemList();

	/**
	* @brief �폜
	*/
	void Clear();

private:
	/// 2D�I�u�W�F�N�g
	Object2D* m_obj2D;

	/// 3D�I�u�W�F�N�g
	Object3D* m_obj3D;
};

#endif