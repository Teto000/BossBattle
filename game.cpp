//===================================================
//
// �Q�[���̏���
// Author : Sato Teruto
//
//===================================================

//------------------------
// �C���N���[�h
//------------------------
#include <time.h>
#include "game.h"
#include "application.h"
#include "renderer.h"
#include "input.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "debug_proc.h"
#include "sound.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "meshfield.h"
#include "fade.h"

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
CCamera*	CGame::m_pCamera = nullptr;		//�J����
CPlayer*	CGame::m_pPlayer = nullptr;		//�v���C���[
CEnemy*		CGame::m_pEnemy = nullptr;		//�G�l�~�[
CMeshField*	CGame::m_pMeshField = nullptr;	//���b�V���t�B�[���h

//===========================
// �R���X�g���N�^
//===========================
CGame::CGame()
{
	
}

//===========================
// �f�X�g���N�^
//===========================
CGame::~CGame()
{

}

//===========================
// ������
//===========================
HRESULT CGame::Init()
{
	//�J�����̐���
	m_pCamera = new CCamera;
	m_pCamera->Init();

	//�v���C���[�̐���
	m_pPlayer = CPlayer::Create();

	//�G�l�~�[�̐���
	{
		D3DXVECTOR3 pos(0.0f, 0.0f, 300.0f);
		D3DXVECTOR3 rot(0.0f, 0.0f, 0.0f);
		m_pEnemy = CEnemy::Create(pos, rot);
	}

	//���b�V���t�B�[���h�̐���
	m_pMeshField = CMeshField::Create();

	return S_OK;
}

//===========================
// �I��
//===========================
void CGame::Uninit()
{
	//�J�����̏I��
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}
}

//===========================
// �X�V
//===========================
void CGame::Update()
{
	//�J�����̍X�V
	if (m_pCamera != nullptr)
	{
		m_pCamera->Update();
	}

	if (CInputKeyboard::Trigger(DIK_RETURN))
	{
		CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
	}
}