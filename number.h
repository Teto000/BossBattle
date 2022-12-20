//===================================================
//
// 数値表示用ヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_	//二重インクルード防止

//-------------------------------
// インクルード
//-------------------------------
#include "object2d.h"

//================================
// ナンバークラス
//================================
class CNumber : public CObject2D
{
public:
	CNumber();				//コンストラクタ
	~CNumber() override;	//デストラクタ

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
	static CNumber* Create(D3DXVECTOR3 pos, int nValue);

private:
	//------------------
	// メンバ変数
	//------------------
	int m_nValue;	//数値
	int m_nDigit;	//桁数
};

#endif