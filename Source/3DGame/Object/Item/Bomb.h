/**
* @file   Bomb.h
* @brief  �A�C�e���{���̏������Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___BOMB_H
#define ___BOMB_H

#include "..\IObject.h"

/**
* @brief �{���A�C�e���N���X
*/
class Bomb : public IItem
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	Bomb(const Vector3& pos);

	/**
	* @brief �f�X�g���N�^
	*/
	~Bomb();

	/**
	* @brief �X�V
	*/
	bool Update();

	/**
	* @brief �`��(�V�F�[�_�[���g�p����)
	*/
	void Render(const Matrix& view, const Matrix& proj);

	/**
	* @brief ���[���h�s����擾����
	*/
	const Matrix& GetWorld();

	/**
	* @brief ���W�̎擾
	*/
	const Vector3& GetPos() const;

	/**
	* @brief �_���[�W�t���O
	*/
	bool IsDamage() const;

	/**
	* @brief �n�ʂɕt���Ă��邩�̃t���O��ݒ肷��
	*/
	void SetGround(bool bFlag);

	/**
	* @brief �_���[�W�t���O��ݒ肷��
	* @param bFlag - �Z�b�g���邩�ۂ��̃t���O
	* @param playerID - �v���C���[�̔ԍ�
	*/
	void SetDamage(bool bFlag, unsigned int playerID);

	/**
	* @brief ���W��ݒ肷��
	*/
	void SetPos(const Vector3& pos);

	/**
	* @brief ���x��ݒ肷��
	*/
	void SetSpeed(const Vector3& speed);

	/**
	* @brief ���b�V�����擾����
	*/
	const MeshPtr& GetMesh() const;

	/**
	* @brief �Ō�ɐG�ꂽ�v���C���[�ԍ����擾����
	*/
	const unsigned int GetID() const;

private:
	/// �I�u�W�F�N�g
	MeshPtr m_obj;

	/// ���W
	Vector3 m_pos;

	/// ���x
	Vector3 m_spd;

	/// �d��
	float m_gravity;

	/// �����t���O
	bool m_isExplosion;

	/// �ڒn�t���O
	bool m_isGround;

	/// �ԍ�
	unsigned int m_id;
};

#endif