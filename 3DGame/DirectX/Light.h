/**
* @file Light.h
* @brief ���C�g�̏������Ǘ�����
* @author Syota Tachikawa
*/

#ifndef ___LIGHT_H
#define ___LIGHT_H

#include "..\Math\Math.h"
#include "..\Shader\Shader.h"

/**
* @brief ���C�g�N���X
*/
class Light
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	Light();

	/**
	* @brief ���C�g�L����
	* @param ���C�g�ԍ�
	*/
	void EnableLight(DWORD index);
	
	/**
	* @brief ���C�g������
	* @param ���C�g�ԍ�
	*/
	void DisableLight(DWORD index);

	/**
	* @brief �����̐ݒ�
	* @param ����
	*/
	void SetAmbient(DWORD ambient);

	/**
	* @brief �t�H�O�̐ݒ�
	* @param param1 - �J�n�ʒu
	* @param param2 - �I���ʒu
	* @param color  - �F
	*/
	void SetFog(float Param1, float Param2, DWORD Color);

	/**
	* @brief ���s�����̐ݒ�
	* @param index - ���C�g�̔ԍ�
	* @param dir - ����
	* @param r,g,b - RGB�J���[
	*/
	void DirLight(int index, Vector3 dir, float r, float g, float b);
	
	/**
	* @brief ���s�����̐ݒ�
	* @param shader - �V�F�[�_�[
	* @param index - ���C�g�̔ԍ�
	* @param dir   - ����
	* @param r,g,b - RGB�J���[
	*/
	void DirLight(ShaderPtr shader, int index, Vector3 dir, float r, float g, float b);

	/**
	* @brief �_�����̐ݒ�
	* @param index - ���C�g�̔ԍ�
	* @param pos - ���W
	* @param r,g,b - RGB�J���[
	* @param range ���C�g�͈̔�
	*/
	void PointLight(int index, Vector3 Pos, float r, float g, float b, float range);
	
	/**
	* @brief �X�|�b�g���C�g�̐ݒ�
	* @param index - ���C�g�̔ԍ�
	* @param pos   - ���W
	* @param r,g,b - RGB�J���[
	* @param range - ���C�g�͈̔�
	*/
	void SpotLight(int index, Vector3 Pos, float r, float g, float b, float range);

private:
	/// �f�o�C�X
	LPDIRECT3DDEVICE9 m_pDevice;
};

#endif