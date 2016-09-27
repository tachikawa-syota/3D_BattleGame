#include "Object2D.h"

/**
* @brief �O�ɒǉ�
*/
void Object2D::AddFront(const I2DObjectPtr& obj)
{
	m_objList.emplace_front(obj);
}

/**
* @brief ���ɒǉ�
*/
void Object2D::AddBack(const I2DObjectPtr& obj)
{
	m_objList.emplace_back(obj);
}

/**
* @brief �I�u�W�F�N�g���X�V����
*/
void Object2D::Update()
{
	// �C�e���[�^���擾
	auto it = m_objList.begin();

	while (it != m_objList.end())
	{
		// �X�V
		if ((*it)->Update()){
			++it;
		}
		// �X�V�t���O��false�Ȃ�v�f���폜����
		else{
			it = m_objList.erase(it);
		}
	}
}

/**
* @brief �I�u�W�F�N�g��`�悷��
*/
void Object2D::Render()
{
	// �͈̓x�[�X���[�v
	for (auto& obj : m_objList)
	{
		obj->Render();
	}
}

/**
* @brief ���X�g���̗v�f�폜
*/
void Object2D::Clear()
{
	m_objList.clear();
}
