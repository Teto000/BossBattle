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
#include "input_keybord.h"
#include "input_joypad.h"
#include "debug_proc.h"
#include "sound.h"
#include "camera.h"
#include "player.h"
#include "meshfield.h"

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
CCamera*	CGame::m_pCamera = nullptr;		//�J����
CPlayer*	CGame::m_pPlayer = nullptr;		//�v���C���[
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
	if (m_pCamera != nullptr)
	{
		m_pCamera->Update();
	}
}