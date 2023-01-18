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
	//当たり判定の情報
	struct CollisionData
	{
		D3DXVECTOR3 pos;		//位置
		D3DXVECTOR3 posOld;		//前の位置
		D3DXVECTOR3 size;		//大きさ
		D3DXMATRIX mtx;			//ワールドマトリックス
	};

	enum COLLISION
	{
		COLLISION_NONE = 0,	//当たっていない
		COLLISION_FRONT,	//前から当たった
		COLLISION_BACK,		//後ろ
		COLLISION_LEFT,		//左
		COLLISION_RIGHT,	//右
		COLLISION_MAX
	};

	CUtility();		//コンストラクタ
	~CUtility();	//デストラクタ

	//-----------------------
	// 静的メンバ関数
	//-----------------------
	static COLLISION Collision(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, D3DXVECTOR3 size, D3DXMATRIX mtx
				, D3DXVECTOR3 targetPos, D3DXVECTOR3 targetSize, D3DXMATRIX targetMtx);
};

#endif _UTILITY_H_