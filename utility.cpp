//===================================================
//
// 便利なやつの処理
// Author:Teruto Sato
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "utility.h"

//=======================================
// コンストラクタ
//=======================================
CUtility::CUtility()
{

}

//=======================================
// デストラクタ
//=======================================
CUtility::~CUtility()
{

}

//=======================================
// 当たり判定の処理
//=======================================
CUtility::COLLISION CUtility::Collision(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, D3DXVECTOR3 size, D3DXMATRIX mtx
			, D3DXVECTOR3 targetPos, D3DXVECTOR3 targetSize, D3DXMATRIX targetMtx)
{
	//------------------------------------
	// 行列を元に戻す
	//------------------------------------
	//ワールド座標を求める
	D3DXVECTOR3 worldPos(0.0f, 0.0f, 0.0f);		//ワールド上の座標
	D3DXVec3TransformCoord(&worldPos, &pos, &mtx);

	//逆行列を求める
	D3DXMATRIX invMtxWorld;						//逆行列の値を入れる
	D3DXMatrixInverse(&invMtxWorld, NULL, &mtx);

	//逆行列を使ってローカル座標を求める
	D3DXVECTOR3 localPos(0.0f, 0.0f, 0.0f);		//ローカル上の座標
	D3DXVec3TransformCoord(&localPos, &worldPos, &invMtxWorld);

	//------------------------------------
	// 自分の端の設定
	//------------------------------------
	float fLeft = localPos.x - (size.x / 2);	//自分の左端
	float fRight = localPos.x + (size.x / 2);	//自分の右端
	float fTop = localPos.y + (size.y / 2);		//自分の上端
	float fBottom = localPos.y - (size.y / 2);	//自分の下端
	float fFront = localPos.z - (size.z / 2);	//自分の前端
	float fBack = localPos.z + (size.z / 2);	//自分の後端

	//------------------------------------
	// 相手の行列を元に戻す
	//------------------------------------
	D3DXVec3TransformCoord(&worldPos, &targetPos, &targetMtx);
	D3DXMatrixInverse(&invMtxWorld, NULL, &targetMtx);
	D3DXVec3TransformCoord(&localPos, &worldPos, &invMtxWorld);

	//------------------------------------
	// 相手の端の設定
	//------------------------------------
	float fTargetLeft = localPos.x + targetSize.x;		//相手の左端
	float fTargetRight = localPos.x - targetSize.x;		//相手の右端
	float fTargetTop = localPos.y + targetSize.y;		//相手の上端
	float fTargetBottom = localPos.y - targetSize.y;	//相手の下端
	float fTargetFront = localPos.z + targetSize.z;		//相手の前端
	float fTargetBack = localPos.z - targetSize.z;		//相手の後端

	//------------------------------------
	// 当たり判定
	//------------------------------------
	if (fTargetTop >= fTop && fTargetBottom <= fBottom)
	{//上下の範囲内なら
		//---------------------------------
		// 前後の当たり判定
		//---------------------------------
		if (fTargetLeft >= fLeft && fTargetRight <= fRight)
		{//左右の範囲内なら
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
		// 左右の当たり判定
		//---------------------------------
		if (fTargetFront >= fFront && fTargetBack <= fBack)
		{//前後の範囲内なら
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