//===================================================
//
// �G�l�~�[�̏���
// Author:Teruto Sato
//
//===================================================

//------------------------
// �C���N���[�h
//------------------------
#include <cmath>
#include "enemy.h"
#include "object.h"
#include "application.h"
#include "renderer.h"
#include "model.h"
#include "line.h"
#include "game.h"
#include "player.h"
#include "fade.h"
#include "hp.h"

//------------------------
// �O���[�o���ϐ�
//------------------------
CEnemy::KEY_SET g_aKeySet[] =	//�L�[�Z�b�g���
{
	//----------------------
	// �L�[1
	//----------------------
	{ 40,	//�t���[����
			//		Pos				Rot
	{ { 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },},
	},

	//----------------------
	// �L�[2
	//----------------------
	{ 40,	//�t���[����
			//		Pos				Rot
	{ { 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f }, },	//��
	},
};

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
const float CEnemy::fDefGravity = 0.8f;	//��{�̏d��

//========================
// �R���X�g���N�^
//========================
CEnemy::CEnemy() : CObject(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�傫���̍ő�l
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�傫���̍ŏ��l
	m_worldMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���[���h��̍ő�l
	m_worldMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���[���h��̍ŏ��l
	m_nMoveTime = 0;		//�ړ��܂ł̎���
	m_fLife = 0.0f;			//�̗�
	m_fRemLife = 0.0f;		//�c��̗�(%)
	m_fMaxLife = 0.0f;		//�ő�̗�
	m_fGravity = 0.0f;		//�d�͂̒l
	m_bNockBack = false;	//�m�b�N�o�b�N������
	m_pHP = nullptr;		//HP

	/* �� ���f����� �� */
	for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	{
		m_pModel[i] = nullptr;
	}

	for (int i = 0; i < nMaxLine; i++)
	{
		m_pLine[i] = nullptr;
	}

	/* �� ���[�V������� �� */
	m_nCurrentKey = 0;
	m_nCntMotion = 0;
}

//========================
// �f�X�g���N�^
//========================
CEnemy::~CEnemy()
{

}

//========================
// ������
//========================
HRESULT CEnemy::Init(D3DXVECTOR3 pos)
{
	//�����l�̐ݒ�
	m_pos = pos;
	m_nCntMotion = 1;
	m_fLife = 300.0f;			//�̗�
	m_fRemLife = 100.0f;		//�c��̗�(%)
	m_fMaxLife = m_fLife;		//�ő�̗�
	m_fGravity = fDefGravity;	//�d�͂̒l

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//--------------------
	// ���f���̐���
	//--------------------
	SetModel();

	//-----------------------
	// ���f���̑傫����ݒ�
	//-----------------------
	m_vtxMin = D3DXVECTOR3(-40.0f, 0.0f, -40.0f);
	m_vtxMax = D3DXVECTOR3(40.0f, 130.0f, 40.0f);

	m_size.x = m_vtxMax.x - m_vtxMin.x;
	m_size.y = m_vtxMax.y - m_vtxMin.y;
	m_size.z = m_vtxMax.z - m_vtxMin.z;

	//-----------------------
	// ���̕\��
	//-----------------------
	//SetLine();

	//-----------------------
	// HP�̐���
	//-----------------------
	{
		D3DXVECTOR3 hpPos(980.0f, 50.0f, 0.0f);
		m_pHP = CHP::Create(hpPos, 500.0f, 30.0f, CHP::HPTYPE_ENEMY);
		m_pHP->SetLife(m_fLife, m_fRemLife);	//HP�̐ݒ�
	}

	return S_OK;
}

//========================
// �I��
//========================
void CEnemy::Uninit()
{
	//-------------------------
	// ���f���̏I��
	//-------------------------
	for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	{//���f���p�[�c������
		if (m_pModel[i])
		{//���f����null����Ȃ��Ȃ�
			m_pModel[i]->Uninit();
			delete m_pModel[i];
			m_pModel[i] = nullptr;
		}
	}

	CObject::Release();
}

//========================
// �X�V
//========================
void CEnemy::Update()
{
	//-------------------------
	// ���f���̍X�V
	//-------------------------
	for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	{//���f���p�[�c������
		if (m_pModel[i])
		{//���f����null����Ȃ��Ȃ�
			m_pModel[i]->Update();
		}
	}

	//------------------------
	// �U������
	//------------------------
	Attack();

	//-------------------
	// ���[�V����
	//-------------------
	SetMotion(true);
	
	//-------------------------
	// ���̍X�V
	//-------------------------
	//UpdateLine();

	//----------------------------
	// �U�����󂯂�����
	//----------------------------
	//�v���C���[�Ƃ̓����蔻��
	CGame::GetPlayer()->GetCollisionPlayer(m_pos, m_size, m_mtxWorld);

	if (CGame::GetPlayer()->GetHitAttack())
	{//�v���C���[���U���𓖂Ă���ԂȂ�
		//���Z�b�g
		m_nAttackTime = 0;			//�U���܂ł̎���
		m_fGravity = fDefGravity;	//�d�͂̒l

		//--------------------------------
		// �m�b�N�o�b�N���鏈��
		//--------------------------------
		//�v���C���[�̈ʒu���擾
		D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPosition();

		//�G�ƃv���C���[�Ԃ̃x�N�g�����v�Z
		D3DXVECTOR3 vec = playerPos - m_pos;

		D3DXVec3Normalize(&vec,&vec);	//�x�N�g���̐��K��

		if (!m_bNockBack)
		{//�m�b�N�o�b�N���Ă��Ȃ��Ȃ�
			m_pos += -vec * 7.0f;	//�t�x�N�g�������Ɉړ�
			m_pos.y += 50.0f;		//�㏸
			m_bNockBack = true;
		}
	}
	else
	{
		m_bNockBack = false;
	}

	//---------------------
	// �d�͂̉��Z
	//---------------------
	if (m_pos.y >= 0.0f)
	{//���ł���Ȃ�
		m_pos.y -= m_fGravity;
		m_fGravity += 0.2f;
	}
	else
	{//�n�ʂɒ�������
		//�d�͂̒l�����Z�b�g
		m_fGravity = fDefGravity;
	}

	//----------------------------
	// ���S������
	//----------------------------
	if (m_fLife <= 0)
	{//�̗͂��s������
		CGame::SetEnemyState();	//�G�����񂾏��

		//HP�o�[�̏���
		if (m_pHP != nullptr)
		{//null����Ȃ��Ȃ�
			m_pHP->Uninit();
		}

		//����
		Uninit();
	}
}

//========================
// �`��
//========================
void CEnemy::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//�f�o�C�X�̎擾

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���f���̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//���f���̌����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//���f���̈ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//-------------------------
	// ���f���̕`��
	//-------------------------
	for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	{//���f���p�[�c������
		if (!m_pModel[i])
		{//���f����null�Ȃ�
			return;
		}

		m_pModel[i]->Draw(&m_mtxWorld);
	}
}

//========================
// ����
//========================
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy *pEnemy = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pEnemy = new CEnemy;	//����

	if (pEnemy != nullptr)
	{//NULL�`�F�b�N
		//�ϐ��ɑ��
		pEnemy->m_rot = rot;

		//������
		pEnemy->Init(pos);
		pEnemy->SetObjType(OBJTYPE_ENEMY);
	}

	return pEnemy;
}

