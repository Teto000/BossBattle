//===================================================
//
// ���b�Z�[�W�̏���
// Author : Sato Teruto
//
//===================================================

//----------------------
// �C���N���[�h
//----------------------
#include "message.h"
#include "renderer.h"
#include "game.h"

//=======================
// �R���X�g���N�^
//=======================
CMessage::CMessage() : CObject2D(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//�F
	m_nCntTime = 0;		//���ԃJ�E���g
	m_fWidth = 0.0f;	//��
	m_fHeight = 0.0f;	//����
	m_message = MESSAGE_NONE;
}

//=======================
// �f�X�g���N�^
//=======================
CMessage::~CMessage()
{

}

//=======================
// ������
//=======================
HRESULT CMessage::Init(D3DXVECTOR3 pos)
{
	//�����l�̐ݒ�
	m_pos = pos;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fWidth = 800.0f;
	m_fHeight = 400.0f;

	CObject2D::Init(m_pos);

	CObject2D::SetSize(m_fWidth, m_fHeight);

	CObject2D::SetTexture(CTexture::TEXTURE_CLEAR);

	return S_OK;
}

//=======================
// �I��
//=======================
void CMessage::Uninit()
{
	CObject2D::Uninit();
}

//=======================
// �X�V
//=======================
void CMessage::Update()
{
	CObject2D::Update();

	//���ԃJ�E���g
	if (CGame::GetFinish())
	{//�I���t���O�������Ă���Ȃ�
		m_nCntTime++;
	}

	//-------------------------
	// �����ɂ���
	//-------------------------
	if (m_nCntTime >= 100)
	{//�J�E���g��60�ȏ�Ȃ�
		m_col.a -= 0.02f;	//�����x�̌���
	}

	//�F�̐ݒ�
	CObject2D::SetColor(m_col);

	if (m_col.a <= 0.0f)
	{//���S�ɓ����ɂȂ�����
		Uninit();
	}
}

//=======================
// �`��
//=======================
void CMessage::Draw()
{
	if (CGame::GetFinish())
	{//�I���t���O�������Ă���Ȃ�
		CObject2D::Draw();
	}
}

//=======================
// ����
//=======================
CMessage *CMessage::Create(D3DXVECTOR3 pos, MESSAGE message)
{
	CMessage *pMessage = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pMessage = new CMessage;	//����

	if (pMessage != nullptr)
	{//NULL�`�F�b�N
		//�����̑��
		pMessage->m_message = message;	//�񋓌^

		//������
		pMessage->Init(D3DXVECTOR3(pos));
		pMessage->SetObjType(OBJTYPE_MESSAGE);
	}

	return pMessage;
}