/**
* @file   Object3D.h
* @brief  3D�I�u�W�F�N�g���Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___OBJECT3D_H
#define ___OBJECT3D_H

#include "..\Character\IPlayer.h"
#include "..\GameSystem\Stock.h"
#include "..\GameSystem\ScoreManager.h"

class DamageData;

/**
* @brief 3D�I�u�W�F�N�g���Ǘ�����N���X
*
* @note�F�V�F�[�_�[���g�p����I�u�W�F�N�g���������X�g�ɓo�^����
*/
class Object3D
{
public:
	/**
	* @brief �C���X�^���X���擾����i�V���O���g���j
	*/
	static Object3D* GetInstance(){
		static Object3D singleton;
		return &singleton;
	}

	/**
	* @brief �I�u�W�F�N�g��ǉ�����
	* @param obj - �I�u�W�F�N�g�C���^�[�t�F�C�X
	*/
	void Add(const IMeshObjectPtr& obj);

	/**
	* @brief �I�u�W�F�N�g��ǉ�����
	* @param obj - �I�u�W�F�N�g�C���^�[�t�F�C�X
	*/
	void Add(const ISkinObjectPtr& obj);

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
	* @brief �X�V
	*/
	void Update();

	/**
	* @brief �`��(�V�F�[�_�[���g�p����)
	*/
	void Render(CameraPtr& camera);

	/**
	* @brief �e��`��(�V���h�E�}�b�v)
	*/
	void RenderShadow(CameraPtr& camera);

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
	/// 3D�I�u�W�F�N�g���X�g
	list<IMeshObjectPtr> m_meshObjList;

	/// 3D�I�u�W�F�N�g���X�g
	list<ISkinObjectPtr> m_skinObjList;

	/// �v���C���[���X�g
	vector<IPlayerPtr> m_players;

	/// �A�C�e�����X�g
	list<IItemPtr> m_itemList;

	/// ���b�V�����X�g(�V���h�E�}�b�v�Ŏg�p)
	list<MeshPtr> m_meshList;

	/// ���b�V�����X�g(�V���h�E�}�b�v�Ŏg�p)
	list<SkinMeshPtr> m_skinList;

	/**
	* @brief �v���C���[�Ə��̓����蔻����`�F�b�N����
	*/
	void CheckCollisionPlayerAndFloor(int index);

	/**
	* @brief �v���C���[�ƍU���̓����蔻����`�F�b�N����
	*/
	void CheckCollisionPlayerAndAttack(CameraPtr& camera, DamageData& damage, int index);

	/**
	* @brief �v���C���[�ƃv���C���[�̓����蔻����`�F�b�N����
	*/
	void CheckCollisionPlayerAndPlayer(int index);

	/**
	* @brief �A�C�e���ƑS�ẴI�u�W�F�N�g�̓����蔻��
	*/
	void CheckCollisionItem(CameraPtr& camera, DamageData& damage, int index);

	/**
	* @brief �A�C�e�����m�̓����蔻��
	*/
	void CheckCollisionItemAndItem(list<IItemPtr>::iterator& item);

	/**
	* @brief �A�C�e���Ə��̓����蔻��
	*/
	void CheckCollisionItemAndFloor();

	/**
	* @brief �v���C���[�̎��S������`�F�b�N����
	*/
	bool CheckDeadPlayer(StockPtr& stock, int index);
};

#endif