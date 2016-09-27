#include "Object3D.h"
#include "Object2D.h"
// ����
#include "..\Common\UniqueData.h"
#include "..\Common\CommonLib.h"
#include "..\Common\Utility.h"
#include "..\Common\STDXDefine.h"
#include "..\Common\CommonParam.h"
// �����蔻��
#include "..\Phyisics\Collision.h"
// �G�t�F�N�g�n
#include "..\Object\Effect\DeadEffect.h"
#include "..\Object\Particle\HitParticle.h"
#include "..\Object\Effect\HitEffect.h"
#include "..\Object\Effect\BurstEffect.h"
#include "..\Object\Effect\ExplosionSmokeEffect.h"
// �V�F�[�_�[
#include "..\Shader\ShaderManager.h"

/**
* @brief �I�u�W�F�N�g��ǉ�����
* @param obj - �I�u�W�F�N�g�C���^�[�t�F�C�X
* @param mesh - ���b�V���f�[�^
*/
void Object3D::Add(const IMeshObjectPtr& obj)
{
	// �I�u�W�F�N�g���X�g�ɒǉ�
	m_meshObjList.emplace_back(obj);
}

/**
* @brief �I�u�W�F�N�g��ǉ�����
* @param obj - �I�u�W�F�N�g�C���^�[�t�F�C�X
* @param mesh - �X�L�����b�V���f�[�^
*/
void Object3D::Add(const ISkinObjectPtr& obj)
{
	// �I�u�W�F�N�g���X�g�ɒǉ�
	m_skinObjList.emplace_back(obj);
}

/**
* @brief �v���C���[�I�u�W�F�N�g��ǉ�����
* @param obj - �I�u�W�F�N�g�C���^�[�t�F�C�X
*/
void Object3D::AddPlayer(const IPlayerPtr& obj)
{
	// �I�u�W�F�N�g���X�g�ɒǉ�
	m_skinObjList.emplace_back(obj);
	// �v���C���[���X�g�ɒǉ�
	m_players.emplace_back(obj);
}

/**
* @brief �A�C�e���I�u�W�F�N�g��ǉ�����
* @param obj - �I�u�W�F�N�g�C���^�[�t�F�C�X
*/
void Object3D::AddItem(const IItemPtr& obj, ShaderPtr& shader)
{
	// �V�F�[�_�[�̐ݒ�
	obj->GetMesh()->SetShader(shader);
	// �A�C�e�����X�g�ɒǉ�
	m_itemList.emplace_back(obj);
}

/**
* @brief �I�u�W�F�N�g���X�V����
*/
void Object3D::Update()
{
	// �C�e���[�^���擾
	auto mesh = m_meshObjList.begin();
	// ���X�g�ɒǉ������I�u�W�F�N�g���ꊇ�X�V
	while (mesh != m_meshObjList.end())
	{
		// �X�V
		if ((*mesh)->Update()){
			++mesh;
		}
		// �X�V�t���O��false�Ȃ�v�f���폜����
		else{
			mesh = m_meshObjList.erase(mesh);
		}
	}

	// �A�C�e���̍X�V
	// �C�e���[�^���擾
	auto item = m_itemList.begin();
	// ���X�g�ɒǉ������I�u�W�F�N�g���ꊇ�X�V
	while (item != m_itemList.end())
	{
		// �X�V
		if ((*item)->Update()){
			++item;
		}
		// �X�V�t���O��false�Ȃ�v�f���폜����
		else{
			item = m_itemList.erase(item);
		}
	}

	// �C�e���[�^�擾
	auto skin = m_skinObjList.begin();
	// ���X�g�ɒǉ������I�u�W�F�N�g���ꊇ�X�V
	while (skin != m_skinObjList.end())
	{
		// �X�V
		if ((*skin)->Update()){
			++skin;
		}
		// �X�V�t���O��false�Ȃ�v�f���폜����
		else{
			skin = m_skinObjList.erase(skin);
		}
	}
}

