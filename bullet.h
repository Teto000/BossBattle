//=============================================
//
// �o���b�g�w�b�_�[
// Author : Sato Teruto
//
//=============================================
#ifndef _BULLET_H_
#define _BULLET_H_

//-------------------------
// �C���N���[�h
//-------------------------
#include "billboard.h"

//====================================
// �o���b�g�N���X
//====================================
class CBullet : public CBillBoard
{
public:
	CBullet();
	explicit CBullet(int nPriority);	//�R���X�g���N�^
	virtual ~CBullet() override;		//�f�X�g���N�^

	//---------------------
	// �����o�֐�
	//---------------------
	virtual HRESULT Init(D3DXVECTOR3 pos) override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
};

#endif // !_BULLET_H_
