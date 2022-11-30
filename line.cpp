//===================================================
//
// ���C���̏���
// Author : Sato Teruto
//
//===================================================

//------------------------
// �C���N���[�h
//------------------------
#include <assert.h>
#include "line.h"
#include "main.h"
#include "renderer.h"
#include "application.h"

//===========================
// �R���X�g���N�^
//===========================
CLine::CLine()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	m_start = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�n�_
	m_end = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�I�_
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�F
	m_pVtxBuff = nullptr;	//���_�o�b�t�@�ւ̃|�C���^
}

//===========================
// �f�X�g���N�^
//===========================
CLine::~CLine()
{
	assert(m_pVtxBuff == nullptr);
}

//===========================
// ������
//===========================
HRESULT CLine::Init()
{
	CObject3D::Init(m_pos);

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2,	//�m�ۂ���o�b�t�@�̃T�C�Y
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,
								&m_pVtxBuff,
								nullptr);

	//-------------------------------
	//	���_���̐ݒ�
	//-------------------------------
	VERTEX_3D*pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�e���_�̖@���̐ݒ�(���x�N�g���̑傫���͕K��1�ɂ���K�v������)
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���_���̐ݒ�
	CObject3D::SetVtx(pVtx);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//===========================
// �I��
//===========================
void CLine::Uninit()
{
	CObject3D::Uninit();

	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	Release();
}

//===========================
// �X�V
//===========================
void CLine::Update()
{
	CObject3D::Update();
}

//===========================
// �`��
//===========================
void CLine::Draw()
{
	CObject3D::Draw();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_LINELIST,	//�v���~�e�B�u�̎��
						   2,				//�`�悷��ŏ��̒��_�C���f�b�N�X
						   1);				//�`�悷��v���~�e�B�u��
}

//===========================
// ���̐ݒ�
// �����F�ʒu�A�n�_�A�I�_�A�F
//===========================
CLine* CLine::Create(D3DXVECTOR3 pos, D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col)
{
	CLine* pLine = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pLine = new CLine;	//����

	if (pLine != nullptr)
	{//NULL�`�F�b�N
		//�ϐ��ɑ��
		pLine->m_pos = pos;		//�ʒu
		pLine->m_start = start;	//�n�_
		pLine->m_end = end;		//�I�_
		pLine->m_col = col;		//�F

		//������
		pLine->Init();
	}

	return pLine;
}