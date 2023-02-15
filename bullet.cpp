//==============================================
//
// �o���b�g�̏���
// Author : Sato Teruto
//
//==============================================

//----------------------------
// �C���N���[�h
//----------------------------
#include "bullet.h"
#include "application.h"
#include "renderer.h"

//====================================
// �R���X�g���N�^
//====================================
CBullet::CBullet() : CBillBoard(0)
{
}
CBullet::CBullet(int nPriority) : CBillBoard(nPriority)
{

}

//====================================
// �f�X�g���N�^
//====================================
CBullet::~CBullet()
{

}

//====================================
// ������
//====================================
HRESULT CBullet::Init(D3DXVECTOR3 pos)
{
	//�����l�̐ݒ�
	m_fWidth = 50.0f;
	m_fHeight = 50.0f;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	CBillBoard::Init(pos);

	CBillBoard::SetPos(pos);
	CBillBoard::SetMove(m_move);
	CBillBoard::SetSize(m_fWidth, m_fHeight);
	CBillBoard::SetColor(m_col);
	CBillBoard::SetTexture(CTexture::TEXTURE_AIM);

	return S_OK;
}

//====================================
// �I��
//====================================
void CBullet::Uninit()
{
	CBillBoard::Uninit();
}

//====================================
// �X�V
//====================================
void CBullet::Update()
{
	CBillBoard::Update();

	//�ʒu�Ɉړ��ʂ����Z
	m_pos = CBillBoard::AddMove(m_move);
	CBillBoard::SetPos(m_pos);
}

//====================================
// �`��
//====================================
void CBullet::Draw()
{
	//�I�u�W�F�N�g�̕`��
	CBillBoard::Draw();
}

//====================================
// ����
//====================================
CBullet* CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 playerRot)
{
	CBullet *pBullet = nullptr;

	//----------------------------------
	// �����Ə�����
	//----------------------------------
	pBullet = new CBullet;	//����

	if (pBullet != nullptr)
	{//NULL�`�F�b�N
		//�ړ���
		pBullet->m_move = D3DXVECTOR3(sinf(playerRot.x) * sinf(playerRot.y) * 5,
									  cosf(playerRot.x) * 5,
									  sinf(playerRot.x) * cosf(playerRot.y) * 5);

		//������
		pBullet->Init(D3DXVECTOR3(pos));
	}

	return pBullet;
}