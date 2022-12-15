//===================================================
//
// HPヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _HP_H_
#define _HP_H_	//二重インクルード防止

//--------------------------------
// インクルード
//--------------------------------
#include "gauge.h"

//--------------------------------
// HPクラス
//--------------------------------
class CHP : public CGauge
{
public:
	CHP();				//コンストラクタ
	~CHP() override;	//デストラクタ

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
	static CHP* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
};

#endif