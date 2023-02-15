//===================================================
//
// �^�C�}�[����
// Author : Sato Teruto
//
//===================================================

//----------------------
// �C���N���[�h
//----------------------
#include "time.h"
#include "number.h"
#include "game.h"
#include "renderer.h"

//=======================
// �R���X�g���N�^
//=======================
CTime::CTime() : CObject(1)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_numberPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�����̈ʒu
	m_nTime = 0;								//����
	m_nCntFream = 0;							//�t���[�����̃J�E���g
	fInterval = 0.0f;							//���l�̊Ԋu

	for (int i = 0; i < nMaxDigits; i++)
	{
		m_aPosTexU[i] = 0;		//���̌��̐��l
		m_pNumber[i] = nullptr;	//���l
	}
}

//=======================
// �f�X�g���N�^
//=======================
CTime::~CTime()
{

}

//=======================
// ������
//=======================
HRESULT CTime::Init(D3DXVECTOR3 pos)
{
	//�����l�̐ݒ�
	m_pos = pos;		//�ʒu
	fInterval = 50.0f;	//���l�̊Ԋu

	//------------------------------
	// ���l�̐ݒ�
	//------------------------------
	for (int i = 0; i < nMaxDigits; i++)
	{
		D3DXVECTOR3 numberPos = D3DXVECTOR3(m_pos.x + (fInterval * i), m_pos.y, m_pos.z);
		m_pNumber[i] = CNumber::Create(numberPos, m_nTime);
		m_pNumber[i]->Set(i);
	}

	SetNumber();

	return S_OK;
}

//=======================
// �I��
//=======================
void CTime::Uninit()
{
	for (int i = 0; i < nMaxDigits; i++)
	{
		m_pNumber[i]->Uninit();
	}
}

//=======================
// �X�V
//=======================
void CTime::Update()
{
	//----------------------
	// �Q�[���I�����Ɋg��
	//----------------------
	if (CGame::GetFinish())
	{
		//--------------------------------------
		// �ړI�̈ʒu�܂ňړ�����
		//--------------------------------------
		m_pos.x += ((SCREEN_WIDTH / 2 - 50.0f) - m_pos.x) * 0.08f;	//��������
		m_pos.y += ((SCREEN_HEIGHT / 2) - m_pos.y) * 0.08f;

		for (int i = 0; i < nMaxDigits; i++)
		{
			m_pNumber[i]->SetPosition(D3DXVECTOR3(m_pos.x + i * 50.0f, m_pos.y, m_pos.z));
		}
	}
	else
	{
		//�t���[�����𐔂���
		m_nCntFream++;

		//----------------------
		// ���Ԃ̉��Z
		//----------------------
		if (m_nCntFream >= 60)
		{
			m_nTime++;
			SetNumber();
			m_nCntFream = 0;
		}
	}
}

//=======================
// �`��
//=======================
void CTime::Draw()
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
CTime *CTime::Create(D3DXVECTOR3 pos)
{
	CTime *pCombo = nullptr;

	//----------------------------------
	// �����Ə�����
	//----------------------------------
	pCombo = new CTime;	//����

	if (pCombo != nullptr)
	{//NULL�`�F�b�N
		//������
		pCombo->Init(D3DXVECTOR3(pos));
	}

	return pCombo;
}

//=======================
// ���l�̐ݒ�
//=======================
void CTime::SetNumber()
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
			m_aPosTexU[i] = (m_nTime % nNum0) / nNum1;
			m_pNumber[i]->Set(m_aPosTexU[i]);
		}
	}
}