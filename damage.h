//===================================================
//
// ダメージヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _DAMAGE_H_
#define _DAMAGE_H_	//二重インクルード防止

//-------------------------------
// インクルード
//-------------------------------
#include "billboard.h"

//================================
// ダメージクラス
//================================
class CDamage : CBillBoard
{
public:
	CDamage();	//コンストラクタ
	~CDamage();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();

	//----------------
	// ゲッター
	//----------------
	D3DXVECTOR3 GetPosition() override { return m_pos; }	//位置の取得
	float GetWidth() override { return 0.0f; }				//幅の取得
	float GetHeight() override { return 0.0f; }				//高さの取得

	//------------------
	// 静的メンバ変数
	//------------------
	static CDamage* Create(D3DXVECTOR3 pos, int nNum);

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
	int m_nAtkValue;				//攻撃力の値
	int m_aPosTexU[nMaxDigits];		//今の桁の数値
	int m_nDeleteTime;				//消えるまでの時間
};

#endif