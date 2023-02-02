//===================================================
//
// ステージヘッダー
// Author:Teruto Sato
//
//===================================================
#ifndef _STAGE_H_
#define _STAGE_H_	//二重インクルード防止

//--------------------------------
// インクルード
//--------------------------------
#include "objectX.h"

//================================
// ステージクラスの定義
//================================
class CStage : public CObjectX
{
public:
	CStage();	//コンストラクタ
	~CStage();	//デストラクタ

	//----------------
	// メンバ関数
	//----------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//----------------
	// 静的メンバ関数
	//----------------
	static CStage* Create(LPCTSTR text, D3DXVECTOR3 pos);

private:
	LPCTSTR m_text;	//モデルのパス
};

#endif