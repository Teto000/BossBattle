//===================================================
//
// HP�w�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _HP_H_
#define _HP_H_	//��d�C���N���[�h�h�~

//--------------------------------
// �C���N���[�h
//--------------------------------
#include "gauge.h"

//--------------------------------
// HP�N���X
//--------------------------------
class CHP : public CGauge
{
public:
	enum HPTYPE
	{
		HPTYPE_NONE = 0,
		HPTYPE_PLAYER,
		HPTYPE_ENEMY,
		HPTYPE_MAX
	};

	CHP();				//�R���X�g���N�^
	~CHP() override;	//�f�X�g���N�^

	//------------------
	// �����o�֐�
	//------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//------------------
	// �ÓI�����o�֐�
	//------------------
	static CHP* Create(D3DXVECTOR3 pos, float fWidth, float fHeight, HPTYPE type);

private:
	//------------------
	// �����o�ϐ�
	//------------------
	HPTYPE m_type;	//���
};

#endif