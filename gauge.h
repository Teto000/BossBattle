//===================================================
//
// �Q�[�W�̃w�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_	//��d�C���N���[�h�h�~

//--------------------------------
// �C���N���[�h
//--------------------------------
#include "billboard.h"

//--------------------------------
// �Q�[�W�N���X
//--------------------------------
class CGauge : public CBillBoard
{
public:
	CGauge();
	explicit CGauge(int nPriority);	//�R���X�g���N�^
	virtual ~CGauge() override;		//�f�X�g���N�^

	//------------------
	// �����o�֐�
	//------------------
	virtual HRESULT Init(D3DXVECTOR3 pos) override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void SetLife(float fLife, float fRemLife);	//HP�̐ݒ�
	void SubHP();	//HP�������̏���

protected:
	//------------------
	// �����o�ϐ�
	//------------------
	D3DXVECTOR3 m_pos;	//�ʒu
	float m_fLife;		//�̗͎󂯎��p
	float m_fRemLife;	//�c��̗͎󂯎��p
	float m_fWidth;		//��
	float m_fHeight;	//����
	float m_fLength;	//HP�o�[�̒���
};

#endif