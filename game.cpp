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
#include "model.h"
#include "meshfield.h"
#include "fade.h"
#include "polygon.h"
#include "message.h"
#include "sky.h"
#include "hp.h"
#include "stage.h"
#include "item.h"
#include "time.h"

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
bool CGame::m_bDeathEnemny = false;			//�G������ł��邩
bool CGame::m_bFinish = false;				//�I���t���O

CCamera*	CGame::m_pCamera = nullptr;		//�J����
CPlayer*	CGame::m_pPlayer = nullptr;		//�v���C���[
CEnemy*		CGame::m_pEnemy = nullptr;		//�G�l�~�[
CMeshField*	CGame::m_pMeshField = nullptr;	//���b�V���t�B�[���h
CPolygon*	CGame::m_pPolygon = nullptr;	//2D�|���S��
CMessage*	CGame::m_pMessage = nullptr;	//���b�Z�[�W
CSky*		CGame::m_pSky = nullptr;		//�X�J�C
CStage*		CGame::m_pStage = nullptr;		//�X�e�[�W
CTime*		CGame::m_pTime = nullptr;		//�^�C�}�[

//===========================
// �R���X�g���N�^
//===========================
CGame::CGame()
{
	nCntFinish = 0;
	nCntItem = 0;
	m_pItem = nullptr;
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
	//�����l�̐ݒ�
	m_bDeathEnemny = false;
	m_bFinish = false;

	//�J�����̐���
	m_pCamera = new CCamera;
	m_pCamera->Init();

	//�v���C���[�̐���
	m_pPlayer = CPlayer::Create();

	//----------------------------
	// �G�l�~�[�̐���
	//----------------------------
	{
		D3DXVECTOR3 pos(0.0f, 0.0f, 300.0f);
		D3DXVECTOR3 rot(0.0f, 0.0f, 0.0f);
		m_pEnemy = CEnemy::Create(pos, rot);
	}

	//���b�V���t�B�[���h�̐���
	m_pMeshField = CMeshField::Create();

	//�X�e�[�W�̐���
	m_pStage = CStage::Create("data\\MODEL\\Stage.x", D3DXVECTOR3(0.0f, -100.0f, 0.0f));

	//��̐���
	m_pSky = CSky::Create(CTexture::TEXTURE_SKY);

	//2D�|���S���̐���
	m_pPolygon = CPolygon::Create(CPolygon::POLYGONTYPE_AIM);

	//���b�Z�[�W�̐���
	{
		D3DXVECTOR3 pos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		m_pMessage = CMessage::Create(pos, 800.0f, 400.0f, CMessage::MESSAGE_CLEAR);
		m_pMessage = CMessage::Create(pos, 600.0f, 200.0f, CMessage::MESSAGE_BREAK);
	}

	//�^�C�}�[�̐���
	m_pTime = CTime::Create(D3DXVECTOR3(1100.0f, 650.0f, 0.0f));

	//BGM�̍Đ�
	CSound::PlaySound(CSound::SOUND_LABEL_GAME);

	return S_OK;
}

//===========================
// �I��
//===========================
void CGame::Uninit()
{
	//BGM�̒�~
	CSound::StopSound();

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
	if (m_bDeathEnemny)
	{//�G������ł���Ȃ�
		m_pEnemy = nullptr;
		m_bFinish = true;	//�I���t���O�𗧂Ă�
	}

	//----------------------------
	// �I������
	//----------------------------
	if (m_bFinish)
	{//�I���t���O�������Ă���Ȃ�
		nCntFinish++;	//�J�E���g�����Z

		if (nCntFinish >= 300)
		{//�J�E���g��5�b�ȏ�Ȃ�
			//���U���g��ʂɈڍs
			CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
		}
		else if (nCntFinish >= 120)
		{//�J�E���g��2�b�ȏ�(�N���A�̕�������������)
			if (CInputKeyboard::Trigger(DIK_RETURN))
			{//ENTER�L�[�������ꂽ��
				//���U���g��ʂɈڍs
				CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
			}
		}
	}

	//----------------------------
	// �J�����̍X�V
	//----------------------------
	if (m_pCamera != nullptr)
	{
		m_pCamera->Update();
	}

	//----------------------------
	// �A�C�e���̐���
	//----------------------------
	nCntItem++;	//�J�E���g�̉��Z

	if(nCntItem >= 60)
	{
		int nWidth = 300;

		int X = rand() % nWidth - (nWidth / 2);
		int Z = rand() % nWidth - (nWidth / 2);

		D3DXVECTOR3 pos((float)X, 100.0f, (float)Z);
		m_pItem = CItem::Create(pos, CItem::ITEMTYPE_HEAL);
		nCntItem = 0;	//���Z�b�g
	}
}