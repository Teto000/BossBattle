//===================================================
//
// �֗��Ȃ�̏���
// Author:Teruto Sato
//
//===================================================

//------------------------
// �C���N���[�h
//------------------------
#include "utility.h"
#include "player.h"
#include "enemy.h"

//=======================================
// �R���X�g���N�^
//=======================================
CUtility::CUtility()
{

}

//=======================================
// �f�X�g���N�^
//=======================================
CUtility::~CUtility()
{

}

//=======================================
// �����蔻��̏���
//=======================================
CUtility::COLLISION CUtility::Collision(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, D3DXVECTOR3 size, D3DXMATRIX mtx
			, D3DXVECTOR3 targetPos, D3DXVECTOR3 targetSize, D3DXMATRIX targetMtx)
{
	//------------------------------------
	// �s������ɖ߂�
	//------------------------------------
	//���[���h���W�����߂�
	D3DXVECTOR3 worldPos(0.0f, 0.0f, 0.0f);		//���[���h��̍��W
	D3DXVec3TransformCoord(&worldPos, &pos, &mtx);

	//�t�s������߂�
	D3DXMATRIX invMtxWorld;						//�t�s��̒l������
	D3DXMatrixInverse(&invMtxWorld, NULL, &mtx);

	//�t�s����g���ă��[�J�����W�����߂�
	D3DXVECTOR3 localPos(0.0f, 0.0f, 0.0f);		//���[�J����̍��W
	D3DXVec3TransformCoord(&localPos, &worldPos, &invMtxWorld);

	//------------------------------------
	// �����̒[�̐ݒ�
	//------------------------------------
	float fLeft = localPos.x - (size.x / 2);	//�����̍��[
	float fRight = localPos.x + (size.x / 2);	//�����̉E�[
	float fTop = localPos.y + (size.y / 2);		//�����̏�[
	float fBottom = localPos.y - (size.y / 2);	//�����̉��[
	float fFront = localPos.z - (size.z / 2);	//�����̑O�[
	float fBack = localPos.z + (size.z / 2);	//�����̌�[

	//------------------------------------
	// ����̍s������ɖ߂�
	//------------------------------------
	D3DXVec3TransformCoord(&worldPos, &targetPos, &targetMtx);
	D3DXMatrixInverse(&invMtxWorld, NULL, &targetMtx);
	D3DXVec3TransformCoord(&localPos, &worldPos, &invMtxWorld);

	//------------------------------------
	// ����̒[�̐ݒ�
	//------------------------------------
	float fTargetLeft = localPos.x + targetSize.x;		//����̍��[
	float fTargetRight = localPos.x - targetSize.x;		//����̉E�[
	float fTargetTop = localPos.y + targetSize.y;		//����̏�[
	float fTargetBottom = localPos.y - targetSize.y;	//����̉��[
	float fTargetFront = localPos.z + targetSize.z;		//����̑O�[
	float fTargetBack = localPos.z - targetSize.z;		//����̌�[

	//------------------------------------
	// �����蔻��
	//------------------------------------
	if (fTargetTop >= fTop && fTargetBottom <= fBottom)
	{//�㉺�͈͓̔��Ȃ�
		//---------------------------------
		// �O��̓����蔻��
		//---------------------------------
		if (fTargetLeft >= fLeft && fTargetRight <= fRight)
		{//���E�͈͓̔��Ȃ�
			if (fTargetFront >= fFront && fTargetFront < posOld.z - (size.z / 2))
			{
				return COLLISION_FRONT;
			}
			else if (fTargetBack <= fBack && fTargetBack > posOld.z - (size.z / 2))
			{
				return COLLISION_BACK;
			}
		}
		//---------------------------------
		// ���E�̓����蔻��
		//---------------------------------
		if (fTargetFront >= fFront && fTargetBack <= fBack)
		{//�O��͈͓̔��Ȃ�
			if (fTargetLeft >= fLeft && fTargetLeft < posOld.x + (size.x / 2))
			{
				return COLLISION_LEFT;
			}
			else if (fTargetRight <= fRight && fTargetRight > posOld.x - (size.x / 2))
			{
				return COLLISION_RIGHT;
			}
		}
	}

	return COLLISION_NONE;
}

//=======================================
// �����߂��ꂽ�ʒu��Ԃ�����
//=======================================
D3DXVECTOR3 CUtility::GetCollisionPos(D3DXVECTOR3 pos, D3DXVECTOR3 posOld
	, D3DXVECTOR3 size, D3DXMATRIX mtx, CObject::EObjType targetType)
{
	//�ϐ��錾
	D3DXVECTOR3 targetPos;		//����̈ʒu
	D3DXVECTOR3 targetSize;		//����̑傫��
	D3DXMATRIX targetMtx;		//����̃}�g���b�N�X

	D3DXVECTOR3 returnPos = pos;	//�Ԃ��l�̑���p�ϐ�

	//--------------------------
	// ����̏����擾
	//--------------------------
	for (int i = 0; i < CObject::GetMaxPriolity(); i++)
	{//�v���C�I���e�B����
		CObject* pObj;
		pObj = CObject::GETObject(targetType, i);

		if (pObj == nullptr)
		{//null�Ȃ�߂�
			continue;
		}

		//---------------------------
		// �I�u�W�F�N�g�̎�ނ̎擾
		//---------------------------
		CObject::EObjType type = pObj->GetObjType();
		if (type != targetType)
		{//�I�u�W�F�N�g�̎�ނ��ړI�̑��肶��Ȃ��Ȃ�
			continue;
		}

		//-----------------------------
		// ����̎�ނ��Ƃ̏���
		//-----------------------------
		if (targetType == CObject::OBJTYPE_PLAYER)
		{//���肪�v���C���[�Ȃ�
			//�_�E���L���X�g
			CPlayer* pPlayer = (CPlayer*)pObj;

			//���̎擾
			targetPos = pPlayer->GetPosition();
			targetSize = pPlayer->GetSize();
			targetMtx = pPlayer->GetmtxWorld();
		}
		else if (targetType == CObject::OBJTYPE_ENEMY)
		{//���肪�G�Ȃ�
			//�_�E���L���X�g
			CEnemy* pEnemy = (CEnemy*)pObj;

			//���̎擾
			targetPos = pEnemy->GetPosition();
			targetSize = pEnemy->GetSize();
			targetMtx = pEnemy->GetmtxWorld();
		}

		//--------------------------
		// �����蔻��̏���
		//--------------------------
		CUtility::COLLISION direction = CUtility::Collision(pos, posOld, size, mtx
			, targetPos, targetSize, targetMtx);

		//--------------------------
		// �������������ɉ���������
		//--------------------------
		switch (direction)
		{
		case CUtility::COLLISION_FRONT:
			returnPos.z = targetPos.z + targetSize.z + (size.z / 2);
			break;

		case CUtility::COLLISION_BACK:
			returnPos.z = targetPos.z - targetSize.z - (size.z / 2);
			break;

		case CUtility::COLLISION_LEFT:
			returnPos.x = targetPos.x + targetSize.x + (size.x / 2);
			break;

		case CUtility::COLLISION_RIGHT:
			returnPos.x = targetPos.x - targetSize.x - (size.x / 2);
			break;

		default:
			break;
		}
	}

	return returnPos;
}