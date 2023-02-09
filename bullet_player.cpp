//==============================================
//
// �o���b�g�̏���
// Author : Sato Teruto
//
//==============================================

//----------------------------
// �C���N���[�h
//----------------------------
#include "bullet_player.h"
#include "bullet.h"
#include "camera.h"

//====================================
// �R���X�g���N�^
//====================================
CBulletPlayer::CBulletPlayer() : CBullet(0)
{

}

//====================================
// �f�X�g���N�^
//====================================
CBulletPlayer::~CBulletPlayer()
{

}

//====================================
// ������
//====================================
HRESULT CBulletPlayer::Init(D3DXVECTOR3 pos)
{
	//�����l�̐ݒ�
	m_fWidth = 50.0f;
	m_fHeight = 50.0f;

	CBullet::Init(pos);

	CBullet::SetPos(pos);
	CBullet::SetMove(m_move);
	CBullet::SetSize(m_fWidth, m_fHeight);

	return S_OK;
}

//====================================
// �I��
//====================================
void CBulletPlayer::Uninit()
{
	CBullet::Uninit();
}

//====================================
// �X�V
//====================================
void CBulletPlayer::Update()
{
	CBullet::Update();
}

//====================================
// �`��
//====================================
void CBulletPlayer::Draw()
{
	CBullet::Draw();
}

//====================================
// ����
//====================================
CBulletPlayer* CBulletPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 playerRot)
{
	CBulletPlayer *pBulletPlayer = nullptr;

	//----------------------------------
	// �����Ə�����
	//----------------------------------
	pBulletPlayer = new CBulletPlayer;	//����

	if (pBulletPlayer != nullptr)
	{//NULL�`�F�b�N
		//�ړ���
		pBulletPlayer->m_move = D3DXVECTOR3(sinf(playerRot.x) * sinf(playerRot.y) * 5,
											cosf(playerRot.x) * 5,
											sinf(playerRot.x) * cosf(playerRot.y) * 5);

		//������
		pBulletPlayer->Init(D3DXVECTOR3(pos));
	}

	return pBulletPlayer;
}