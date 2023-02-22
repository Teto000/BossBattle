//===================================================
//
// ランキングヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _RANKING_H_
#define _RANKING_H_	//二重インクルード防止

//-------------------------------
// インクルード
//-------------------------------
#include "object.h"

//-------------------------------
// 前方宣言
//-------------------------------
class CTime;	//時間

//================================
// ランキングクラス
//================================
class CRanking : CObject
{
public:
	CRanking();	//コンストラクタ
	~CRanking();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();

	void Save();
	void Load();
	void Ranking();

	//----------------
	// セッター
	//----------------
	static void SetNewTime(int nTime) { m_nTime = nTime; }	//新しい時間を設定

	//----------------
	// ゲッター
	//----------------
	D3DXVECTOR3 GetPosition() override { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	//位置の取得
	float GetWidth() override { return 0.0f; }				//幅の取得
	float GetHeight() override { return 0.0f; }				//高さの取得

	//------------------
	// 静的メンバ変数
	//------------------
	static CRanking* Create();

private:
	//------------------
	// 定数
	//------------------
	static const int nMaxRanking = 5;	//表示するランキングの最大数

	//------------------
	// メンバ変数
	//------------------
	int	m_nRankUpdate;				//更新ランクNo.
	CTime*	m_pTime[nMaxRanking];	//時間

	//------------------
	// 静的メンバ変数
	//------------------
	static int m_nTime;	//時間
};

#endif