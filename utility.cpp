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
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "debug_proc.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
D3DXVECTOR3 CUtility::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
D3DXVECTOR3 CUtility::m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//前の位置
D3DXVECTOR3 CUtility::m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//大きさ
D3DXMATRIX  CUtility::m_mtxWorld;	//ワールドマトリックス

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

//=============================================================================
// 当たり判定の処理
// 引数：自分(位置、前の位置、大きさ、マトリックス)
//=============================================================================
CUtility::COLLISION CUtility::Collision(D3DXVECTOR3 targetPos, D3DXVECTOR3 targetSize, D3DXMATRIX targetMtx)
{
	//------------------------------------
	// 行列を元に戻す
	//------------------------------------
	////ワールド座標を求める
	//D3DXVECTOR3 worldPos(0.0f, 0.0f, 0.0f);		//ワールド上の座標
	//D3DXVec3TransformCoord(&worldPos, &m_pos, &m_mtxWorld);

	////逆行列を求める
	//D3DXMATRIX invMtxWorld;						//逆行列の値を入れる
	//D3DXMatrixInverse(&invMtxWorld, NULL, &m_mtxWorld);

	////逆行列を使ってローカル座標を求める
	//D3DXVECTOR3 localPos(0.0f, 0.0f, 0.0f);		//ローカル上の座標
	//D3DXVec3TransformCoord(&localPos, &worldPos, &invMtxWorld);

	//------------------------------------
	// 自分の端の設定
	//------------------------------------
	float fLeft = m_pos.x - (m_size.x / 2);		//自分の左端
	float fRight = m_pos.x + (m_size.x / 2);	//自分の右端
	float fTop = m_pos.y + (m_size.y / 2);		//自分の上端
	float fBottom = m_pos.y - (m_size.y / 2);	//自分の下端
	float fFront = m_pos.z - (m_size.z / 2);	//自分の前端
	float fBack = m_pos.z + (m_size.z / 2);		//自分の後端

	//------------------------------------
	// 相手の行列を元に戻す
	//------------------------------------
	/*D3DXVec3TransformCoord(&worldPos, &targetPos, &targetMtx);
	D3DXMatrixInverse(&invMtxWorld, NULL, &targetMtx);
	D3DXVec3TransformCoord(&localPos, &worldPos, &invMtxWorld);*/

	//------------------------------------
	// 相手の端の設定
	//------------------------------------
	float fTargetLeft = targetPos.x + targetSize.x;		//相手の左端
	float fTargetRight = targetPos.x - targetSize.x;	//相手の右端
	float fTargetTop = targetPos.y + targetSize.y;		//相手の上端
	float fTargetBottom = targetPos.y - targetSize.y;	//相手の下端
	float fTargetFront = targetPos.z + targetSize.z;	//相手の前端
	float fTargetBack = targetPos.z - targetSize.z;		//相手の後端

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
			if (fTargetFront >= fFront && fTargetFront < m_posOld.z - (m_size.z / 2))
			{
				return COLLISION_FRONT;
			}
			else if (fTargetBack <= fBack && fTargetBack > m_posOld.z - (m_size.z / 2))
			{
				return COLLISION_BACK;
			}
		}
		//---------------------------------
		// 左右の当たり判定
		//---------------------------------
		if (fTargetFront >= fFront && fTargetBack <= fBack)
		{//前後の範囲内なら
			if (fTargetLeft >= fLeft && fTargetLeft < m_posOld.x + (m_size.x / 2))
			{
				return COLLISION_LEFT;
			}
			else if (fTargetRight <= fRight && fTargetRight > m_posOld.x - (m_size.x / 2))
			{
				return COLLISION_RIGHT;
			}
		}
	}

	return COLLISION_NONE;
}

