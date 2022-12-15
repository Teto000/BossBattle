//===================================================
//
// �O�Ճw�b�_�[
// Author:Teruto Sato
//
//===================================================
#ifndef _MESHORBIT_H_
#define _MESHORBIT_H_	//��d�C���N���[�h�h�~

//--------------------------------
// �C���N���[�h
//--------------------------------
#include "object.h"

//--------------------------------
// ���̋O�ՃN���X
//--------------------------------
class CMeshOrbit : public CObject
{
public:
	CMeshOrbit();	//�R���X�g���N�^
	~CMeshOrbit();	//�f�X�g���N�^

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
	void SetMtxParent(D3DXMATRIX* pMtx) { m_pMtxParent = pMtx; };	//�e�̃}�g���b�N�X�̐ݒ�

	//------------------
	// �Q�b�^�[
	//------------------
	D3DXVECTOR3 GetPosition() override { return m_localpos; }	//�ʒu�̎擾
	float GetWidth() override { return 0.0f; }				//���̎擾
	float GetHeight() override { return 0.0f; }				//�����̎擾

	//------------------
	// �ÓI�����o�ϐ�
	//------------------
	static CMeshOrbit* Create(D3DXVECTOR3 pos);

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
	D3DXMATRIX* m_pMtxParent;			//�O�Ղ̐e���f���̃}�g���b�N�X
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
};

#endif