//======================================================
//
// �A�C�e���̏���
// Author : Sato Teruto
//
//======================================================

//-------------------------------
// �C���N���[�h
//-------------------------------
#include "item.h"
#include "utility.h"
#include "game.h"
#include "player.h"

//===============================================
// �R���X�g���N�^
//===============================================
CItem::CItem()
{
	m_type = ITEMTYPE_NONE;		//���
}

//===============================================
// �f�X�g���N�^
//===============================================
CItem::~CItem()
{

}

//===============================================
// ������
// �����F�ʒu
//===============================================
HRESULT CItem::Init(D3DXVECTOR3 pos)
{
	//�����l�̐ݒ�
	m_fWidth = 70.0f;
	m_fHeight = 70.0f;

	CBillBoard::Init(pos);
	CBillBoard::SetSize(m_fWidth, m_fHeight);
	CBillBoard::SetTexture(CTexture::TEXTURE_CLEAR);

	return S_OK;
}

//===============================================
// �I��
//===============================================
void CItem::Uninit()
{
	CBillBoard::Uninit();
}

//===============================================
// �X�V
//===============================================
void CItem::Update()
{
	CBillBoard::Update();

	//--------------------------------
	// �v���C���[�̏����擾
	//--------------------------------
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPosition();
	D3DXVECTOR3 playerSize = CGame::GetPlayer()->GetSize();

	//--------------------------------
	// �����蔻��
	//--------------------------------
	if (Collision(playerPos, playerSize))
	{//�v���C���[�Ɠ���������
		switch (m_type)
		{
		case ITEMTYPE_HEAL:
			CGame::GetPlayer()->AddLife(100);
			break;

		default:
			break;
		}

		//�A�C�e���̏���
		Uninit();
	}
}

//===============================================
// �`��
//===============================================
void CItem::Draw()
{
	CBillBoard::Draw();
}

//===============================================
// ����
// �����F�ʒu
//===============================================
CItem * CItem::Create(D3DXVECTOR3 pos, ITEMTYPE type)
{
	CItem* pItem = nullptr;

	//----------------------------------
	// �����Ə�����
	//----------------------------------
	pItem = new CItem;	//����

	if (pItem != nullptr)
	{//NULL�`�F�b�N
		//�����o�ϐ��ɑ��
		pItem->m_type = type;

		//������
		pItem->Init(pos);
	}

	return pItem;
}

//===============================================
// �����蔻��
// �����F����̈ʒu�A����̑傫��
//===============================================
bool CItem::Collision(D3DXVECTOR3 targetPos, D3DXVECTOR3 targetSize)
{
	//--------------------------------
	// �v���C���[�̒[�̐ݒ�
	//--------------------------------
	float fLeft = targetPos.x - targetSize.x;
	float fRight = targetPos.x + targetSize.x;
	float fTop = targetPos.y + targetSize.y;
	float fBottom = targetPos.y - targetSize.y;
	float fFront = targetPos.z - targetSize.z;
	float fBack = targetPos.z + targetSize.z;

	//--------------------------------
	// �v���C���[�Ƃ̓����蔻��
	//--------------------------------
	float fRange = 30.0f;	//�����蔻��͈̔�

	if (m_pos.x + fRange >= fLeft
		&& m_pos.x - fRange <= fRight
		&& m_pos.y + fRange >= fBottom
		&& m_pos.y - fRange <= fTop
		&& m_pos.z + fRange >= fFront
		&& m_pos.z - fRange <= fBack)
	{//�v���C���[���͈͓��ɓ�������
		return true;
	}

	return false;
}