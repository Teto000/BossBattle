//===================================================
//
// ���b�Z�[�W�w�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _MESSAGE_H_
#define _MESSAGE_H_	//��d�C���N���[�h�h�~

//-------------------------------
// �C���N���[�h
//-------------------------------
#include "object2d.h"

//================================
// ���b�Z�[�W�N���X
//================================
class CMessage : public CObject2D
{
public:
	enum MESSAGE
	{
		MESSAGE_NONE = 0,
		MESSAGE_CLEAR,		//�N���A
		MESSAGE_BREAK,		//�u���C�N
		MESSAGE_MAX
	};

	CMessage();		//�R���X�g���N�^
	~CMessage();	//�f�X�g���N�^

	//------------------
	// �����o�֐�
	//------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//------------------
	// �ÓI�����o�ϐ�
	//------------------
	static CMessage* Create(D3DXVECTOR3 pos, MESSAGE message);

private:
	//------------------
	// �����o�ϐ�
	//------------------
	D3DXCOLOR m_col;	//�F
	int m_nCntTime;		//���ԃJ�E���g
	MESSAGE m_message;	//���b�Z�[�W
};

#endif