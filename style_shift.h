//===================================================
//
// 戦闘スタイル表示ヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _STYLE_SHIFT_H_
#define _STYLE_SHIFT_H_	//二重インクルード防止

//--------------------------------
// インクルード
//--------------------------------
#include "billboard.h"

//================================
// スタイルシフトクラスの定義
//================================
class CStyleShift : public CBillBoard
{
public:
	CStyleShift();				//コンストラクタ
	~CStyleShift() override;	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//------------------
	// セッター
	//------------------
	void SetStyle(bool flash);	//スタイルの状態の設定

	//------------------
	// 静的メンバ関数
	//------------------
	static CStyleShift* Create(D3DXVECTOR3 pos);

private:
	//------------------
	// メンバ変数
	//------------------
	bool m_bFlash;	//明るくするか
};

#endif