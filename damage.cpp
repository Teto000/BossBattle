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

//=======================
// �R���X�g���N�^
//=======================
CDamage::CDamage() : CObject(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
	m_nAtkValue = 0;						//�R���{��

	for (int i = 0; i < nMaxDigits; i++)
	{
		m_aPosTexU[i] = 0;		//���̌��̐��l
		m_pNumber[i] = nullptr;	//���l
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

	//------------------------------
	// ���l�̐ݒ�
	//------------------------------
	for (int i = 0; i < nMaxDigits; i++)
	{
		D3DXVECTOR3 numberPos = D3DXVECTOR3(m_pos.x + (50.0f * i), m_pos.y, m_pos.z);
		m_pNumber[i] = CNumber::Create(numberPos, m_nAtkValue);
		m_pNumber[i]->Set(i);
	}

	SetNumber();

	return S_OK;
}

//=======================
// �I��
//=======================
void CDamage::Uninit()
{
	for (int i = 0; i < nMaxDigits; i++)
	{
		m_pNumber[i]->Uninit();
	}
}

//=======================
// �X�V
//=======================
void CDamage::Update()
{
	for (int i = 0; i < nMaxDigits; i++)
	{
		m_pNumber[i]->Update();
	}
}

//=======================
// �`��
//=======================
void CDamage::Draw()
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
		if (m_pNumber[i] != nullptr)
		{//null����Ȃ��Ȃ�
			//pow�Ō������o���B
			int nCntNumber = nMaxDigits - i - 1;
			int nNum0 = (int)pow(10, nCntNumber + 1);
			int nNum1 = (int)pow(10, nCntNumber);

			//�����Ƃ̒l�����߂�
			m_aPosTexU[i] = (m_nAtkValue % nNum0) / nNum1;
			m_pNumber[i]->Set(m_aPosTexU[i]);
		}
	}
}