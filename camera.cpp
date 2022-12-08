//===================================================
//
// �J�����̏���
// Author:Teruto Sato
//
//===================================================

//----------------------
// �C���N���[�h
//----------------------
#include "camera.h"
#include "input.h"
#include "input_keyboard.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "enemy.h"

//===========================
// �R���X�g���N�^
//===========================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_worldPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_worldPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_TSPEED = 0.0f;
	m_bLockOn = false;
}

//===========================
// �f�X�g���N�^
//===========================
CCamera::~CCamera()
{

}

//========================
// �J�����̏���������
//========================
void CCamera::Init(void)
{
	//---------------------------------
	// ���_�E�����_�E�������ݒ肷��
	//---------------------------------
	m_posV = D3DXVECTOR3(0.0f, 200.0f, -400.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_worldPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_worldPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_TSPEED = 0.02f;		//�J�����̉�]���x(Turn)
}

//========================
// �J�����̏I������
//========================
void CCamera::Uninit(void)
{

}

//========================
// �J�����̍X�V����
//========================
void CCamera::Update(void)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//�f�o�C�X�̎擾

	//��]
	Turn();

	//���b�N�I����Ԃ̐؂�ւ�
	if (CInputKeyboard::Trigger(DIK_SPACE))
	{
		//���b�N�I����Ԃ�؂�ւ�
		m_bLockOn = !m_bLockOn;
	}

	//----------------------------------------
	// �J�������̃v���C���[�̈ʒu�ݒ�
	//----------------------------------------
	if (CInputKeyboard::Press(DIK_LEFT))
	{
		m_posV.x += 5.0f;
		m_posR.x += 5.0f;
	}
	else if (CInputKeyboard::Press(DIK_RIGHT))
	{
		m_posV.x -= 5.0f;
		m_posR.x -= 5.0f;
	}

	if (CInputKeyboard::Press(DIK_UP))
	{
		m_posV.z -= 5.0f;
		m_posR.z -= 5.0f;
	}
	else if (CInputKeyboard::Press(DIK_DOWN))
	{
		m_posV.z += 5.0f;
		m_posR.z += 5.0f;
	}

	//----------------------------------------
	// �s����g�����J��������
	//----------------------------------------
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//�v���C���[�̈ʒu���擾
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPosition();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�s��ɉ�]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�s��Ɉړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, playerPos.x, playerPos.y, playerPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���[���h�ϊ��s����g����posV,posR�����߂�
	D3DXVec3TransformCoord(&m_worldPosR, &m_posR, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldPosV, &m_posV, &m_mtxWorld);

	//�ړI�̍��W���X�V
	m_posVDest = m_worldPosV;
	m_posRDest = m_worldPosR;

	//----------------------------------------
	// ���b�N�I������
	//----------------------------------------
	//���b�N�I���ʒu
	D3DXVECTOR3 lockOnPos;
	if (CGame::GetEnemy())
	{//�G��null����Ȃ��Ȃ�
		lockOnPos = CGame::GetEnemy()->GetPosition();
	}

	if (m_bLockOn)
	{//���b�N�I����ԂȂ�
		//�����_�����b�N�I���ʒu�ɐݒ�
		m_worldPosR = lockOnPos;

		//�v���C���[�ʒu�ƃ��b�N�I���ʒu�Ԃ̃x�N�g�������߂�
		D3DXVECTOR3 vec = lockOnPos - playerPos;

		//���K��
		D3DXVec3Normalize(&vec, &vec);

		//���_�̈ʒu��ݒ�
		m_worldPosV = playerPos + (-vec) * 500.0f;
		m_worldPosV.y = 200.0f;

		//�J�����̊p�x�����b�N�I����ɍ��킹��
		m_rot.y = atan2f(m_worldPosR.x - m_worldPosV.x, m_worldPosR.z - m_worldPosV.z);
	}

	//----------------------------------------
	// �p�x�̐��K��
	//----------------------------------------
	//���E
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
}

//========================
// �J�����̐ݒ菈��
//========================
void CCamera::SetCamera(LPDIRECT3DDEVICE9 pDevice)
{
	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
					   &m_worldPosV,
					   &m_worldPosR,
					   &m_vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
							   D3DXToRadian(45.0f),
							   (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
							   10.0f,
							   100000.0f);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//========================
// �J�����̐���
//========================
void CCamera::Turn()
{
	//------------------
	// �����_�̐���
	//------------------
	if (CInputKeyboard::Press(DIK_Q))	//����]
	{//Q�L�[�������ꂽ
		m_rot.y -= m_TSPEED;	//��]�ʂ̑���
	}
	else if (CInputKeyboard::Press(DIK_E))	//�E��]
	{//E�L�[�������ꂽ
		m_rot.y += m_TSPEED;
	}
	if (CInputKeyboard::Press(DIK_Y))	//���]
	{//Y�L�[�������ꂽ
		m_rot.x += m_TSPEED;
	}
	else if (CInputKeyboard::Press(DIK_B))	//����]
	{//B�L�[�������ꂽ
		m_rot.x -= m_TSPEED;
	}
}

//========================
// ���_�̎擾
//========================
D3DXVECTOR3 CCamera::GetPosV()
{
	return m_posV;
}

//========================
// �p�x�̎擾
//========================
D3DXVECTOR3 CCamera::GetRot()
{
	return m_rot;
}