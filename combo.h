//===================================================
//
// コンボヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _COMBO_H_
#define _COMBO_H_	//二重インクルード防止

//-------------------------------
// インクルード
//-------------------------------
#include "object.h"

//-------------------------------
// 前方宣言
//-------------------------------
class CNumber;	//数値

//================================
// ナンバークラス
//================================
class CCombo : CObject
{
public:
	CCombo();	//コンストラクタ
	~CCombo();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();

	int AddNumber(int nNum);	//数値の加算

	//----------------
	// ゲッター
	//----------------
	D3DXVECTOR3 GetPosition() override { return m_pos; }	//位置の取得
	float GetWidth() override { return 0.0f; }				//幅の取得
	float GetHeight() override { return 0.0f; }				//高さの取得

	//------------------
	// 静的メンバ変数
	//------------------
	static CCombo* Create(D3DXVECTOR3 pos, int nNum);

private:
	void SetNumber();			//数値の設定

private:
	//------------------
	// 定数
	//------------------
	static const int nMaxDigits = 2;	//最大桁数

	//------------------
	// メンバ変数
	//------------------
	D3DXVECTOR3 m_pos;				//位置
	D3DXCOLOR m_col;				//色
	int m_nNumCombo;				//コンボ数
	int m_aPosTexU[nMaxDigits];		//今の桁の数値
	CNumber* m_pNumber[nMaxDigits];	//数値
};

#endif