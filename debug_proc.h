//===================================================
//
// デバッグヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _DEBUG_PROC_H_
#define _DEBUG_PROC_H_	

//--------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>
#include <string>

//================================
// デバッグクラスの定義
//================================
class CDebugProc
{
public:
	CDebugProc();	//コンストラクタ
	~CDebugProc();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init();
	void Uninit();

	//------------------
	// 静的メンバ関数
	//------------------
	static void Print(const char* pFormat, ...);
	static void Draw();

private:
	//------------------
	// 定数
	//------------------
	static const int nMaxString = 1024;	//文字列の最大数

	//------------------
	// 静的メンバ変数
	//------------------
	static LPD3DXFONT m_pFont;
	static std::string m_Str;
	static int m_nCntData;
};

#endif