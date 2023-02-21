//===================================================
//
// �w�i�̏���
// Author:Teruto Sato
//
//===================================================

//------------------------
// �C���N���[�h
//------------------------
#include "bg.h"
#include "renderer.h"
#include "texture.h"

//========================
// �R���X�g���N�^
//========================
CBg::CBg() : CObject(0)
{
	m_pObject2D = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
}

//========================
// �f�X�g���N�^
//========================
CBg::~CBg()
{

}

//========================
// ������
//========================
HRESULT CBg::Init(D3DXVECTOR3 pos)
{
	//�����l�̐ݒ�
	m_pos = pos;
	m_fWidth = SCREEN_WIDTH;
	m_fHeight = SCREEN_HEIGHT;

	if (m_pObject2D != nullptr)
	{
		m_pObject2D->Init(m_pos);

		//-------------------------
		// ��ނ��Ƃ̏���
		//-------------------------
		switch (m_type)
		{
		case BGTYPE_TITLE:
			m_pObject2D->SetTexture(CTexture::TEXTURE_TITLE);
			m_fWidth = 1000.0f;
			m_fHeight = 300.f;
			break;

		case BGTYPE_RESULT:
			m_pObject2D->SetTexture(CTexture::TEXTURE_RESULT);
			break;

		case BGTYPE_COMBO:
			m_fWidth = 200.0f;
			m_fHeight = 180.0f;
			m_pObject2D->SetTexture(CTexture::TEXTURE_BG_COMBO);
			break;

		case BGTYPE_TIMER:
			m_fWidth = 200.0f;
			m_fHeight = 180.0f;
			m_pObject2D->SetTexture(CTexture::TEXTURE_BG_TIMER);
			break;

		case BGTYPE_PLAYER_HP:
			m_fWidth = 350.0f;
			m_fHeight = 50.0f;
			m_pObject2D->SetTexture(CTexture::TEXTURE_BG_PLAYERHP);
			break;

		default:
			break;
		}

		m_pObject2D->SetSize(m_fWidth, m_fHeight);
	}

	return S_OK;
}

//========================
// �I��
//========================
void CBg::Uninit()
{
	if (m_pObject2D != nullptr)
	{
		m_pObject2D->Uninit();
	}
}

//========================
// �X�V
//========================
void CBg::Update()
{
	if (m_pObject2D != nullptr)
	{
		m_pObject2D->Update();
	}
}

//========================
// �`��
//========================
void CBg::Draw()
{
	if (m_pObject2D != nullptr)
	{
		m_pObject2D->Draw();
	}
}

//========================
// ����
//========================
CBg* CBg::Create(D3DXVECTOR3 pos, BGTYPE type)
{
	CBg *pBg = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pBg = new CBg;	//����
	pBg->m_pObject2D = new CObject2D(0);

	if (pBg != nullptr)
	{//NULL�`�F�b�N
		//�ϐ��ɑ��
		pBg->m_type = type;

		//������
		pBg->Init(pos);
	}

	return pBg;
}