//========================
// ���f���̐���
//========================
void CEnemy::SetModel()
{
	//���f��0
	m_pModel[0] = CModel::Create("data\\MODEL\\enemy.x", nullptr,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//========================
// ���[�V�����̐ݒ�
//========================
void CEnemy::SetMotion(bool bLoop)
{
	if (m_nCurrentKey + 1 >= MAX_ENEMY_KEY)
	{//�L�[���ő吔�ɒB������
		if (bLoop)
		{
			m_nCurrentKey = 0;	//�L�[�ԍ��̏�����
		}
		else
		{
			return;
		}
	}

	for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	{//���f���p�[�c������
	 //-----------------------------------------
	 // NULL�`�F�b�N
	 //-----------------------------------------
		if (!m_pModel[i])
		{//���f���p�[�c��null�Ȃ�
			return;
		}

		//�L�[�����������ϐ�
		KEY key = g_aKeySet[m_nCurrentKey].aKey[i];
		KEY keyNext = g_aKeySet[m_nCurrentKey + 1].aKey[i];

		//-----------------------------------------
		// ���ݒl���擾
		//-----------------------------------------
		//�ʒu���擾
		float fPosX = m_pModel[i]->GetPos().x;
		float fPosY = m_pModel[i]->GetPos().y;
		float fPosZ = m_pModel[i]->GetPos().z;

		//�������擾
		float fRotX = m_pModel[i]->GetRot().x;
		float fRotY = m_pModel[i]->GetRot().y;
		float fRotZ = m_pModel[i]->GetRot().z;

		//-----------------------------------------
		// �����̌v�Z
		// (�I���l - �J�n�l)
		//-----------------------------------------
		//�ʒu
		float fDifPosX = keyNext.fPosX - key.fPosX;
		float fDifPosY = keyNext.fPosY - key.fPosY;
		float fDifPosZ = keyNext.fPosZ - key.fPosZ;

		//����
		float fDifRotX = keyNext.fRotX - key.fRotX;
		float fDifRotY = keyNext.fRotY - key.fRotY;
		float fDifRotZ = keyNext.fRotZ - key.fRotZ;

		//-----------------------------------------
		// ���Βl�̌v�Z
		// (���[�V�����J�E���^�[ / �t���[����)
		//-----------------------------------------
		float fNumRelative = m_nCntMotion / (float)g_aKeySet[m_nCurrentKey].nFrame;

		//-----------------------------------------
		// ���ݒl�̌v�Z
		// (�J�n�l + (���� * ���Βl))
		//-----------------------------------------
		//�ʒu
		fPosX += key.fPosX + (fDifPosX * fNumRelative);
		fPosY += key.fPosY + (fDifPosY * fNumRelative);
		fPosZ += key.fPosZ + (fDifPosZ * fNumRelative);

		//����
		fRotX = key.fRotX + (fDifRotX * fNumRelative);
		fRotY = key.fRotY + (fDifRotY * fNumRelative);
		fRotZ = key.fRotZ + (fDifRotZ * fNumRelative);

		//-----------------------------------------
		// ���f�����̐ݒ�
		//-----------------------------------------
		//�ʒu�̐ݒ�
		m_pModel[i]->SetPos(D3DXVECTOR3(fPosX, fPosY, fPosZ));

		//�����̐ݒ�
		m_pModel[i]->SetRot(D3DXVECTOR3(D3DXToRadian(fRotX), D3DXToRadian(fRotY), D3DXToRadian(fRotZ)));
	}

	//���[�V�����J�E���^�[��i�߂�
	m_nCntMotion++;

	//-------------------------
	// ������
	//-------------------------
	if (m_nCntMotion >= g_aKeySet[m_nCurrentKey].nFrame)
	{//���[�V�����J�E���^�[���Đ��t���[���ɒB������
		m_nCurrentKey++;	//�L�[�ԍ������Z
		m_nCntMotion = 0;	//���[�V�����J�E���^�[��������
	}
}

//========================
// ���̐ݒu
//========================
void CEnemy::SetLine()
{
	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�ϊ��s����g����Min,Max�����߂�
	D3DXVec3TransformCoord(&m_worldMin, &m_vtxMin, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldMax, &m_vtxMax, &m_mtxWorld);

	//�������l���܂Ƃ߂�
	D3DXVECTOR3 min = m_worldMin;
	D3DXVECTOR3 max = m_worldMax;

	for (int i = 0; i < nMaxLine; i++)
	{
		m_pLine[i] = CLine::CreateAll(m_pLine[i], i, m_pos, min, max);
	}
}

//========================
// ���̏��̍X�V
//========================
void CEnemy::UpdateLine()
{
	//���[���h�ϊ��s����g����Min,Max�����߂�
	D3DXVec3TransformCoord(&m_worldMin, &m_vtxMin, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldMax, &m_vtxMax, &m_mtxWorld);

	//�������l���܂Ƃ߂�
	D3DXVECTOR3 min = m_worldMin;
	D3DXVECTOR3 max = m_worldMax;

	for (int i = 0; i < nMaxLine; i++)
	{
		if (m_pLine[i])
		{
			m_pLine[i]->SetLinePos(i, min, max);
		}
	}
}

//========================
// HP�������̏���
//========================
void CEnemy::SubLife(float fDamage)
{
	m_fLife -= fDamage;	//�̗͂̌���

	//�c��̗͂��v�Z
	m_fRemLife = m_fLife * 100 / m_fMaxLife;

	//HP�̐ݒ�
	m_pHP->SetLife(m_fLife, m_fRemLife);
}

//========================
// �U������
//========================
void CEnemy::Attack()
{
	//�ϐ��錾
	int nMaxAttackTime = 120;				//�U������
	float fAttackArea = 120.0f;				//�G�̍U���͈�
	float fMoveArea = fAttackArea - 50.0f;	//�G�̈ړ��͈�

	//�v���C���[�̈ʒu���擾
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPosition();

	//�G�ƃv���C���[�Ԃ̋������v�Z
	D3DXVECTOR3 vec = playerPos - m_pos;

	//�����̐�Βl���v�Z
	float fDistance = fabs((vec.x + vec.z) / 2);

	//-------------------------
	// �v���C���[�܂ňړ�
	//-------------------------
	if (fDistance >= fMoveArea)
	{//�v���C���[���͈͓��ɂ��Ȃ��Ȃ�
		Move();
	}

	//-------------------------
	// �U������
	//-------------------------
	if (fDistance <= fAttackArea)
	{//�v���C���[���͈͓��ɂ���Ȃ�
		//�U���܂ł̎��Ԃ����Z
		m_nAttackTime++;

		if (m_nAttackTime >= nMaxAttackTime)
		{//�U�����Ԃ��l�ɒB������
			CGame::GetPlayer()->SubLife(1);
		}
	}
}

//========================
// �ړ�����
//========================
void CEnemy::Move()
{
	//------------------------------
	// �v���C���[�̕�������
	//------------------------------
	//�v���C���[�̈ʒu���擾
	D3DXVECTOR3 playerPos(CGame::GetPlayer()->GetPosition());

	//2�_�Ԃ̋��������߂�
	float X = m_pos.x - playerPos.x;
	float Z = m_pos.z - playerPos.z;

	//�p�x�̐ݒ�
	float angle = atan2f(X, Z);

	//�����̐ݒ�
	m_rotDest = D3DXVECTOR3(0.0f, angle, 0.0f);

	//-------------------------------
	// �ړI�̊p�x�̐��K��
	//-------------------------------
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{//��]�������p�x��180�ȏ�Ȃ�
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{//��]�������p�x��-180�ȉ��Ȃ�
		m_rotDest.y += D3DX_PI * 2;
	}

	//-------------------------------
	// �ړI�̊p�x�܂ŉ�]����
	//-------------------------------
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.03f;	//��������

	//-------------------------------
	// �p�x�̐��K��
	//-------------------------------
	if (m_rot.y > D3DX_PI)
	{//�p�x��180�ȏ�Ȃ�
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{//�p�x��-180�ȉ��Ȃ�
		m_rot.y += D3DX_PI * 2;
	}

	//------------------------------
	// �v���C���[�Ɍ������Ĉړ�
	//------------------------------
	//�v���C���[�ƓG�̃x�N�g�������߂�
	D3DXVECTOR3 vec(playerPos - m_pos);

	//�x�N�g���̐��K��
	D3DXVec3Normalize(&vec, &vec);

	//�v���C���[�Ɍ������Ĉړ�
	m_move = vec * 1.5f;
	m_pos += m_move;
}