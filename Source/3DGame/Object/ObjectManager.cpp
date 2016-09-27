#include "ObjectManager.h"

/**
* @brief �R���X�g���N�^
*/
ObjectManager::ObjectManager()
{
	m_obj2D = Object2D::GetInstance();
	m_obj3D = Object3D::GetInstance();
}

/**
* @brief 2D�I�u�W�F�N�g�X�V
*/
void ObjectManager::Update2D()
{
	m_obj2D->Update();
}

/**
* @brief 2D�I�u�W�F�N�g�X�V
*/
void ObjectManager::Update3D()
{
	m_obj3D->Update();
}

/**
* @brief �`��
*/
void ObjectManager::Render2D()
{
	m_obj2D->Render();
}

/**
* @brief �`��
*/
void ObjectManager::Render3D(CameraPtr& camera)
{
	m_obj3D->Render(camera);
}

/**
* @brief �e��`��(�V���h�E�}�b�v)
*/
void ObjectManager::RenderShadow(CameraPtr& camera)
{
	m_obj3D->RenderShadow(camera);
}

/**
* @brief �I�u�W�F�N�g��ǉ�����
* @param obj - �I�u�W�F�N�g�C���^�[�t�F�C�X
*/
void ObjectManager::Add(const IMeshObjectPtr& obj)
{
	m_obj3D->Add(obj);
}

/**
* @brief �O�ɒǉ�
*/
void ObjectManager::AddFront(const I2DObjectPtr& obj)
{
	m_obj2D->AddFront(obj);
}

/**
* @brief ���ɒǉ�
*/
void ObjectManager::AddBack(const I2DObjectPtr& obj)
{
	m_obj2D->AddBack(obj);
}

/**
* @brief �v���C���[�I�u�W�F�N�g��ǉ�����
* @param obj - �I�u�W�F�N�g�C���^�[�t�F�C�X
*/
void ObjectManager::AddPlayer(const IPlayerPtr& obj)
{
	m_obj3D->AddPlayer(obj);
}

/**
* @brief �A�C�e���I�u�W�F�N�g��ǉ�����
* @param obj - �I�u�W�F�N�g�C���^�[�t�F�C�X
*/
void ObjectManager::AddItem(const IItemPtr& obj, ShaderPtr& shader)
{
	m_obj3D->AddItem(obj, shader);
}

/**
* @brief �S�ẴI�u�W�F�N�g�̓����蔻����`�F�b�N����
*/
void ObjectManager::CheckAllCollision(CameraPtr& camera, StockPtr& stock, ScoreManagerPtr& score, DamageData& damage)
{
	m_obj3D->CheckAllCollision(camera, stock, score, damage);
}

/**
* @brief �A�C�e�����X�g���擾����
*/
list<IItemPtr>& ObjectManager::GetItemList()
{
	return m_obj3D->GetItemList();
}

/**
* @brief �폜
*/
void ObjectManager::Clear()
{
	m_obj2D->Clear();
	m_obj3D->Clear();
}

