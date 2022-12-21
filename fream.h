//===================================================
//
// ゲージフレームヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _FREAM_H_
#define _FREAM_H_	//二重インクルード防止

//--------------------------------
// インクルード
//--------------------------------
#include "object2D.h"

//--------------------------------
// フレームクラス
//--------------------------------
class CFream : public CObject2D
{
public:
	CFream();				//コンストラクタ
	~CFream() override;		//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//------------------
	// 静的メンバ関数
	//------------------
	static CFream* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
};

#endif