/**
* @brief �I�u�W�F�N�g��`�悷��
*/
void Object3D::Render(CameraPtr& camera)
{
	// �V�F�[�_�[�}�l�[�W���[�̃C���X�^���X���擾����
	ShaderManager* mgr = ShaderManager::GetInstance();

	// ���X�g�ɒǉ������X�L���I�u�W�F�N�g���ꊇ�`��
	// �͈̓x�[�X���[�v
	for (auto& it : m_skinObjList)
	{
		
		// �`�悷��O��
		// �V�F�[�_�[�p�����[�^�̐ݒ���s�Ȃ�
		mgr->Update(it->GetSkinMesh()->GetTransMatrix(), camera);
		// �`��
		it->Render(camera->GetView(), camera->GetProj());
	}

	// �A�C�e���̕`��
	for (auto& item : m_itemList)
	{
		// �`�悷��O��
		// �V�F�[�_�[�p�����[�^�̐ݒ���s�Ȃ�
		mgr->Update(item->GetMesh()->GetTransMatrix(), camera);
		// �`��
		item->Render(camera->GetView(), camera->GetProj());
	}

	// ���X�g�ɒǉ��������̃I�u�W�F�N�g���ꊇ�`��
	// �͈̓x�[�X���[�v
	for (auto& it : m_meshObjList)
	{
		// �`�悷��O��
		// �V�F�[�_�[�p�����[�^�̐ݒ���s�Ȃ�
		mgr->Update(it->GetMesh()->GetTransMatrix(), camera);
		// �`��
		it->Render(camera->GetView(), camera->GetProj());
	}
}

/**
* @brief �e��`��(�V���h�E�}�b�v)
*/
void Object3D::RenderShadow(CameraPtr& camera)
{
	// �V�F�[�_�[�}�l�[�W���[�̃C���X�^���X���擾����
	ShaderManager* mgr = ShaderManager::GetInstance();

	// �V���h�E�}�b�v�V�F�[�_�[�J�n
	mgr->GetShadowMapShader()->Begine();

	//	for (auto& it : m_skinObjList)
	//	{
	//		// �`�悷��O��
	//		// �V�F�[�_�[�p�����[�^�̐ݒ�
	//		mgr->Update(it->GetSkinMesh()->GetTransMatrix(), camera);
	//		// �e�̕`��
	//		it->GetSkinMesh()->DrawShadow();
	//	}

	// ���̃��b�V��
	for (auto& it : m_meshObjList)
	{
		// �`�悷��O��
		// �V�F�[�_�[�p�����[�^�̐ݒ�
		mgr->Update(it->GetMesh()->GetTransMatrix(), camera);
		// �e�̕`��
		it->GetMesh()->DrawShadow();
	}

	// �A�C�e���̕`��
	for (auto& item : m_itemList)
	{
		// �`�悷��O��
		// �V�F�[�_�[�p�����[�^�̐ݒ���s�Ȃ�
		mgr->Update(item->GetMesh()->GetTransMatrix(), camera);
		// �`��
		item->GetMesh()->DrawShadow();
	}

	//  �V���h�E�}�b�v�V�F�[�_�[�I��
	mgr->GetShadowMapShader()->End();
}

/**
* @brief �����蔻����`�F�b�N����
*/
void Object3D::CheckAllCollision(CameraPtr& camera,
	StockPtr& stock,
	ScoreManagerPtr& score,
	DamageData& damage)
{
	// �����蔻��̃C���X�^���X
	Collision* collision = Collision::GetInstance();

	// �v���C���[�̐��������[�v
	for (int i = 0; i != m_players.size(); i++)
	{
		// ���^�C�A���Ă��炻�̃v���C���[�ԍ��͏��������Ȃ�
		if (m_players[i]->IsRetire()) continue;

		// �v���C���[�Ə��̓����蔻��
		CheckCollisionPlayerAndFloor(i);

		// �v���C���[�ƍU���̓����蔻��
		CheckCollisionPlayerAndAttack(camera, damage, i);

		// �v���C���[�ƃv���C���[�̓����蔻��
		CheckCollisionPlayerAndPlayer(i);

		// �A�C�e���̓����蔻��
		CheckCollisionItem(camera, damage, i);

		// �v���C���[�̎��S����
		if (CheckDeadPlayer(stock, i))
		{
			// ���S(�N�ɓ|���ꂽ����߂�l�Ŏ擾)
			int killPlayer = m_players[i]->Dead();

			// ���j�����v���C���[�̃X�R�A�����Z����
			if (killPlayer != -1){
				score->AddScore(killPlayer);
			}

			// ���j���ꂽ�v���C���[�̓X�R�A�����Z����
			score->SubScore(i);

			// �_���[�W������
			damage.SetReset(true, i);

			// ���������
			int rand = Utility::Random(0, 3);
			float returnPosX;
			// ���ʂɉ����ĕ��A���W��ς���
			if		(rand == 0) returnPosX = GetComPrm->Pattern1ReturnPosX;
			else if (rand == 1) returnPosX = GetComPrm->Pattern2ReturnPosX;
			else if (rand == 2) returnPosX = GetComPrm->Pattern3ReturnPosX;
			else if (rand == 3) returnPosX = GetComPrm->Pattern4ReturnPosX;
			Vector3 pos = Vector3(returnPosX, GetComPrm->ReturnPosY, GetComPrm->InitPosZ);
			// ���W�̃Z�b�g
			m_players[i]->SetPos(pos);
		}
	}

	// �A�C�e���Ə��̓����蔻��
	CheckCollisionItemAndFloor();
}

