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
#include "object2D.h"

//--------------------------------
// �O���錾
//--------------------------------
class CFream;	//�t���[������

//--------------------------------
// �Q�[�W�N���X
//--------------------------------
class CGauge : public CObject2D
{
public:
	enum GAUGETYPE
	{
		GAUGETYPE_NONE = 0,
		GAUGETYPE_PLAYER,
		GAUGETYPE_ENEMY,
		GAUGETYPE_MAX
	};

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

	//------------------
	// �Z�b�^�[
	//------------------
	void SetLife(float fLife, float fRemLife);		//HP�̐ݒ�
	void SubHP();									//HP�������̏���
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }	//�ʒu�̐ݒ�
	void SetType(int nNum);		//��ނ̐ݒ�

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
	GAUGETYPE m_type;	//���
	CFream* m_pFream;	//�t���[��
};

#endif