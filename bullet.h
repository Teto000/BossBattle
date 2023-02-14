//=============================================
//
// バレットヘッダー
// Author : Sato Teruto
//
//=============================================
#ifndef _BULLET_H_
#define _BULLET_H_

//-------------------------
// インクルード
//-------------------------
#include "billboard.h"

//====================================
// バレットクラス
//====================================
class CBullet : public CBillBoard
{
public:
	CBullet();
	explicit CBullet(int nPriority);	//コンストラクタ
	virtual ~CBullet() override;		//デストラクタ

	//---------------------
	// メンバ関数
	//---------------------
	virtual HRESULT Init(D3DXVECTOR3 pos) override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	//---------------------
	// 静的メンバ関数
	//---------------------
	static CBullet* CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 playerRot);
};

#endif // !_BULLET_H_
