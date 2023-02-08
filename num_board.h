//===================================================
//
// 数値表示(ビルボード)ヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _NUM_BOARD_H_
#define _NUM_BOARD_H_	//二重インクルード防止

//-------------------------------
// インクルード
//-------------------------------
#include "billboard.h"

//================================
// ナンバークラス
//================================
class CNumBoard : public CBillBoard
{
public:
	CNumBoard();				//コンストラクタ
	~CNumBoard() override;	//デストラクタ

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
	void Set(int nDigit);	//数値の設定

	//------------------
	// 静的メンバ変数
	//------------------
	static CNumBoard* Create(D3DXVECTOR3 pos, int nValue);

private:
	//------------------
	// メンバ変数
	//------------------
	int m_nValue;	//数値
	int m_nDigit;	//桁数
};

#endif