//===================================================
//
// 便利なやつヘッダー
// Author:Teruto Sato
//
//===================================================
#ifndef _UTILITY_H_
#define _UTILITY_H_	//二重インクルード防止

//--------------------------
// インクルード
//--------------------------
#include "object.h"

//================================
// ユーティリティクラスの定義
//================================
class CUtility
{
public:
	enum COLLISION
	{
		COLLISION_NONE = 0,	//当たっていない
		COLLISION_FRONT,	//前から当たった
		COLLISION_BACK,		//後ろから
		COLLISION_LEFT,		//左から
		COLLISION_RIGHT,	//右から
		COLLISION_MAX
	};

	CUtility();		//コンストラクタ
	~CUtility();	//デストラクタ

	//-----------------------
	// 静的メンバ関数
	//-----------------------
	//当たり判定の処理
	static COLLISION Collision(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, D3DXVECTOR3 size, D3DXMATRIX mtx
				, D3DXVECTOR3 targetPos, D3DXVECTOR3 targetSize, D3DXMATRIX targetMtx);

	//押し戻された位置を返す処理
	static D3DXVECTOR3 GetCollisionPos(D3DXVECTOR3 pos, D3DXVECTOR3 posOld
		, D3DXVECTOR3 size, D3DXMATRIX mtx, CObject::EObjType targetType);
};

#endif _UTILITY_H_