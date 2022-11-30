//===================================================
//
// キーボードヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _INPUT_KEYBORD_H_
#define _INPUT_KEYBOARD_H_	

//--------------------------------
// ライブラリのリンク 
//--------------------------------
#pragma comment(lib,"dxguid.lib")		//DirectXコンポーネント(部品)に使用
#pragma comment(lib,"dinput8.lib")		//入力処理に必要 (キーボード)

//--------------------------------
// インクルード
//--------------------------------
#define DIRECTINPUT_VERSION (0x0800)		//ビルド時の警告対処用マクロ
#include <dinput.h>

//--------------------------------
// マクロ定義
//--------------------------------
#define NUM_KEY_MAX		(256)	// キーの最大数（キーボード）

//================================
// キーボードクラスの定義
//================================
class CInputKeyboard
{
public:
	//--------------------------------------------------
	// キーボードの構造体
	//--------------------------------------------------
	typedef struct
	{
		LPDIRECTINPUT8			pInput;							// DirectInputオブジェクトへのポインタ
		LPDIRECTINPUTDEVICE8	pDevKeyboard;					// 入力デバイス(キーボード)へのポインタ
		BYTE					aKeyState[NUM_KEY_MAX];			// キーボードのプレス情報
		BYTE					aKeyStateTrigger[NUM_KEY_MAX];	// キーボードのトリガー情報
		BYTE					aKeyStateRelease[NUM_KEY_MAX];	// キーボードのリリース情報
	}Keyboard;

	CInputKeyboard();	//コンストラクタ
	~CInputKeyboard();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新

	//------------------
	// 静的メンバ関数
	//------------------
	static bool AllPress(void);		// 全キープレス処理
	static bool Press(int nKey);	// プレス処理
	static bool AllTrigger(void);	// 全キートリガー処理
	static bool Trigger(int nKey);	// トリガー処理
	static bool AllRelease(void);	// 全リリース処理
	static bool Release(int nKey);	// リリース処理

private:
	//------------------
	// 静的メンバ変数
	//------------------
	static Keyboard m_Keyboard;	//構造体
};

#endif // !_INPUT_KEYBORD_H_