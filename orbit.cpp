//===================================================
//
// �O�Ղ̏���
// Author:Teruto Sato
//
//===================================================

//-----------------------
// �C���N���[�h
//-----------------------
//
#include <assert.h>
#include "orbit.h"
#include "application.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "model.h"

//==========================
// �R���X�g���N�^
//==========================
COrbit::COrbit() : CObject(0)
{
	m_localpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���[�J�����W
	m_worldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���[���h���W
	D3DXMatrixIdentity(&m_mtxWorld);	//���[���h�}�g���b�N�X
	D3DXMatrixIdentity(&m_pMtxParent);	//�e�̃}�g���b�N�X
	m_pVtxBuff = nullptr;				//���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuff = nullptr;				//�C���f�b�N�X�o�b�t�@�̃|�C���^
}

//==========================
// �f�X�g���N�^
//==========================
COrbit::~COrbit()
{
	/*assert(m_pVtxBuff);
	assert(m_pIdxBuff);*/
}

//==========================
// ������
//==========================
HRESULT COrbit::Init(D3DXVECTOR3 pos)
{
	//�����l�̐ݒ�
	m_localpos = pos;

	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//�f�o�C�X�̎擾

	//----------------------------------
	// ���b�V�����\��������̐ݒ�
	//----------------------------------
	m_nNumDivision = 12;				//�|���S���̕�����
	m_nCntVtx = m_nNumDivision + 1;		//���_�̕�����
	m_nNumVtx = (m_nNumDivision + 1) * (m_nNumDivision + 1);							//���_��
	m_nNumPolygon = m_nNumDivision * m_nNumDivision * 2 + (m_nNumDivision - 1) * 4;		//�|���S����
	m_nNumIndex = (m_nNumDivision + 1) * 2 * m_nNumDivision + (m_nNumDivision - 1) * 2;	//�C���f�b�N�X��
	m_fMaxWidth = 5500.0f;								//���b�V���t�B�[���h�̍ő啝
	m_fMeshWidth = (m_fMaxWidth / m_nNumDivision);		//���b�V���t�B�[���h�̕�
	m_fTexSize = (5.0f / m_nNumDivision);				//�e�N�X�`���̕����T�C�Y


	//----------------------------------
	// ���_�o�b�t�@�̐���
	//----------------------------------
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,	//�m�ۂ���o�b�t�@�̃T�C�Y
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,
								&m_pVtxBuff,
								NULL);

	//----------------------------------
	// �C���f�b�N�X�o�b�t�@�̐���
	//----------------------------------
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex,	//�m�ۂ���o�b�t�@�̃T�C�Y
								D3DUSAGE_WRITEONLY,
								D3DFMT_INDEX16,			//���_�t�H�[�}�b�g
								D3DPOOL_MANAGED,
								&m_pIdxBuff,
								NULL);

	VERTEX_3D*pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//----------------------------------
	// ���_���̐ݒ�
	//----------------------------------
	for (int z = 0; z < m_nCntVtx; z++)
	{
		for (int x = 0; x < m_nCntVtx; x++)
		{
			//���_���W�̐ݒ�
			{
				D3DXVECTOR3 vtxPos(-(m_fMaxWidth / 2) + (m_fMeshWidth * x), 0.0f, (m_fMaxWidth / 2) - (m_fMeshWidth * z));
				pVtx[x + (z * (m_nCntVtx))].pos = vtxPos;
			}

			//�e���_�̖@���̐ݒ�(���x�N�g���̑傫���͕K��1�ɂ���K�v������)
			pVtx[x + (z * (m_nCntVtx))].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[x + (z * (m_nCntVtx))].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[x + (z * (m_nCntVtx))].tex = D3DXVECTOR2(0.0f, 1.0f);
		}
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//�C���f�b�N�X�o�b�t�@�̃��b�N
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//----------------------------------
	// �C���f�b�N�X�̐ݒ�
	//----------------------------------
	int nCnt = 0;

	for (int Z = 0; Z < m_nNumDivision; Z++)
	{
		for (int X = 0; X < m_nCntVtx; X++)
		{
			//�C���f�b�N�X�o�b�t�@�̐ݒ�
			pIdx[nCnt] = (WORD(X + Z * (m_nCntVtx)+(m_nCntVtx)));
			pIdx[nCnt + 1] = (WORD(X + Z * (m_nCntVtx)));

			nCnt += 2;

			//�k�ރ|���S���̒ǉ�
			if (X == m_nNumDivision)
			{
				pIdx[nCnt] = (WORD)(X + Z * m_nCntVtx);
				pIdx[nCnt + 1] = (WORD)((Z + 2) * m_nCntVtx);

				nCnt += 2;
			}
		}
	}

	pVtx += m_nNumVtx;
	pIdx += m_nNumIndex;

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_pIdxBuff->Unlock();

	return S_OK;
}

//==========================
// �I��
//==========================
void COrbit::Uninit()
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}
}

//==========================
// �X�V
//==========================
void COrbit::Update()
{
	VERTEX_3D*pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//--------------------------------------
	// ���_���W����납��R�s�[����
	//--------------------------------------
	for (int i = nMaxVtx; i > 1; i--)
	{//���_�����~���ɉ�
		pVtx[i].pos = pVtx[i - 1].pos;
	}

	//--------------------------------------
	// �󂢂�0��1�ɋO�Ղ̐V�������W����
	//--------------------------------------
	//�e���f���̌��_����I�t�Z�b�g���ړ��������W����
	D3DXVec3TransformCoord(&m_worldPos, &m_localpos, &m_pMtxParent);
	pVtx[1].pos = m_worldPos;

	//�e���f���̌��_�̍��W����
	D3DXVec3TransformCoord(&m_worldPos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &m_pMtxParent);
	pVtx[0].pos = m_worldPos;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==========================
// �`��
//==========================
void COrbit::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//�f�o�C�X�̎擾

	//------------------------------------
	// �}�g���b�N�X
	//------------------------------------
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//���̌������擾
	D3DXVECTOR3 swordRot(CGame::GetPlayer()->GetModel(6)->GetRot());

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, swordRot.y, swordRot.x, swordRot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//���̈ʒu���擾
	D3DXVECTOR3 swordPos(CGame::GetPlayer()->GetModel(6)->GetPos());

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, swordPos.x, swordPos.y, swordPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//------------------------------------
	// ���_
	//------------------------------------
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
}

//==========================
// ����
//==========================
COrbit* COrbit::Create(D3DXVECTOR3 offset, D3DXMATRIX mtx)
{
	COrbit* pOrbit = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pOrbit = new COrbit;	//����

	if (pOrbit != nullptr)
	{//NULL�`�F�b�N
		//�����̑��
		pOrbit->m_pMtxParent = mtx;	//�e�̃}�g���b�N�X

		//������
		pOrbit->Init(offset);
		pOrbit->SetObjType(OBJTYPE_MESHORBIT);
	}

	return pOrbit;
}