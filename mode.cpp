//===================================================
//
// ��ʑJ�ڂ̏���
// Author : Sato Teruto
//
//===================================================

//------------------------
// �C���N���[�h
//------------------------
#include "mode.h"
#include "game.h"
#include "title.h"
#include "tutorial.h"
#include "result.h"
#include "fade.h"

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
CTitle*			CMode::m_pTitle = nullptr;		//�^�C�g���N���X
CTutorial*		CMode::m_pTutorial = nullptr;	//�`���[�g���A��
CGame*			CMode::m_pGame = nullptr;		//�Q�[���N���X
CResult*		CMode::m_pResult = nullptr;		//���U���g�N���X
CFade*			CMode::m_pFade = nullptr;		//�t�F�[�h�N���X
CMode::MODE	CMode::m_mode = MODE_MAX;	//�Q�[�����[�h

//===========================
// �R���X�g���N�^
//===========================
CMode::CMode()
{
}

//===========================
// �f�X�g���N�^
//===========================
CMode::~CMode()
{
}

//===========================
// ������
//===========================
HRESULT CMode::Init()
{
	m_pFade = new CFade;
	SetMode(MODE_TITLE);
	m_pFade->Init(MODE_TITLE);

	return S_OK;
}

//===========================
// �I��
//===========================
void CMode::Uninit()
{
	//----------------------------
	// �^�C�g���̏I��
	//----------------------------
	if (m_pTitle != nullptr)
	{
		m_pTitle->Uninit();
		delete m_pTitle;
		m_pTitle = nullptr;
	}

	//----------------------------
	// �`���[�g���A���̏I��
	//----------------------------
	if (m_pTutorial != nullptr)
	{
		m_pTutorial->Uninit();
		delete m_pTutorial;
		m_pTutorial = nullptr;
	}

	//----------------------------
	// �Q�[���̏I��
	//----------------------------
	if (m_pGame != nullptr)
	{
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = nullptr;
	}

	//----------------------------
	// ���U���g�̏I��
	//----------------------------
	if (m_pResult != nullptr)
	{
		m_pResult->Uninit();
		delete m_pResult;
		m_pResult = nullptr;
	}

	//----------------------------
	// �t�F�[�h�̏I��
	//----------------------------
	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}
}

//===========================
// �X�V
//===========================
void CMode::Update()
{
	//���[�h���Ƃ̍X�V
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Update();
		break;

	case MODE_TUTORIAL:
		m_pTutorial->Update();
		break;

	case MODE_GAME:
		m_pGame->Update();
		break;

	case MODE_RESULT:
		m_pResult->Update();
		break;

	default:
		break;
	}

	//�t�F�[�h�̍X�V
	m_pFade->Update();
}

//===========================
// ���[�h�̐ݒ�
//===========================
void CMode::SetMode(MODE mode)
{
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Uninit();
		delete m_pTitle;
		m_pTitle = nullptr;
		break;

	case MODE_TUTORIAL:
		m_pTutorial->Uninit();
		delete m_pTutorial;
		m_pTutorial = nullptr;
		break;

	case MODE_GAME:
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = nullptr;
		break;

	case MODE_RESULT:
		m_pResult->Uninit();
		delete m_pResult;
		m_pResult = nullptr;
		break;

	default:
		break;
	}

	//�I�u�W�F�N�g�̑S���
	CObject::ReleaseAll(true);

	//���[�h�̐؂�ւ�
	m_mode = mode;

	//�V�������[�h�̐���
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle = nullptr;
		m_pTitle = new CTitle;
		m_pTitle->Init();
		break;

	case MODE_TUTORIAL:
		m_pTutorial = nullptr;
		m_pTutorial = new CTutorial;
		m_pTutorial->Init();
		break;

	case MODE_GAME:
		m_pGame = nullptr;
		m_pGame = new CGame;
		m_pGame->Init();
		break;

	case MODE_RESULT:
		m_pResult = nullptr;
		m_pResult = new CResult;
		m_pResult->Init();
		break;

	default:
		break;
	}
}