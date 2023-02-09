//===================================================
//
// �O�Ճw�b�_�[
// Author:Teruto Sato
//
//===================================================
#ifndef _ORBIT_H_
#define _ORBIT_H_	//��d�C���N���[�h�h�~

//--------------------------------
// �C���N���[�h
//--------------------------------
#include "object.h"

//--------------------------------
// ���̋O�ՃN���X
//--------------------------------
class COrbit : public CObject
{
public:
	COrbit();	//�R���X�g���N�^
	~COrbit();	//�f�X�g���N�^

	//------------------
	// �����o�֐�
	//------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//------------------
	// �Z�b�^�[
	//------------------
	void SetMtxParent(D3DXMATRIX pMtx) { m_pMtxParent = pMtx; };	//�e�̃}�g���b�N�X�̐ݒ�

	//------------------
	// �Q�b�^�[
	//------------------
	D3DXVECTOR3 GetPosition() override { return m_localpos; }	//�ʒu�̎擾
	float GetWidth() override { return 0.0f; }				//���̎擾
	float GetHeight() override { return 0.0f; }				//�����̎擾

	//------------------
	// �ÓI�����o�ϐ�
	//------------------
	static COrbit* Create(D3DXVECTOR3 offset, D3DXMATRIX mtx);

private:
	//------------------
	// �萔
	//------------------
	static const int nMaxVtx = 32;		//���_�̍ő吔

	//------------------
	// �����o�ϐ�
	//------------------
	D3DXVECTOR3 m_localpos;				//���[�J�����W
	D3DXVECTOR3 m_worldPos;				//���[���h���W
	D3DXMATRIX	m_mtxWorld;				//���[���h�}�g���b�N�X
	D3DXMATRIX m_pMtxParent;			//�O�Ղ̐e���f���̃}�g���b�N�X

	/* �� ���b�V�����\�������� */
	int m_nNumDivision;	//�|���S���̕�����
	int m_nCntVtx;			//���_�̕�����
	int m_nNumVtx;			//���_��
	int m_nNumPolygon;		//�|���S����
	int m_nNumIndex;		//�C���f�b�N�X��
	float m_fMaxWidth;		//���b�V���t�B�[���h�̍ő啝
	float m_fMeshWidth;		//���b�V���t�B�[���h�̕�
	float m_fTexSize;		//�e�N�X�`���̕����T�C�Y

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
};

#endif