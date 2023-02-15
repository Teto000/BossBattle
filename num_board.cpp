//===================================================
//
// ���l���r���{�[�h�ŕ\�����鏈��
// Author : Sato Teruto
//
//===================================================

//----------------------
// �C���N���[�h
//----------------------
#include "num_board.h"
#include "application.h"
#include "renderer.h"

//=======================
// �R���X�g���N�^
//=======================
CNumBoard::CNumBoard() : CBillBoard(0)
{
	m_nValue = 0;	//���l
	m_nDigit = 0;	//����
}

//=======================
// �f�X�g���N�^
//=======================
CNumBoard::~CNumBoard()
{

}

//=======================
// ������
//=======================
HRESULT CNumBoard::Init(D3DXVECTOR3 pos)
{
	//�I�u�W�F�N�g�̏�����
	CBillBoard::Init(pos);

	//�e�N�X�`���̐ݒ�
	CBillBoard::SetTexture(CTexture::TEXTURE_NUMBER);

	return S_OK;
}

//=======================
// �I��
//=======================
void CNumBoard::Uninit()
{
	//�I�u�W�F�N�g�̏I��
	CBillBoard::Uninit();
}

//=======================
// �X�V
//=======================
void CNumBoard::Update()
{
	//�I�u�W�F�N�g�̍X�V
	CBillBoard::Update();
}

//=======================
// �`��
//=======================
void CNumBoard::Draw()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//�A���t�@�e�X�g�̗L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 80);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Z�o�b�t�@�̃N���A
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	//�I�u�W�F�N�g�̕`��
	CBillBoard::Draw();

	// Z�o�b�t�@�̗L�����ݒ�
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	//�A���t�@�e�X�g�̖�����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=======================
// ����
// �����F�ʒu�A�l�A����
//=======================
CNumBoard *CNumBoard::Create(D3DXVECTOR3 pos, int nValue)
{
	CNumBoard *pNumber = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pNumber = new CNumBoard;	//����

	if (pNumber != nullptr)
	{//NULL�`�F�b�N
	 //�����o�ϐ��ɑ��
		pNumber->m_nValue = nValue;

		//������
		pNumber->Init(D3DXVECTOR3(pos));
		pNumber->SetObjType(OBJTYPE_NUMBER);
	}

	return pNumber;
}

//=======================
// ���l�̐ݒ�
// �����F����
//=======================
void CNumBoard::Set(int nDigit)
{
	CBillBoard::SetAnimation((float)nDigit, 10);
}