//===================================================
//
// �퓬�X�^�C���\���w�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _STYLE_SHIFT_H_
#define _STYLE_SHIFT_H_	//��d�C���N���[�h�h�~

//--------------------------------
// �C���N���[�h
//--------------------------------
#include "billboard.h"

//================================
// �X�^�C���V�t�g�N���X�̒�`
//================================
class CStyleShift : public CBillBoard
{
public:
	enum STYLE_TYPE
	{
		STYLE_TYPE_NONE = 0,
		STYLE_TYPE_ATTACK,
		STYLE_TYPE_SPEED,
		STYLE_TYPE_COMBO,
		STYLE_TYPE_MAX
	};

	CStyleShift();				//�R���X�g���N�^
	~CStyleShift() override;	//�f�X�g���N�^

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
	void SetStyle(bool flash);	//�X�^�C���̏�Ԃ̐ݒ�

	//------------------
	// �ÓI�����o�֐�
	//------------------
	static CStyleShift* Create(D3DXVECTOR3 pos, STYLE_TYPE type);

private:
	//------------------
	// �����o�ϐ�
	//------------------
	D3DXVECTOR3 m_stylePos;
	D3DXVECTOR3 m_offSet;
	bool m_bFlash;		//���邭���邩
	STYLE_TYPE m_type;	//�X�^�C���̎��
};

#endif