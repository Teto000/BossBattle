//===================================================
//
// �v���C���[�̏���
// Author:Teruto Sato
//
//===================================================

//------------------------
// �C���N���[�h
//------------------------
#include "sky.h"
#include "application.h"
#include "renderer.h"

//========================
// �R���X�g���N�^
//========================
CSky::CSky() : CObjectX(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
}

//========================
// �f�X�g���N�^
//========================
CSky::~CSky()
{

}

//========================
// ������
//========================
HRESULT CSky::Init(D3DXVECTOR3 pos)
{
	CObjectX::Init(pos);

	CObjectX::SetFireName("data\\MODEL\\sky.x");

	return S_OK;
}

//========================
// �I��
//========================
void CSky::Uninit()
{
	CObjectX::Uninit();
}

//========================
// �X�V
//========================
void CSky::Update()
{
	CObjectX::Update();
}

//========================
// �`��
//========================
void CSky::Draw()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//���C�g�𖳌��ɂ���
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	CObjectX::Draw();

	//���C�g�̗L����
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//========================
// ����
//========================
CSky* CSky::Create()
{
	CSky *pSky = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pSky = new CSky;	//����

	if (pSky != nullptr)
	{//NULL�`�F�b�N
		//������
		pSky->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		//pSky->SetObjType(OBJTYPE_PLAYER);
	}

	return pSky;
}