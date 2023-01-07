//===================================================
//
// �퓬�X�^�C���\������
// Author:Teruto Sato
//
//===================================================

//------------------------
// �C���N���[�h
//------------------------
#include "style_shift.h"
#include "texture.h"
#include "game.h"
#include "player.h"

//========================
// �R���X�g���N�^
//========================
CStyleShift::CStyleShift() : CBillBoard(0)
{
	m_stylePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_offSet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//========================
// �f�X�g���N�^
//========================
CStyleShift::~CStyleShift()
{

}

//========================
// ������
//========================
HRESULT CStyleShift::Init(D3DXVECTOR3 pos)
{
	//�����l�̐ݒ�
	m_fWidth = 70.0f;	//��
	m_fHeight = 70.0f;	//����
	m_stylePos = pos;
	m_offSet = m_pos;

	CBillBoard::Init(m_stylePos);

	//�傫���̐ݒ�
	CBillBoard::SetSize(m_fWidth, m_fHeight);

	//�e�N�X�`���̐ݒ�
	CBillBoard::SetTexture(CTexture::TEXTURE_NONE);

	switch (m_type)
	{
	case STYLE_TYPE_ATTACK:
		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);	//�F
		break;

	case STYLE_TYPE_SPEED:
		m_col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.0f);
		break;

	case STYLE_TYPE_COMBO:
		m_col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f);
		break;

	default:
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		break;
	}

	//�F�̐ݒ�
	CBillBoard::SetColor(m_col);

	return S_OK;
}

//========================
// �I��
//========================
void CStyleShift::Uninit()
{
	CBillBoard::Uninit();
}

//========================
// �X�V
//========================
void CStyleShift::Update()
{

	CBillBoard::Update();

	//-----------------------
	// ���X�ɑN���ɂ��鏈��
	//-----------------------
	if (m_bFlash && m_col.a <= 1.0f)
	{//���邭������ & �s��������Ȃ��Ȃ�
		m_col.a += 0.05f;
		CBillBoard::SetColor(m_col);
	}

	//-----------------------------
	// �v���C���[�̈ʒu���Q��
	//-----------------------------
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetWorldPos();
	D3DXVECTOR3 pos = playerPos;
	CBillBoard::SetPos(pos);
}

//========================
// �`��
//========================
void CStyleShift::Draw()
{
	CBillBoard::Draw();
}

//========================
// ����
//========================
CStyleShift* CStyleShift::Create(D3DXVECTOR3 pos, STYLE_TYPE type)
{
	CStyleShift *pStyleShift = nullptr;

	//----------------------------------
	// �����Ə�����
	//----------------------------------
	pStyleShift = new CStyleShift;	//����

	if (pStyleShift != nullptr)
	{//NULL�`�F�b�N
		//�����o�ϐ��ɑ��
		pStyleShift->m_type = type;	//���

		//������
		pStyleShift->Init(pos);
		pStyleShift->SetObjType(OBJTYPE_STYLESHIFT);
	}

	return pStyleShift;
}

//========================
// �X�^�C���̏�Ԃ̐ݒ�
//========================
void CStyleShift::SetStyle(bool flash)
{
	//��Ԃ�ݒ�
	m_bFlash = flash;

	//-------------------------------
	// ��Ԃɉ����������x�ɐݒ�
	//-------------------------------
	switch (m_bFlash)
	{
	case true:
		m_col.a = 0.5f;
		break;

	case false:
		m_col.a = 0.0f;
		break;
	}

	CBillBoard::SetColor(m_col);
}