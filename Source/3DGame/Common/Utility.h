/**
* @file   Utility.h
* @brief  �L�p���̂��鏈�����Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___UTILITY_H
#define ___UTILITY_H

#include "..\GameSystem\CorrectionValue.h"
#include "..\GameSystem\DamageData.h"

// �x���}��
#pragma warning(disable:4172)

using std::mt19937;
using std::random_device;
using std::uniform_int_distribution;
using std::uniform_real_distribution;

/**
* @brief ���[�e�B���e�B�N���X
*/
class Utility
{
public:
	/**
	* @brief �����^������Ԃ�
	* @param min - �ŏ��l
	* @param max - �ő�l
	*
	* @note�F�����Z���k�E�c�C�X�^
	*/
	static int Random(int min, int max)
	{
		// �����Z���k�E�c�C�X�^
		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> rad(min, max);
		int result = rad(mt);

		return result;
	}

	/**
	* @brief ���������_������Ԃ�
	* @param min - �ŏ��l
	* @param max - �ő�l
	*
	* @note�F�����Z���k�E�c�C�X�^
	*/
	static float Random(float min, float max)
	{
		random_device rd;
		mt19937 mt(rd());
		uniform_real_distribution<float> rad(min, max);
		float result = rad(mt);

		return result;
	}

	/**
	* @brief ������ԑ��x���v�Z����
	* @param obj1 - �U���҂̃v���C���[�ԍ�
	* @param obj2 - ��e�҂̃v���C���[�ԍ�
	* @param rate - �_���[�W��
	* @return �v�Z���ꂽ���x
	*/
	static Vector3 BlownOffSpeed(const type_info& type, vector<IPlayerPtr>& player, int obj1, int obj2, DWORD rate)
	{
		// ���������Ă���
		Vector3 result = Vector3();
		// �����擾
		result = Utility::DiffVector(player[obj1]->GetPos(), player[obj2]->GetPos());
		
		// ���K��
		result.Normalize();

		// ��e�҂��n�ʂƐڂ��Ă����������ɔ�΂�
		if (player[obj2]->IsGround()){
			result.y += 0.5f;
			// ���K��
			result.Normalize();
		}

		// �L�����N�^�[�ƍU���ɉ����ĕ␳�l���擾����
		float value = CorrectionValue::GetInstance()->GetValue(type, player, obj1);

		// �_���[�W���ƕ␳�l����Z����
		result = result * value * static_cast<float>(rate);

		// ����
		return result;
	}

	/**
	* @brief ������ԑ��x���v�Z����(�v���C���[�ƃA�C�e��)
	* @param player - �v���C���[�N���X
	* @param index - �v���C���[�ԍ�
	* @param item - �A�C�e���̃C�e���[�^
	* @param value - ������ѕ␳�l
	* @return �v�Z���ꂽ���x
	*/
	static Vector3 BlownOffSpeed(vector<IPlayerPtr>& player, int index, Vector3& itemPos, float value)
	{
		// ���������Ă���
		Vector3 result = Vector3();
		// �����擾
		result = Utility::DiffVector(player[index]->GetPos(), itemPos);
		
		// ���K��
		result.Normalize();

		// y�l�̕␳��������
		result.y += 0.4f;

		// ���K��
		result.Normalize();

		float speed = static_cast<float>(player[index]->GetPower());
		if (speed <= 0.0f){
			speed -= value;
		}
	
		// ���x�̕␳
		result = result * 0.04f * speed;

		// ����
		return result;
	}

	/**
	* @brief �x�N�g���̍������擾����
	*/
	static Vector3&& DiffVector(const Vector3& vector1, const Vector3& vector2)
	{
		Vector3 vec1 = vector1;
		Vector3 vec2 = vector2;

		Vector3 vec3 = vec2 - vec1;

		return Vector3(vec3.x, vec3.y, vec3.z);
	}

	/**
	* @brief ��ԃ_���[�W�������Ă���v���C���[���擾
	* @param player - �v���C���[�C���^�[�t�F�C�X�̃X�}�[�g�|�C���^
	* @param damage - �_���[�W�f�[�^�N���X
	* @param index - ���g�̔ԍ�
	* @return ��ԃ_���[�W�������Ă���v���C���[
	*/
	static IPlayerPtr GetMostDamagePlayer(vector<IPlayerPtr>& player, DamageData damage, int index)
	{
		DWORD minDamage = 0;
		IPlayerPtr result = nullptr;

		for (int i = 0; i != player.size(); i++)
		{
			// �����͏���
			if (i == index){
				continue;
			}

			// �_���[�W�����擾����
			DWORD temp = damage.GetDamageRate(i);
			
			// ��r����
			if (temp > minDamage){
				minDamage = temp;
				result = player[i];
			}
		}

		return result;
	}

	/**
	* @brief �����ƈ�ԋ߂��v���C���[���擾
	* @param player - �v���C���[�C���^�[�t�F�C�X�̃X�}�[�g�|�C���^
	* @param index - ���g�̔ԍ�
	* @return �����ƈ�ԋ߂��v���C���[
	*/
	static IPlayerPtr GetNearPlayer(vector<IPlayerPtr>& player, int index)
	{
		float minDistance = 999.0f;

		IPlayerPtr result = nullptr;

		for (int i = 0; i != player.size(); i++)
		{
			// �����͏���
			if (i == index){
				continue;
			}
			// ���^�C�A���Ă���v���C���[������
			if (player[i]->IsRetire()){
				continue;
			}

			// �����̈ʒu
			Vector3 plPos = player[index]->GetPos();

			// ����̈ʒu
			Vector3 emPos = player[i]->GetPos();

			// ����
			Vector3 diff = DiffVector(plPos, emPos);

			// ����
			float distance = diff.Length();

			if (distance < minDistance){
				minDistance = distance;
				result = player[i];
			}
		}

		return result;
	}

	/**
	* @brief ��ԗ���Ă���v���C���[���m�̋����𑪂�
	* @param �v���C���[�̃X�}�[�g�|�C���^
	*/
	static float GetFarPlayerDistance(const vector<IPlayerPtr>& player)
	{
		// ���݂���v���C���[���̔z����m��
		vector<float> posX;
		posX.resize(player.size());

		// �v���C���[��X���W���R�s�[����
		for (int i = 0; i != player.size(); i++){
			posX[i] = player[i]->GetPos().x;
			
			// ���^�C�A���Ă�v���C���[�͌v�Z�Ɋ܂߂Ȃ�
			if (player[i]->IsRetire()){
				posX[i] = 0.0f;
			}
		}

		// X���W�̍ŏ��l
		float min = *min_element(posX.begin(), posX.end());
		// X���W�̍ő�l
		float max = *max_element(posX.begin(), posX.end());
		
		Vector3 minVec, maxVec;
		// X���W���ŏ��ƍő�̃v���C���[�̃x�N�g�����擾����
		for (int i = 0; i != player.size(); i++)
		{
			if (player[i]->GetPos().x == min){
				minVec = player[i]->GetPos();
			}
			if (player[i]->GetPos().x == max){
				maxVec = player[i]->GetPos();
			}
		}

		// ����
		Vector3 diff = Utility::DiffVector(maxVec, minVec);

		// ����
		float len = diff.Length();

		// ������Ԃ�
		return len;
	}

	/**
	* @brief �_�C�A���O�{�b�N�X��\�����Ȃ���A�v���P�[�V�������I������
	*/
	static void abortWithMsg(char* msg)
	{
		MessageBox(NULL, msg, "�Ӑ}���Ȃ��I��", MB_OK | MB_ICONERROR);
		exit(0);
	}
};

#endif