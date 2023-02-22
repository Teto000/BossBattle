//===================================================
//
// タイマーヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _TIME_H_
#define _TIME_H_	//二重インクルード防止

//-------------------------------
// インクルード
//-------------------------------
#include "object.h"

//-------------------------------
// 前方宣言
//-------------------------------
class CNumber;	//数値

//================================
// タイマークラス
//================================
class CTime : CObject
{
public:
	CTime();	//コンストラクタ
	~CTime();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();

	//----------------
	// セッター
	//----------------
	void SetTime(int nTime);		//時間の設定
	void SetColor(D3DXCOLOR col);	//色の設定

	//----------------
	// ゲッター
	//----------------
	D3DXVECTOR3 GetPosition() override { return m_pos; }	//位置の取得
	float GetWidth() override { return 0.0f; }				//幅の取得
	float GetHeight() override { return 0.0f; }				//高さの取得
	int GetTime() { return m_nTime; }		//時間の取得

	//------------------
	// 静的メンバ変数
	//------------------
	static CTime* Create(D3DXVECTOR3 pos);

private:
	void SetNumber();			//数値の設定

private:
	//------------------
	// 定数
	//------------------
	static const int nMaxDigits = 3;	//最大桁数

	//------------------
	// メンバ変数
	//------------------
	D3DXVECTOR3 m_pos;				//位置
	D3DXVECTOR3 m_numberPos;		//数字の位置
	int m_nTime;					//時間
	int m_nCntMove;					//移動までの時間
	int m_nCntFream;				//フレーム数のカウント
	int m_aPosTexU[nMaxDigits];		//今の桁の数値
	float fInterval;				//数値の間隔
	CNumber* m_pNumber[nMaxDigits];	//数値
};

#endif