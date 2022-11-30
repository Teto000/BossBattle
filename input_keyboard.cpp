//===================================================
//
// キーボードの処理
// Author : Sato Teruto
//
//===================================================

//---------------------------
// インクルード
//---------------------------
#include <d3dx9.h>
#include "input_keyboard.h"

//---------------------------
// 静的メンバ変数宣言
//---------------------------
CInputKeyboard::Keyboard CInputKeyboard::m_Keyboard = {};

//===========================================
// コンストラクタ
//===========================================
CInputKeyboard::CInputKeyboard()
{
	memset(&m_Keyboard, 0, sizeof(m_Keyboard));
}

//===========================================
// デストラクタ
//===========================================
CInputKeyboard::~CInputKeyboard()
{

}

//===========================================
// キーボードの初期化処理
//===========================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_Keyboard.pInput, NULL)))
	{
		return E_FAIL;
	}

	// 入力デバイス（キーボード）の生成
	if (FAILED(m_Keyboard.pInput->CreateDevice(GUID_SysKeyboard, &m_Keyboard.pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_Keyboard.pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_Keyboard.pDevKeyboard->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得
	m_Keyboard.pDevKeyboard->Acquire();

	return S_OK;
}

//===========================================
// キーボードの終了処理
//===========================================
void CInputKeyboard::Uninit(void)
{
	// 入力デバイス（キーボード）の放棄
	if (m_Keyboard.pDevKeyboard != NULL)
	{
		m_Keyboard.pDevKeyboard->Unacquire();		// キーボードへのアクセス権を放棄
		m_Keyboard.pDevKeyboard->Release();
		m_Keyboard.pDevKeyboard = NULL;
	}

	// DirectInputオブジェクトの破壊
	if (m_Keyboard.pInput != NULL)
	{
		m_Keyboard.pInput->Release();
		m_Keyboard.pInput = NULL;
	}
}

//===========================================
// キーボードの更新処理
//===========================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];		// キーボードの入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_Keyboard.pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_Keyboard.aKeyStateTrigger[nCntKey] = ~m_Keyboard.aKeyState[nCntKey] & aKeyState[nCntKey];		// キーボードのトリガー情報を保存
			m_Keyboard.aKeyStateRelease[nCntKey] = m_Keyboard.aKeyState[nCntKey] & ~aKeyState[nCntKey];		// キーボードのリリース情報を保存
			m_Keyboard.aKeyState[nCntKey] = aKeyState[nCntKey];										// キーボードのプレス情報を保存
		}
	}
	else
	{
		m_Keyboard.pDevKeyboard->Acquire();			// キーボードへのアクセス権を獲得
	}

}

//===========================================
// キーボードのプレス情報を取得
//===========================================
bool CInputKeyboard::Press(int nKey)
{
	return (m_Keyboard.aKeyState[nKey] & 0x80) != 0;
}

//===========================================
// キーボードのトリガー情報を取得
//===========================================
bool CInputKeyboard::Trigger(int nKey)
{
	return (m_Keyboard.aKeyStateTrigger[nKey] & 0x80) != 0;
}

//===========================================
// キーボードのリリース情報を取得
//===========================================
bool CInputKeyboard::Release(int nKey)
{
	return (m_Keyboard.aKeyStateRelease[nKey] & 0x80) != 0;
}

//===========================================
// キーボードの全キープレス情報を取得
//===========================================
bool CInputKeyboard::AllPress(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if (CInputKeyboard::Press(nCntKey))
		{
			return true;
		}
	}

	return false;
}

//===========================================
// キーボードの全キートリガー情報を取得
//===========================================
bool CInputKeyboard::AllTrigger(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if (CInputKeyboard::Trigger(nCntKey))
		{
			return true;
		}
	}

	return false;
}

//===========================================
// キーボードの全キーリリース情報を取得
//===========================================
bool CInputKeyboard::AllRelease(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if (CInputKeyboard::Release(nCntKey))
		{
			return true;
		}
	}

	return false;
}