//===================================================
//
// �R���{�\������
// Author : Sato Teruto
//
//===================================================

//----------------------
// �C���N���[�h
//----------------------
#include "combo.h"
#include "number.h"

//=======================
// �R���X�g���N�^
//=======================
CCombo::CCombo() : CObject(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
	m_nNumCombo = 0;						//�R���{��

	for (int i = 0; i < nMaxDigits; i++)
	{
		m_aPosTexU[i] = 0;		//���̌��̐��l
		m_pNumber[i] = nullptr;	//���l
	}
}

//=======================
// �f�X�g���N�^
//=======================
CCombo::~CCombo()
{

}

//=======================
// ������
//=======================
HRESULT CCombo::Init(D3DXVECTOR3 pos)
{
	//�����l�̐ݒ�
	m_pos = pos;		//�ʒu

	//------------------------------
	// ���l�̐ݒ�
	//------------------------------
	for (int i = 0; i < nMaxDigits; i++)
	{
		D3DXVECTOR3 numberPos = D3DXVECTOR3(m_pos.x + (50.0f * i), m_pos.y, m_pos.z);
		m_pNumber[i] = CNumber::Create(numberPos, m_nNumCombo);
		m_pNumber[i]->Set(i);
	}

	SetNumber();

	return S_OK;
}

//=======================
// �I��
//=======================
void CCombo::Uninit()
{
	for (int i = 0; i < nMaxDigits; i++)
	{
		m_pNumber[i]->Uninit();
	}
}

//=======================
// �X�V
//=======================
void CCombo::Update()
{
	for (int i = 0; i < nMaxDigits; i++)
	{
		m_pNumber[i]->Update();
	}
}

//=======================
// �`��
//=======================
void CCombo::Draw()
{
	for (int i = 0; i < nMaxDigits; i++)
	{
		m_pNumber[i]->Draw();
	}
}

//=======================
// ����
// �����F�ʒu�A�R���{��
//=======================
CCombo *CCombo::Create(D3DXVECTOR3 pos, int nNum)
{
	CCombo *pCombo = nullptr;

	//----------------------------------
	// �����Ə�����
	//----------------------------------
	pCombo = new CCombo;	//����

	if (pCombo != nullptr)
	{//NULL�`�F�b�N
		//�����o�ϐ��ɑ��
		pCombo->m_nNumCombo = nNum;

		//������
		pCombo->Init(D3DXVECTOR3(pos));
	}

	return pCombo;
}

//=======================
// ���l�̐ݒ�
//=======================
void CCombo::SetNumber()
{
	for (int i = 0; i < nMaxDigits; i++)
	{//��������
		if (m_pNumber[i] != nullptr)
		{//null����Ȃ��Ȃ�
		 //pow�Ō������o���B
			int nCntNumber = nMaxDigits - i - 1;
			int nNum0 = (int)pow(10, nCntNumber + 1);
			int nNum1 = (int)pow(10, nCntNumber);

			//�����Ƃ̒l�����߂�
			m_aPosTexU[i] = (m_nNumCombo % nNum0) / nNum1;
			m_pNumber[i]->Set(m_aPosTexU[i]);
		}
	}
}

//=======================
// ���l�̉��Z
//=======================
int CCombo::AddNumber(int nNum)
{
	m_nNumCombo += nNum;

	//���l�̐ݒ�
	SetNumber();

	return m_nNumCombo;
}

//=======================
// �R���{�����Z�b�g
//=======================
void CCombo::ResetCombo()
{
	m_nNumCombo = 0;

	//���l�̐ݒ�
	SetNumber();
}