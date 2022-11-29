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
		m_pObject2D->SetSize(m_fWidth, m_fHeight);

		//-------------------------
		// ��ނ��Ƃ̏���
		//-------------------------
		switch (m_type)
		{
		case BGTYPE_TITLE:
			m_pObject2D->SetTexture(CTexture::TEXTURE_TITLE);
			break;

		case BGTYPE_RESULT:
			m_pObject2D->SetTexture(CTexture::TEXTURE_RESULT);
			break;

		default:
			break;
		}
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

//========================
// �ʒu�̎擾
//========================
D3DXVECTOR3 CBg::GetPosition()
{
	return m_pos;
}

//========================
//	���̎擾
//========================
float CBg::GetWidth()
{
	return m_fWidth;
}

//========================
// �����擾
//========================
float CBg::GetHeight()
{
	return m_fHeight;
}