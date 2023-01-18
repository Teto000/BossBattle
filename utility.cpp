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