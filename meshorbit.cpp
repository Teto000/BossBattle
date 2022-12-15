//===================================================
//
// �O�Ղ̏���
// Author:Teruto Sato
//
//===================================================

//-----------------------
// �C���N���[�h
//-----------------------
#include <assert.h>
#include "meshorbit.h"
#include "application.h"
#include "renderer.h"

//==========================
// �R���X�g���N�^
//==========================
CMeshOrbit::CMeshOrbit() : CObject(0)
{
	m_localpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���[�J�����W
	m_worldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���[���h���W
	D3DXMatrixIdentity(m_pMtxParent);				//�e�̃}�g���b�N�X
	m_pVtxBuff = nullptr;							//���_�o�b�t�@�ւ̃|�C���^
}

//==========================
// �f�X�g���N�^
//==========================
CMeshOrbit::~CMeshOrbit()
{

}

//==========================
// ������
//==========================
HRESULT CMeshOrbit::Init(D3DXVECTOR3 pos)
{
	return S_OK;
}

//==========================
// �I��
//==========================
void CMeshOrbit::Uninit()
{

}

//==========================
// �X�V
//==========================
void CMeshOrbit::Update()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	VERTEX_3D*pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//--------------------------------------
	// ���_���W����납��R�s�[����
	//--------------------------------------
	for (int i = nMaxVtx; i > 1; i++)
	{//���_�����~���ɉ�
		pVtx[i].pos = pVtx[i - 1].pos;
	}

	//--------------------------------------
	// �󂢂�0��1�ɋO�Ղ̐V�������W����
	//--------------------------------------
	// �e���f���̌��_����I�t�Z�b�g���ړ��������W����
	D3DXVec3TransformCoord(&m_worldPos, &m_localpos, m_pMtxParent);
	pVtx[1].pos = m_worldPos;

	// �e���f���̌��_�̍��W����
	D3DXVec3TransformCoord(&m_worldPos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pMtxParent);
	pVtx[0].pos = m_worldPos;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==========================
// �`��
//==========================
void CMeshOrbit::Draw()
{

}

//==========================
// ����
//==========================
CMeshOrbit* CMeshOrbit::Create(D3DXVECTOR3 pos)
{
	CMeshOrbit *pMeshOrbit = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pMeshOrbit = new CMeshOrbit;	//����

	if (pMeshOrbit != nullptr)
	{//NULL�`�F�b�N
		//������
		pMeshOrbit->Init(pos);
		pMeshOrbit->SetObjType(OBJTYPE_MESHORBIT);
	}

	return pMeshOrbit;
}