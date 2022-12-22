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

//========================
// �R���X�g���N�^
//========================
CStyleShift::CStyleShift() : CBillBoard(0)
{
	
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
	m_fWidth = 100.0f;	//��
	m_fHeight = 100.0f;	//����
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	//�F

	CBillBoard::Init(pos);

	//�傫���̐ݒ�
	CBillBoard::SetSize(m_fWidth, m_fHeight);

	//�F�̐ݒ�
	CBillBoard::SetColor(m_col);

	//�e�N�X�`���̐ݒ�
	CBillBoard::SetTexture(CTexture::TEXTURE_NONE);

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
CStyleShift* CStyleShift::Create(D3DXVECTOR3 pos)
{
	CStyleShift *pStyleShift = nullptr;

	//----------------------------------
	// �����Ə�����
	//----------------------------------
	pStyleShift = new CStyleShift;	//����

	if (pStyleShift != nullptr)
	{//NULL�`�F�b�N
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
		CBillBoard::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		break;

	case false:
		CBillBoard::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		break;
	}
}