/**
* @brief �v���C���[�Ə��̓����蔻����`�F�b�N����
*/
void Object3D::CheckCollisionPlayerAndFloor(int index)
{
	// �����蔻��̃C���X�^���X
	Collision* collision = Collision::GetInstance();

	// �v���C���[�̐����擾
	int plNum = m_players.size();

	// �����蔻��
	vector<bool> isGround;
	if (isGround.size() == NULL){
		isGround.resize(plNum);
	}
	// �������Ă��Ȃ�
	isGround[index] = false;

	// �v���C���[�̍��W���擾����
	Vector3 playerPos = m_players[index]->GetPos();

	// �����蔻��
	if (playerPos.y < collision->GetHeight(playerPos))
	{
		// �������Ă���
		isGround[index] = true;

		// ���W�␳
		playerPos.y = collision->GetHeight(playerPos);

		// ���W�Z�b�g
		m_players[index]->SetPos(playerPos);
	}

	// �n�ʂɕt���Ă邩�Z�b�g
	m_players[index]->SetGround(isGround[index]);
}

/**
* @brief �v���C���[�ƍU���̓����蔻����`�F�b�N����
*/
void Object3D::CheckCollisionPlayerAndAttack(
	CameraPtr& camera,
	DamageData& damage,
	int index)
{
	// �����蔻��̃C���X�^���X
	Collision* collision = Collision::GetInstance();
	Object2D* mgr2d = Object2D::GetInstance();

	for (int i = 0; i != m_players.size(); i++)
	{
		// �����͏���
		if (i == index){
			continue;
		}
		// �Ώۃv���C���[�ɍU�����肪���������瓖����Ȃ�
		if (!m_players[index]->IsAttack(i)) {
			continue;
		}
		// ���G�t���O�������Ă���L�����N�^�[������(������Ȃ�)
		if (m_players[i]->IsInvincible()){
			continue;
		}

		// ��e�͈͂��擾
		Vector3 damagePos = m_players[i]->GetDamagePos();
		// �U���͈͂��擾
		Vector3 hitPos = m_players[index]->GetHitPos();

		// �����蔻��(���a1.0f�̋�)
		if (collision->CheckShpere(damagePos, 1.0f, hitPos, 1.0f))
		{
			// ������ё��x
			Vector3 blown = Utility::BlownOffSpeed(typeid(Rock), m_players, index, i, damage.GetDamageRate(i));

			// ��e��Ԃɂ���
			m_players[i]->Damage(blown, index);

			// ������ї������Z
			damage.AddDamageRate(m_players[index]->GetPower(), i);

			// ���ʉ��Đ�
			SoundPlayer::GetInstance()->Play(HIT_SE);

			// �G�t�F�N�g�ǉ�
			Vector3 effectPos = m_players[i]->GetDamagePos();
			mgr2d->AddFront(make_shared<HitEffect>(effectPos, camera->GetView()));
			for (int effect = 0; effect < 5; effect++){
				mgr2d->AddFront(make_shared<HitParticle>(effectPos, camera->GetView()));
			}

			// �U�������������v���C���[�ɂ͘A�����čU����������Ȃ�
			m_players[index]->SetAttack(i, false);
		}
	}
}

/**
* @brief �v���C���[�ƃv���C���[�̓����蔻����`�F�b�N����
*/
void Object3D::CheckCollisionPlayerAndPlayer(int index)
{
	// �����蔻��̃C���X�^���X
	Collision* collision = Collision::GetInstance();

	for (int i = 0; i != m_players.size(); i++)
	{
		// �����͏���
		if (i == index){
			continue;
		}

		// �v���C���[�̍��W���擾����
		Vector3 obj1 = m_players[index]->GetPos();
		Vector3 obj2 = m_players[i]->GetPos();

		// �����擾
		Vector3 diffVec = Utility::DiffVector(obj2, obj1);

		// �����蔻��
		if (collision->CheckShpere(obj1, 1.0f, obj2, 1.0f))
		{
			// ���W�̏C��
			m_players[index]->SetPos(m_players[index]->GetPos() + diffVec * 0.07f);
		}
	}
}

