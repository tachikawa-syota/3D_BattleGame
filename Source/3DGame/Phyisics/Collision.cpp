#include "Collision.h"


Collision::~Collision()
{

}

/**
* @brief �X�e�[�W�̃��b�V����ݒ肷��
* @param �X�e�[�W�̃��b�V�����
*/
void Collision::SetStageObj(MeshPtr obj)
{
	m_stage = obj; 
}

/**
* @brief ���Ƃ̓����蔻��
* @param �v���C���[�̍��W
*/
float Collision::GetHeight(const Vector3& pos)
{
	Vector3	out;
	Vector3	p = pos + Vector3(0, 2.0f, 0);
	Vector3 v(0, -1, 0);
	float d = 2.0f;
	// ���C�s�b�N
	int index = m_stage->RayPick(&out, &p, &v, &d);
	if (index < 0){
		return pos.y;
	}
	return out.y;
}

/**
* @brief ���̓��m�̓����蔻��
* @param pos - �I�u�W�F�N�g�̍��W
* @param radius - �I�u�W�F�N�g�̔��a
*/
bool Collision::CheckShpere(const Vector3& posA, float radiusA, const Vector3& posB, float radiusB)
{
	// �ŏ��͓������Ă��Ȃ����Z�b�g����
	bool isHit = false;

	// ����
	Vector3 v = posB - posA;
	float	d = v.Length();
	// �����@< ���Ĕ��a + �󂯔��a
	if (d < radiusA + radiusB){
		isHit = true;
	}

	return isHit;
}