//=============================================================================
// 押し戻された位置を返す処理
// 引数：位置、前の位置、大きさ、マトリックス、相手の種類
//=============================================================================
D3DXVECTOR3 CUtility::GetCollisionPos(D3DXVECTOR3 pos, D3DXVECTOR3 posOld
	, D3DXVECTOR3 size, D3DXMATRIX mtx, CObject::EObjType targetType)
{
	//引数に代入
	m_pos = pos;		//位置
	m_posOld = posOld;	//前の位置
	m_size = size;		//大きさ
	m_mtxWorld = mtx;	//ワールドマトリックス

	//変数宣言
	D3DXVECTOR3 targetPos;		//相手の位置
	D3DXVECTOR3 targetSize;		//相手の大きさ
	D3DXMATRIX targetMtx;		//相手のマトリックス

	//--------------------------
	// 相手の情報を取得
	//--------------------------
	for (int i = 0; i < CObject::GetMaxPriolity(); i++)
	{//プライオリティ分回す
		CObject* pObj;
		pObj = CObject::GETObject(targetType, i);

		if (pObj == nullptr)
		{//nullなら戻す
			continue;
		}

		//---------------------------
		// オブジェクトの種類の取得
		//---------------------------
		CObject::EObjType type = pObj->GetObjType();
		if (type != targetType)
		{//オブジェクトの種類が目的の相手じゃないなら
			continue;
		}

		//-----------------------------
		// 相手の種類ごとの処理
		//-----------------------------
		if (targetType == CObject::OBJTYPE_PLAYER)
		{//相手がプレイヤーなら
			//ダウンキャスト
			CPlayer* pPlayer = (CPlayer*)pObj;

			//情報の取得
			targetPos = pPlayer->GetPosition();
			targetSize = pPlayer->GetSize();
			targetMtx = pPlayer->GetmtxWorld();
		}
		else if (targetType == CObject::OBJTYPE_ENEMY)
		{//相手が敵なら
			//ダウンキャスト
			CEnemy* pEnemy = (CEnemy*)pObj;

			//情報の取得
			targetPos = pEnemy->GetPosition();
			targetSize = pEnemy->GetSize();
			targetMtx = pEnemy->GetmtxWorld();
		}

		//--------------------------
		// 当たり判定の処理
		//--------------------------
		CUtility::COLLISION direction = CUtility::Collision(targetPos, targetSize, targetMtx);

		//--------------------------
		// 当たった方向に応じた処理
		//--------------------------
		switch (direction)
		{
		case CUtility::COLLISION_FRONT:
			m_pos.z = targetPos.z + targetSize.z + (size.z / 2);
			break;

		case CUtility::COLLISION_BACK:
			m_pos.z = targetPos.z - targetSize.z - (size.z / 2);
			break;

		case CUtility::COLLISION_LEFT:
			m_pos.x = targetPos.x + targetSize.x + (size.x / 2);
			break;

		case CUtility::COLLISION_RIGHT:
			m_pos.x = targetPos.x - targetSize.x - (size.x / 2);
			break;

		default:
			break;
		}
	}

	return m_pos;
}

//=============================================================================
// モデルの先の当たり判定(球)
// 引数：オフセット座標、球の直径、ワールドマトリックス、相手の種類
//==============================================================================
bool CUtility::ColliaionWeapon(D3DXVECTOR3 offset,float fDiameter,
								D3DXMATRIX mtxWorld, CObject::EObjType targetType)
{
	//変数宣言
	D3DXVECTOR3 worldPos(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 targetPos(0.0f, 0.0f, 0.0f);

	//剣先までのオフセットを加算した位置を取得
	D3DXVec3TransformCoord(&worldPos, &offset, &mtxWorld);

	//オフセット先にプレイヤーを移動
	//CGame::GetPlayer()->SetPosition(worldPos);

	//--------------------------------------
	// モデル先を中心とした球の当たり判定
	//--------------------------------------
	//相手の情報を取得
	for (int i = 0; i < CObject::GetMaxPriolity(); i++)
	{//プライオリティ分回す
		CObject* pObj;
		pObj = CObject::GETObject(targetType, i);

		if (pObj == nullptr)
		{//nullなら戻す
			continue;
		}

		//---------------------------
		// オブジェクトの種類の取得
		//---------------------------
		CObject::EObjType type = pObj->GetObjType();
		if (type != targetType)
		{//オブジェクトの種類が目的の相手じゃないなら
			continue;
		}

		//-----------------------------
		// 相手の種類ごとの処理
		//-----------------------------
		if (targetType == CObject::OBJTYPE_PLAYER)
		{//相手がプレイヤーなら
			//情報の取得
			targetPos = CGame::GetPlayer()->GetPosition();
		}
		else if (targetType == CObject::OBJTYPE_ENEMY)
		{//相手が敵なら
			targetPos = CGame::GetEnemy()->GetPosition();
		}

		//2点間の距離を求める
		D3DXVECTOR3 distance = worldPos - targetPos;

		//当たり判定								//球の範囲(半径+半径)
		if (sqrtf(D3DXVec3Dot(&distance, &distance)) < fDiameter)
		{
			return true;
		}
	}

	return false;
}

//=============================================================================
// 角度の正規化
// 引数：角度
//==============================================================================
float CUtility::GetNorRot(float rot)
{
	float rotData = rot;

	if (rotData > D3DX_PI)
	{//角度が180以上なら
		rotData -= D3DX_PI * 2;
	}
	else if (rotData < -D3DX_PI)
	{//角度が-180以下なら
		rotData += D3DX_PI * 2;
	}

	return rotData;
}

//=============================================================================
// 移動限界の設定
// 引数：位置
//==============================================================================
D3DXVECTOR3 CUtility::LimitMove(D3DXVECTOR3 pos)
{
	//---------------------
	// 移動の限界値
	//---------------------
	float fLeft = -1000.0f;
	float fRight = 1000.0f;
	float fFlont = -1000.0f;
	float fBack = 1000.0f;

	//---------------------
	// X軸の限界
	//---------------------
	if (pos.x <= fLeft)
	{
		pos.x = fLeft;
	}
	else if (pos.x >= fRight)
	{
		pos.x = fRight;
	}

	//---------------------
	// Z軸の限界
	//---------------------
	if (pos.z <= fFlont)
	{
		pos.z = fFlont;
	}
	else if (pos.z >= fBack)
	{
		pos.z = fBack;
	}

	return pos;
}