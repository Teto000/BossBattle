//===================================================
//
// �_���[�W������
// Author : Sato Teruto
//
//===================================================

//----------------------
// �C���N���[�h
//----------------------
#include "damage.h"
#include "number.h"
#include "application.h"
#include "renderer.h"

//=======================
// �R���X�g���N�^
//=======================
CDamage::CDamage() : CBillBoard(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
	m_nAtkValue = 0;						//�R���{��
	m_nDeleteTime = 0;						//������܂ł̎���

	for (int i = 0; i < nMaxDigits; i++)
	{
		m_aPosTexU[i] = 0;		//���̌��̐��l
	}
}

//=======================
// �f�X�g���N�^
//=======================
CDamage::~CDamage()
{

}

//=======================
// ������
//=======================
HRESULT CDamage::Init(D3DXVECTOR3 pos)
{
	//�����l�̐ݒ�
	m_pos = pos;		//�ʒu
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�F
	m_fWidth = 60.0f;	//��
	m_fHeight = 80.0f;	//����

	//------------------------------
	// ���l�̐ݒ�
	//------------------------------
	//�I�u�W�F�N�g�̏�����
	CBillBoard::Init(pos);

	//�傫���̐ݒ�
	CBillBoard::SetSize(m_fWidth, m_fHeight);

	//�e�N�X�`���̐ݒ�
	CBillBoard::SetTexture(CTexture::TEXTURE_NUMBER);

	//���l�̐ݒ�
	SetNumber();

	return S_OK;
}

//=======================
// �I��
//=======================
void CDamage::Uninit()
{
	CBillBoard::Uninit();
}

//=======================
// �X�V
//=======================
void CDamage::Update()
{
	CBillBoard::Update();

	//������܂ł̎��Ԃ𐔂���
	m_nDeleteTime++;

	if (m_nDeleteTime >= 60)
	{//��莞�Ԃ��o�߂�����
		//�����x�̌���
		m_col.a -= 0.1f;
	}

	if (m_col.a <= 0.0f)
	{//���S�ɓ����ɂȂ�����
		Uninit();
	}
}

//=======================
// �`��
//=======================
void CDamage::Draw()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//�A���t�@�e�X�g�̗L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 80);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Z�o�b�t�@�̃N���A
	pDevice->SetRenderState(D3DRS_ZENABLE, false);

	//�I�u�W�F�N�g�̕`��
	CBillBoard::Draw();

	// Z�o�b�t�@�̗L�����ݒ�
	pDevice->SetRenderState(D3DRS_ZENABLE, true);

	//�A���t�@�e�X�g�̖�����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=======================
// ����
// �����F�ʒu�A�R���{��
//=======================
CDamage *CDamage::Create(D3DXVECTOR3 pos, int nNum)
{
	CDamage *pDamage = nullptr;

	//----------------------------------
	// �����Ə�����
	//----------------------------------
	pDamage = new CDamage;	//����

	if (pDamage != nullptr)
	{//NULL�`�F�b�N
		//�����o�ϐ��ɑ��
		pDamage->m_nAtkValue = nNum;

		//������
		pDamage->Init(D3DXVECTOR3(pos));
	}

	return pDamage;
}

//=======================
// ���l�̐ݒ�
//=======================
void CDamage::SetNumber()
{
	for (int i = 0; i < nMaxDigits; i++)
	{//��������
		//pow�Ō������o���B
		int nCntNumber = nMaxDigits - i - 1;
		int nNum0 = (int)pow(10, nCntNumber + 1);
		int nNum1 = (int)pow(10, nCntNumber);

		//�����Ƃ̒l�����߂�
		m_aPosTexU[i] = (m_nAtkValue % nNum0) / nNum1;

		CBillBoard::SetAnimation((float)m_aPosTexU[i], 10);
	}
}