//=============================================
//
// �o���b�g�w�b�_�[
// Author : Sato Teruto
//
//=============================================
#ifndef _BULLET_PLAYER_H_
#define _BULLET_PLAYER_H_

//-------------------------
// �C���N���[�h
//-------------------------
#include "bullet.h"

//====================================
// �o���b�g�N���X
//====================================
class CBulletPlayer : public CBullet
{
public:
	CBulletPlayer();
	~CBulletPlayer() override;		//�f�X�g���N�^

	//---------------------
	// �����o�֐�
	//---------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//---------------------
	// �ÓI�����o�֐�
	//---------------------
	static CBulletPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 playerRot);
};

#endif
