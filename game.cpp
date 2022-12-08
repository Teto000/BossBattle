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
#include "polygon.h"

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
CCamera*	CGame::m_pCamera = nullptr;		//�J����
CPlayer*	CGame::m_pPlayer = nullptr;		//�v���C���[
CEnemy*		CGame::m_pEnemy = nullptr;		//�G�l�~�[
CMeshField*	CGame::m_pMeshField = nullptr;	//���b�V���t�B�[���h
CPolygon*	CGame::m_pPolygon = nullptr;	//2D�|���S��

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

	//2D�|���S���̐���
	m_pPolygon = CPolygon::Create(CPolygon::POLYGONTYPE_AIM);

	//BGM�̍Đ�
	CSound::PlaySound(CSound::SOUND_LABEL_GAME);

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

	//BGM�̒�~
	CSound::StopSound();
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

	//��ʑJ��
	if (CInputKeyboard::Trigger(DIK_RETURN))
	{
		//���U���g��ʂɈڍs
		CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);

		//SE�̍Đ�
		CSound::PlaySound(CSound::SOUND_LABEL_SE_BUTTOM);
	}
}