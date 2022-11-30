//===================================================
//
// ���C���w�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _LINE_H_
#define _LINE_H_	//��d�C���N���[�h�h�~

//--------------------------------
// �C���N���[�h
//--------------------------------
#include <d3dx9.h>
#include "object3d.h"

//================================
// ���C���N���X�̒�`
//================================
class CLine : public CObject3D
{
public:
	CLine();	//�R���X�g���N�^
	~CLine();	//�f�X�g���N�^

	//--------------------
	// �����o�֐�
	//--------------------
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	//--------------------
	// �ÓI�����o�֐�
	//--------------------
	static CLine* Create(D3DXVECTOR3 pos, D3DXVECTOR3 start
						, D3DXVECTOR3 end, D3DXCOLOR col);

private:
	//--------------------
	// �����o�ϐ�
	//--------------------
	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_rot;		//����
	D3DXVECTOR3 m_start;	//�n�_
	D3DXVECTOR3 m_end;		//�I�_
	D3DXCOLOR	m_col;		//�F
	D3DXMATRIX	m_mtxWorld;	//���[���h�}�g���b�N�X
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
};

#endif // !_LINE_H_