// ��������̍��W
#define FALLING_DECISION_POS -30.0f
// �����̎��S����
#define DEAD_DECISION_LEFT_POS 40.0f
// �E���̎��S����
#define DEAD_DECISION_RIGHT_POS -40.0f

/**
* @brief �v���C���[�̎��S������`�F�b�N����
*/
bool Object3D::CheckDeadPlayer(StockPtr& stock, int index)
{
	Object2D* mgr2d = Object2D::GetInstance();

	// ����
	bool result = false;

	// ��������
	if (m_players[index]->GetPos().y < FALLING_DECISION_POS)
	{
		// �G�t�F�N�g�o�^
		mgr2d->AddFront(make_shared<DeadEffect>(0));

		// ����������n�m�ɂ���
		result = true;
	}

	// �����̔���
	if (m_players[index]->GetPos().x > DEAD_DECISION_LEFT_POS)
	{
		// �G�t�F�N�g�o�^
		mgr2d->AddFront(make_shared<DeadEffect>(1));

		// ����������n�m�ɂ���
		result = true;
	}

	// �E���̔���
	if (m_players[index]->GetPos().x < DEAD_DECISION_RIGHT_POS)
	{
		// �G�t�F�N�g�o�^
		mgr2d->AddFront(make_shared<DeadEffect>(2));

		// ����������n�m�ɂ���
		result = true;
	}

	// ���S���肪�����Ă�����
	if (result)
	{
		// �X�g�b�N���[�h��������X�g�b�N�������炷
		if (UniqueData::GetInstance()->GetBattleMode() == BattleMode::Stock){
			stock->MinusStock(index);
		}
	}

	// ����
	return result;
}

/**
* @brief �A�C�e���ƑS�ẴI�u�W�F�N�g�̓����蔻��
*/
void Object3D::CheckCollisionItem(CameraPtr& camera, DamageData& damage, int index)
{
	Object2D* mgr2d = Object2D::GetInstance();
	// �����蔻��̃C���X�^���X
	Collision* collision = Collision::GetInstance();

	// �v���C���[�̍��W���擾����
	Vector3 playerPos = m_players[index]->GetPos();

	// �C�e���[�^���擾
	auto item = m_itemList.begin();

	while (item != m_itemList.end())
	{
		// �A�C�e���̍��W���擾����
		Vector3 itemPos = (*item)->GetPos();

		// �A�C�e�����m�̓����蔻��
		CheckCollisionItemAndItem(item);

		// �U���͈͂��擾
		Vector3 hitPos = m_players[index]->GetHitPos();

		bool flag = false;

		// �v���C���[�̍U���ƃA�C�e���̓����蔻��
		if (collision->CheckShpere(hitPos, 1.0f, itemPos, 1.0f))
		{
			// �U�����肪�o�Ă���Ƃ�����
			if (m_players[index]->GetAction() == Action::NormalAttack ||
				m_players[index]->GetAction() == Action::SpecialAttackUp ||
				m_players[index]->GetAction() == Action::SmashAttack){

				// ������ё��x
				Vector3 blown = Utility::BlownOffSpeed(m_players, index, itemPos, 1.0f);
				blown.z = 0.0f;
				// ���ʉ��Đ�
				SoundPlayer::GetInstance()->Play(HIT_SE);

				// �G�t�F�N�g�ǉ�
				Vector3 effectPos = (*item)->GetPos();
				mgr2d->AddFront(make_shared<HitEffect>(effectPos, camera->GetView()));
				for (int effect = 0; effect < 15; effect++){
					mgr2d->AddBack(make_shared<HitParticle>(effectPos, camera->GetView()));
				}

				// �_���[�W�t���O���n�m
				(*item)->SetDamage(true, index);

				// ���x�̐ݒ�
				(*item)->SetSpeed(blown);
			}
			else{
				flag = true;
			}
		}

		// ����������������폜����
		if ((*item)->GetPos().y < FALLING_DECISION_POS){
			item = m_itemList.erase(item);
			continue;
		}
		// �����̔���
		if ((*item)->GetPos().x > DEAD_DECISION_LEFT_POS)
		{
			item = m_itemList.erase(item);
			continue;
		}

		// �E���̔���
		if ((*item)->GetPos().x < DEAD_DECISION_RIGHT_POS)
		{
			item = m_itemList.erase(item);
			continue;
		}

		// �_���[�W�t���O�������Ă��Ȃ�������I��
		if (!(*item)->IsDamage()) {
			++item;
			continue;
		}

		// �폜�t���O
		bool deadFlag = false;

		for (int i = 0; i != m_players.size(); i++)
		{
			// ���W���擾
			Vector3 pos = m_players[i]->GetPos();
			// �_���[�W�t���O�n�m�̃A�C�e���ƃv���C���[���g�̓����蔻��
			if (collision->CheckShpere(itemPos, 1.3f, pos, 1.3f) && flag)
			{
				// ������ё��x
				Vector3 blown = Utility::BlownOffSpeed(m_players, i, itemPos, 50.0f);

				// ��e��Ԃɂ���
				m_players[i]->Damage(blown, (*item)->GetID());

				// ������ї������Z(�Ƃ肠�����ꗥ�Őݒ肵�Ă���)
				damage.AddDamageRate(60, i);

				// ���ʉ��Đ�
				SoundPlayer::GetInstance()->Play(BOMB_HIT_SE);

				// �G�t�F�N�g�ǉ�
				Vector3 effectPos = itemPos;
				// ���G�t�F�N�g
				mgr2d->AddFront(make_shared<ExplosionSmokeEffect>(effectPos, camera->GetView()));
				for (int e = 0; e < 3; e++){
				// �Ռ��g�G�t�F�N�g
					mgr2d->AddFront(make_shared<BurstEffect>(effectPos, camera->GetView()));
				}
				// ��e�G�t�F�N�g
				for (int effect = 0; effect < 15; effect++){
					mgr2d->AddFront(make_shared<HitParticle>(effectPos, camera->GetView()));
				}

				// �U�������������v���C���[�ɂ͘A�����čU����������Ȃ�
				m_players[i]->SetAttack(i, false);

				// �폜����
				deadFlag = true;
			}
		}

		// �폜�t���O�������Ă��������
		if (deadFlag){
			item = m_itemList.erase(item);
		}
		else{
			++item;
		}
	}
}

