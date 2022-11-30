//===================================================
//
// 入力処理
// Author : Sato Teruto
//
//===================================================

//------------------------
// インクルード
//------------------------
#include <assert.h>
#include "input.h"
#include "input_keyboard.h"
#include "input_joypad.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
CInputKeyboard *CInput::m_pInputKeybord = nullptr;	//キーボード
CInputJoypad *CInput::m_pInputJoypad = nullptr;		//ジョイパッド

//===========================
// コンストラクタ
//===========================
CInput::CInput()
{

}

//===========================
// デストラクタ
//===========================
CInput::~CInput()
{

}

//===========================
// 全ての入力の初期化
//===========================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// キーボードの初期化処理
	if (FAILED(m_pInputKeybord->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// ジョイパッド初期化
	m_pInputJoypad->Init();

	return S_OK;
}

//===========================
// 全ての入力の終了
//===========================
void CInput::Uninit()
{
	// キーボードの終了処理
	m_pInputKeybord->Uninit();

	// ジョイパッド終了処理
	m_pInputJoypad->Uninit();
}

//===========================
// 全ての入力の更新
//===========================
void CInput::Update()
{
	// キーボードの更新処理
	m_pInputKeybord->Update();

	// ジョイパッド更新処理
	m_pInputJoypad->Update();
}