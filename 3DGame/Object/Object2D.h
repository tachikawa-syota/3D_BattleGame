/**
* @file   Object2D.h
* @brief  2D�I�u�W�F�N�g���Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___OBJECT2D_H
#define ___OBJECT2D_H

#include "IObject.h"

/**
* @brief 2D�I�u�W�F�N�g���Ǘ�����N���X
*/
class Object2D
{
public:
	/**
	* @brief �C���X�^���X���擾����i�V���O���g���j
	*/
	static Object2D* GetInstance(){
		static Object2D singleton;
		return &singleton;
	}

	/**
	* @brief �X�V
	*/
	void Update();

	/**
	* @brief �`��
	*/
	void Render();

	/**
	* @brief �O�ɒǉ�
	*/
	void AddFront(const I2DObjectPtr& obj);

	/**
	* @brief ���ɒǉ�
	*/
	void AddBack(const I2DObjectPtr& obj);

	/**
	* @brief �폜
	*/
	void Clear();

private:
	/// 2D�I�u�W�F�N�g���X�g
	list<I2DObjectPtr> m_objList;
};

#endif