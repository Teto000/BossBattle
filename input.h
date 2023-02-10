//===================================================
//
// インプットヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _INPUT_H_
#define _INPUT_H_	

//--------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>
#include "main.h"

//--------------------------------
// 前方宣言
//--------------------------------
class CInputKeyboard;	//キーボード
class CInputJoypad;		//ジョイパッド

//--------------------------------
// マクロ定義
//--------------------------------
#define NUM_KEY_MAX		(256)	// キーの最大数（キーボード）

//================================
// インプットクラスの定義
//================================
class CInput
{
public:
	CInput();	//コンストラクタ
	~CInput();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//入力処理全部の初期化
	void Uninit(void);								//入力処理全部の終了処理
	void Update(void);								//入力処理全部の更新処理

	static CInputJoypad* GetJoypad() { return m_pInputJoypad; }

private:
	//------------------
	// 静的メンバ変数
	//------------------
	static CInputKeyboard *m_pInputKeybord;	//キーボード
	static CInputJoypad *m_pInputJoypad;	//ジョイパッド
};

#endif // !_INPUT_H_