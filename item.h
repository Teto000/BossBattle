//======================================================
//
// �A�C�e���w�b�_�[
// Author : Sato Teruto
//
//======================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//-------------------------------
// �C���N���[�h
//-------------------------------
#include "billboard.h"

//======================================
// �A�C�e���N���X
//======================================
class CItem : public CBillBoard
{
public:
	//�A�C�e���̎��
	enum ITEMTYPE
	{
		ITEMTYPE_NONE = 0,
		ITEMTYPE_HEAL,
		ITEMTYPE_MAX
	};

	CItem();			//�R���X�g���N�^
	~CItem() override;	//�f�X�g���N�^

	//-------------------
	// �����o�֐�
	//-------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//-------------------
	// �ÓI�����o�֐�
	//-------------------
	static CItem* Create(D3DXVECTOR3 pos, ITEMTYPE type);

private:
	//-------------------
	// �����o�ϐ�
	//-------------------
	ITEMTYPE m_type;	//���
};

#endif // !_ITEM_H_
