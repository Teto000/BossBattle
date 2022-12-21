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
CGauge::CGauge() : CObject2D(0)
{
}
CGauge::CGauge(int nPriority) : CObject2D(nPriority)
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

	CObject2D::Init(pos);
	CObject2D::SetSize(m_fWidth, m_fHeight);

	//���_���W�̐ݒ�
	CObject2D::SetVtxCIE_Gauge(m_pos, -m_fWidth / 2,
		-m_fWidth / 2 + (m_fLength * 100), -m_fHeight / 2, m_fHeight / 2);

	CObject2D::SetTexture(CTexture::TEXTURE_NONE);	//�e�N�X�`���̐ݒ�

	return S_OK;
}

//==========================
// �I��
//==========================
void CGauge::Uninit()
{
	CObject2D::Uninit();
}

//==========================
// �X�V
//==========================
void CGauge::Update()
{
	CObject2D::Update();

	//HP�������̏���
	SubHP();
}

//==========================
// �`��
//==========================
void CGauge::Draw()
{
	CObject2D::Draw();
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
// HP�̐ݒ�
//===========================
void CGauge::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
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
		CObject2D::SetVtxCIE_Gauge(m_pos, -m_fWidth / 2,
			-m_fWidth / 2 + (m_fLength * m_fRemLife), -m_fHeight / 2, m_fHeight / 2);
	}
	else if (m_fRemLife == 0 && m_fLife > 0)
	{//�c��̗͂�0% ���� �̗͂�0����Ȃ��Ȃ�
		//1%���̃Q�[�W���ێ�
		CObject2D::SetVtxCIE_Gauge(m_pos, -m_fWidth / 2,
			-m_fWidth / 2 + m_fLength, -m_fHeight / 2, m_fHeight / 2);
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
		CObject2D::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else if (m_fRemLife <= 50)
	{//HP��50%�ȉ��ɂȂ�����
		//���F�ɂ���
		CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
		//�ΐF�ɂ���
		CObject2D::SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}
}