//===================================================
//
// タイトルヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _TITLE_H_
#define _TITLE_H_	

//--------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>

//--------------------------------
// 前方宣言
//--------------------------------
class CBg;	//背景

//================================
// タイトルクラスの定義
//================================
class CTitle
{
public:
	CTitle();	//コンストラクタ
	~CTitle();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init();
	void Uninit();
	void Update();

private:
	//------------------
	// メンバ変数
	//------------------
	CBg* m_pBg;	//背景
};

#endif