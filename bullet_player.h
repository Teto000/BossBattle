//=============================================
//
// バレットヘッダー
// Author : Sato Teruto
//
//=============================================
#ifndef _BULLET_PLAYER_H_
#define _BULLET_PLAYER_H_

//-------------------------
// インクルード
//-------------------------
#include "bullet.h"

//====================================
// バレットクラス
//====================================
class CBulletPlayer : public CBullet
{
public:
	CBulletPlayer();
	~CBulletPlayer() override;		//デストラクタ

	//---------------------
	// メンバ関数
	//---------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//---------------------
	// 静的メンバ関数
	//---------------------
	static CBulletPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 playerRot);
};

#endif