/**
* @brief �A�C�e�����m�̓����蔻��
*/
void Object3D::CheckCollisionItemAndItem(list<IItemPtr>::iterator& item)
{
	// �����蔻��̃C���X�^���X
	Collision* collision = Collision::GetInstance();

	// �C�e���[�^���擾
	auto it = m_itemList.begin();
	while (it != m_itemList.end())
	{
		// �����͏���
		if (it == item){
			++it;
			continue;
		}

		// �A�C�e���̍��W���擾����
		Vector3 itemPos = (*item)->GetPos();
		Vector3 itemPos2 = (*it)->GetPos();

		// �����擾
		Vector3 diffVec = Utility::DiffVector(itemPos, itemPos2);

		// �����蔻��
		if (collision->CheckShpere(itemPos, 1.0f, itemPos2, 1.0f))
		{
			// ���W�̏C��
			(*it)->SetPos(itemPos2 + diffVec * 0.07f);
		}
		++it;
	}
}

/**
* @brief �A�C�e���Ə��̓����蔻��
*/
void Object3D::CheckCollisionItemAndFloor()
{
	// �����蔻��̃C���X�^���X
	Collision* collision = Collision::GetInstance();

	auto item = m_itemList.begin();

	while (item != m_itemList.end())
	{
		// �A�C�e���̍��W���擾����
		Vector3 itemPos = (*item)->GetPos();

		// �A�C�e���Ə��̓����蔻��
		if (itemPos.y < collision->GetHeight(itemPos))
		{
			// ���W�␳
			itemPos.y = collision->GetHeight(itemPos);

			// �ڒn�t���O�n�m
			(*item)->SetGround(true);

			// ���W�Z�b�g
			(*item)->SetPos(itemPos);
		}
		else{
			// �ڒn�t���O�n�e�e
			(*item)->SetGround(false);
		}

		++item;
	}
}

/**
* @brief �A�C�e�����X�g���擾����
*/
list<IItemPtr>& Object3D::GetItemList()
{
	return m_itemList;
}

/**
* @brief ���X�g���̗v�f�폜
*/
void Object3D::Clear()
{
	m_meshObjList.clear();
	m_skinObjList.clear();
	m_players.clear();
	m_itemList.clear();
}