//===================================================
//
// �֗��Ȃ�w�b�_�[
// Author:Teruto Sato
//
//===================================================
#ifndef _UTILITY_H_
#define _UTILITY_H_	//��d�C���N���[�h�h�~

//--------------------------
// �C���N���[�h
//--------------------------
#include "object.h"

//================================
// ���[�e�B���e�B�N���X�̒�`
//================================
class CUtility
{
public:
	enum COLLISION
	{
		COLLISION_NONE = 0,	//�������Ă��Ȃ�
		COLLISION_FRONT,	//�O���瓖������
		COLLISION_BACK,		//��납��
		COLLISION_LEFT,		//������
		COLLISION_RIGHT,	//�E����
		COLLISION_MAX
	};

	CUtility();		//�R���X�g���N�^
	~CUtility();	//�f�X�g���N�^

	//-----------------------
	// �ÓI�����o�֐�
	//-----------------------
	//�����蔻��̏���
	static COLLISION Collision(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, D3DXVECTOR3 size, D3DXMATRIX mtx
				, D3DXVECTOR3 targetPos, D3DXVECTOR3 targetSize, D3DXMATRIX targetMtx);

	//�����߂��ꂽ�ʒu��Ԃ�����
	static D3DXVECTOR3 GetCollisionPos(D3DXVECTOR3 pos, D3DXVECTOR3 posOld
		, D3DXVECTOR3 size, D3DXMATRIX mtx, CObject::EObjType targetType);
};

#endif _UTILITY_H_