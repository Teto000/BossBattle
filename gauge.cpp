//===================================================
//
// �Q�[�W�̏���
// Author : Sato Teruto
//
//===================================================

//-----------------------
// �C���N���[�h
//-----------------------
#include "gauge.h"

//==========================
// �R���X�g���N�^
//==========================
CGauge::CGauge() : CBillBoard(0)
{
}
CGauge::CGauge(int nPriority) : CBillBoard(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
	m_fLife = 0;		//�̗͎󂯎��p
	m_fRemLife = 0;		//�c��̗͎󂯎��p
	m_fWidth = 0.0f;	//��
	m_fHeight = 0.0f;	//����
	m_fLength = 0.0f;	//HP�o�[�̒���
}

//==========================
// �f�X�g���N�^
//==========================
CGauge::~CGauge()
{

}

//==========================
// ������
//==========================
HRESULT CGauge::Init(D3DXVECTOR3 pos)
{
	//�����l�̐ݒ�
	m_pos = pos;	//�ʒu
	m_fLength = (m_fWidth / 100);	//��

	CBillBoard::Init(pos);
	CBillBoard::SetSize(m_fWidth, m_fHeight);

	//���_���W�̐ݒ�
	CBillBoard::SetVtxCIE_Gauge(m_pos, m_fWidth / 2,
		-m_fWidth / 2 + (m_fLength * 100), m_fHeight / 2, m_fHeight / 2);

	CBillBoard::SetTexture(CTexture::TEXTURE_NONE);	//�e�N�X�`���̐ݒ�

	return S_OK;
}

//==========================
// �I��
//==========================
void CGauge::Uninit()
{
	CBillBoard::Uninit();
}

//==========================
// �X�V
//==========================
void CGauge::Update()
{
	CBillBoard::Update();

	//HP�������̏���
	SubHP();
}

//==========================
// �`��
//==========================
void CGauge::Draw()
{
	CBillBoard::Draw();
}

//===========================
// HP�̐ݒ�
//===========================
void CGauge::SetLife(float fLife, float fRemLife)
{
	m_fLife = fLife;
	m_fRemLife = fRemLife;
}

//===========================
// HP�������̏���
//===========================
void CGauge::SubHP()
{
	//-------------------------
	// ���_���W�̐ݒ�
	//-------------------------
	if (m_fRemLife >= 1)
	{//�c��̗�(%)������Ȃ�
		//���̗͕̑��̍��W��ݒ�
		CBillBoard::SetVtxCIE_Gauge(m_pos, -m_fWidth / 2,
			-m_fWidth / 2 + (m_fLength * m_fRemLife), m_fHeight / 2, -m_fHeight / 2);
	}
	else if (m_fRemLife == 0 && m_fLife > 0)
	{//�c��̗͂�0% ���� �̗͂�0����Ȃ��Ȃ�
		//1%���̃Q�[�W���ێ�
		CBillBoard::SetVtxCIE_Gauge(m_pos, -m_fWidth / 2,
			-m_fWidth / 2 + m_fLength, -m_fHeight / 2, -m_fHeight / 2);
	}

	//-------------------------
	// HP���Ƃ̏���
	//-------------------------
	if (m_fRemLife <= 0 && m_fLife <= 0)
	{//HP��0�ɂȂ��� ���� �̗͂��Ȃ�������
		//HP�o�[�̏���
		Uninit();
		return;
	}
	else if (m_fRemLife <= 20)
	{//HP��20%�ȉ��ɂȂ�����
		//�ԐF�ɂ���
		CBillBoard::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else if (m_fRemLife <= 50)
	{//HP��50%�ȉ��ɂȂ�����
		//���F�ɂ���
		CBillBoard::SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
		//�ΐF�ɂ���
		CBillBoard::SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}
}