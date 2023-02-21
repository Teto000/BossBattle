//===================================================
//
// ジョイパッドの処理
// Author : Koduna Hirohito
// Author : Sato Teruto
//
//===================================================

//---------------------------
// インクルード
//---------------------------
#include <assert.h>
#include <d3dx9.h>
#include "input_joypad.h"

//---------------------------
// 静的メンバ変数宣言
//---------------------------
CInputJoypad::Joypad CInputJoypad::m_Joypad = {};


//===========================================
// コンストラクタ
//===========================================
CInputJoypad::CInputJoypad()
{
	memset(&m_Joypad, 0, sizeof(m_Joypad));
}

//===========================================
// デストラクタ
//===========================================
CInputJoypad::~CInputJoypad()
{

}


//===========================================
// ジョイパッドの初期化処理
//===========================================
HRESULT CInputJoypad::Init()
{
	// XInputのステートを設定（有効にする）
	XInputEnable(true);

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		// メモリーのクリア
		memset(&m_Joypad.JoyKeyState[nCnt], 0, sizeof(XINPUT_STATE));
		memset(&m_Joypad.JoyKeyStateTrigger[nCnt], 0, sizeof(XINPUT_STATE));

		// ジョイパッドの振動制御の０クリア
		ZeroMemory(&m_Joypad.JoyMoter[nCnt], sizeof(XINPUT_VIBRATION));

		// 振動制御用の初期化
		m_Joypad.nStrength[nCnt] = 0;
		m_Joypad.nTime[nCnt] = 0;
	}

	return S_OK;
}

//===========================================
// ジョイパッドの終了処理
//===========================================
void CInputJoypad::Uninit(void)
{
	// XInputのステートを設定（無効にする）
	XInputEnable(false);
}

//===========================================
// ジョイパッドの更新処理
//===========================================
void CInputJoypad::Update(void)
{
	XINPUT_STATE JoyKeyState;		// ジョイパッド入力情報

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		// ジョイパッドの状態を取得
		if (XInputGetState(nCnt, &JoyKeyState) == ERROR_SUCCESS)
		{
			// トリガー情報を保存
			m_Joypad.JoyKeyStateTrigger[nCnt].Gamepad.wButtons = ~m_Joypad.JoyKeyState[nCnt].Gamepad.wButtons & JoyKeyState.Gamepad.wButtons;

			// リリース情報を保存
			m_Joypad.JoyKeyStateRelease[nCnt].Gamepad.wButtons = m_Joypad.JoyKeyState[nCnt].Gamepad.wButtons & ~JoyKeyState.Gamepad.wButtons;

			// プレス情報を保存
			m_Joypad.JoyKeyState[nCnt] = JoyKeyState;

			// スティックのトリガーの更新
			UpdateStickTrigger(JOYKEY_LEFT_STICK, JOYKEY_UP, nCnt);
			UpdateStickTrigger(JOYKEY_LEFT_STICK, JOYKEY_DOWN, nCnt);
			UpdateStickTrigger(JOYKEY_LEFT_STICK, JOYKEY_LEFT, nCnt);
			UpdateStickTrigger(JOYKEY_LEFT_STICK, JOYKEY_RIGHT, nCnt);
			UpdateStickTrigger(JOYKEY_RIGHT_STICK, JOYKEY_UP, nCnt);
			UpdateStickTrigger(JOYKEY_RIGHT_STICK, JOYKEY_DOWN, nCnt);
			UpdateStickTrigger(JOYKEY_RIGHT_STICK, JOYKEY_LEFT, nCnt);
			UpdateStickTrigger(JOYKEY_RIGHT_STICK, JOYKEY_RIGHT, nCnt);

			m_Joypad.bUseJoyPad[nCnt] = true; // 使用状況の更新
		}
		else
		{
			m_Joypad.bUseJoyPad[nCnt] = false;	// 使用状況の更新
		}

		// ジョイパッドの振動
		m_Joypad.JoyMoter[nCnt].wLeftMotorSpeed = m_Joypad.nStrength[nCnt];
		m_Joypad.JoyMoter[nCnt].wRightMotorSpeed = m_Joypad.nStrength[nCnt];
		XInputSetState(nCnt, &m_Joypad.JoyMoter[nCnt]);

		if (m_Joypad.nTime[nCnt] > 0)
		{
			m_Joypad.nTime[nCnt]--;
		}
		else
		{
			m_Joypad.nStrength[nCnt] = 0;
			m_Joypad.nTime[nCnt] = 0;
		}
	}
}

//===========================================
// スティックのトリガーの更新
//===========================================
void CInputJoypad::UpdateStickTrigger(CInputJoypad::JOYKEY Stick, CInputJoypad::JOYKEY Key, int nPlayer)
{
	D3DXVECTOR3 stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fDeadZone = 0.0f;
	int nStick = 0;

	switch (Stick)
	{// スティックの種類
	case CInputJoypad::JOYKEY_LEFT_STICK:
		stick = D3DXVECTOR3((float)(m_Joypad.JoyKeyState[nPlayer].Gamepad.sThumbLX), (float)(-m_Joypad.JoyKeyState[nPlayer].Gamepad.sThumbLY), 0.0f);
		fDeadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		nStick = 0;
		break;

	case CInputJoypad::JOYKEY_RIGHT_STICK:
		stick = D3DXVECTOR3((float)(m_Joypad.JoyKeyState[nPlayer].Gamepad.sThumbRX), (float)(-m_Joypad.JoyKeyState[nPlayer].Gamepad.sThumbRY), 0.0f);
		fDeadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		nStick = 1;
		break;

	default:
		assert(false);
		break;
	}

	switch (Key)
	{// 傾ける方向
	case CInputJoypad::JOYKEY_UP:		// 上
		if (m_Joypad.bLean[nPlayer][nStick][0])
		{// 傾いた
			m_Joypad.bStick[nPlayer][nStick][0] = true;
			m_Joypad.bLean[nPlayer][nStick][0] = false;
		}
		else
		{// 傾いてない
			if (stick.y >= -fDeadZone)
			{// 傾いてない
				m_Joypad.bStick[nPlayer][nStick][0] = false;
			}
		}
		break;

	case CInputJoypad::JOYKEY_DOWN:	// 下
		if (m_Joypad.bLean[nPlayer][nStick][1])
		{// 傾いた
			m_Joypad.bStick[nPlayer][nStick][1] = true;
			m_Joypad.bLean[nPlayer][nStick][1] = false;
		}
		else
		{// 傾いてない
			if (stick.y <= fDeadZone)
			{// 傾いてない
				m_Joypad.bStick[nPlayer][nStick][1] = false;
			}
		}
		break;

	case CInputJoypad::JOYKEY_LEFT:	// 左
		if (m_Joypad.bLean[nPlayer][nStick][2])
		{// 傾いた
			m_Joypad.bStick[nPlayer][nStick][2] = true;
			m_Joypad.bLean[nPlayer][nStick][2] = false;
		}
		else
		{// 傾いてない
			if (stick.x >= -fDeadZone)
			{// 傾いてない
				m_Joypad.bStick[nPlayer][nStick][2] = false;
			}
		}
		break;

	case CInputJoypad::JOYKEY_RIGHT:	// 右
		if (m_Joypad.bLean[nPlayer][nStick][3])
		{// 傾いた
			m_Joypad.bStick[nPlayer][nStick][3] = true;
			m_Joypad.bLean[nPlayer][nStick][3] = false;
		}
		else
		{// 傾いてない
			if (stick.x <= fDeadZone)
			{// 傾いてない
				m_Joypad.bStick[nPlayer][nStick][3] = false;
			}
		}
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// ジョイパッドのプレス処理(プレイヤー指定あり)
//--------------------------------------------------
bool CInputJoypad::IdxPress(CInputJoypad::JOYKEY Key, int nPlayer)
{
	return (m_Joypad.JoyKeyState[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//--------------------------------------------------
// ジョイパッドのトリガー処理(プレイヤー指定あり)
//--------------------------------------------------
bool CInputJoypad::IdxTrigger(CInputJoypad::JOYKEY Key, int nPlayer)
{
	bool a = (m_Joypad.JoyKeyStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
	return a;
}

//--------------------------------------------------
// ジョイパッドのリリース処理(プレイヤー指定あり)
//--------------------------------------------------
bool CInputJoypad::IdxRelease(CInputJoypad::JOYKEY Key, int nPlayer)
{
	bool a = (m_Joypad.JoyKeyStateRelease[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
	return a;
}

//--------------------------------------------------
// ジョイパッドのプレス処理(プレイヤー指定なし)
//--------------------------------------------------
bool CInputJoypad::Press(CInputJoypad::JOYKEY Key)
{
	for (int nPlayer = 0; nPlayer < PLAYER_MAX; nPlayer++)
	{
		if (CInputJoypad::IdxPress(Key, nPlayer))
		{
			return true;
		}
	}
	return false;
}

//--------------------------------------------------
// ジョイパッドのトリガー処理(プレイヤー指定なし)
//--------------------------------------------------
bool CInputJoypad::Trigger(CInputJoypad::JOYKEY Key)
{
	for (int nPlayer = 0; nPlayer < PLAYER_MAX; nPlayer++)
	{
		if (CInputJoypad::IdxTrigger((CInputJoypad::JOYKEY)Key, nPlayer))
		{
			return true;
		}
	}
	return false;
}

//--------------------------------------------------
// ジョイパッドのリリース処理(プレイヤー指定なし)
//--------------------------------------------------
bool CInputJoypad::Release(CInputJoypad::JOYKEY Key)
{
	for (int nPlayer = 0; nPlayer < PLAYER_MAX; nPlayer++)
	{
		if (CInputJoypad::IdxRelease((CInputJoypad::JOYKEY)Key, nPlayer))
		{
			return true;
		}
	}
	return false;
}

//--------------------------------------------------
// ジョイパッドの全キープレス処理
//--------------------------------------------------
bool CInputJoypad::AllPress(void)
{
	for (int nPlayer = 0; nPlayer < PLAYER_MAX; nPlayer++)
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			if (CInputJoypad::IdxPress((CInputJoypad::JOYKEY)nCntKey, nPlayer))
			{
				return true;
			}
		}
	}
	return false;
}

//--------------------------------------------------
// ジョイパッドの全キートリガー処理
//--------------------------------------------------
bool CInputJoypad::AllTrigger(void)
{
	for (int nPlayer = 0; nPlayer < PLAYER_MAX; nPlayer++)
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			if (CInputJoypad::IdxTrigger((CInputJoypad::JOYKEY)nCntKey, nPlayer))
			{
				return true;
			}
		}
	}
	return false;
}


//--------------------------------------------------
// ジョイパット（スティックプレス）処理
//--------------------------------------------------
D3DXVECTOR3 CInputJoypad::Stick(CInputJoypad::JOYKEY Key, int nPlayer)
{
	switch (Key)
	{
	case CInputJoypad::JOYKEY_LEFT_STICK:
		m_Joypad.JoyStickPos[nPlayer] = D3DXVECTOR3(m_Joypad.JoyKeyState[nPlayer].Gamepad.sThumbLX / 32767.0f, -m_Joypad.JoyKeyState[nPlayer].Gamepad.sThumbLY / 32767.0f, 0.0f);
		break;
	case CInputJoypad::JOYKEY_RIGHT_STICK:
		m_Joypad.JoyStickPos[nPlayer] = D3DXVECTOR3(m_Joypad.JoyKeyState[nPlayer].Gamepad.sThumbRX / 32767.0f, -m_Joypad.JoyKeyState[nPlayer].Gamepad.sThumbRY / 32767.0f, 0.0f);
		break;
	}

	return m_Joypad.JoyStickPos[nPlayer];
}

//--------------------------------------------------
// ジョイパット（トリガーペダル）処理
//--------------------------------------------------
int CInputJoypad::TriggerPedal(CInputJoypad::JOYKEY Key, int nPlayer)
{
	int nJoypadTriggerPedal = 0;

	switch (Key)
	{
	case CInputJoypad::JOYKEY_LEFT_TRIGGER:
		nJoypadTriggerPedal = m_Joypad.JoyKeyState[nPlayer].Gamepad.bLeftTrigger;
		break;
	case CInputJoypad::JOYKEY_RIGHT_TRIGGER:
		nJoypadTriggerPedal = m_Joypad.JoyKeyState[nPlayer].Gamepad.bRightTrigger;
		break;
	}

	return nJoypadTriggerPedal;
}

//--------------------------------------------------
// コントローラーの振動制御
//--------------------------------------------------
void CInputJoypad::Vibration(int nTime, WORD nStrength, int nPlayer)
{
	m_Joypad.nTime[nPlayer] = nTime;				// 振動持続時間
	m_Joypad.nStrength[nPlayer] = nStrength;		// 振動の強さ
}

//--------------------------------------------------
// ジョイパッドの使用されているか返す処理
//--------------------------------------------------
bool CInputJoypad::IsJoyPadUse(int nPlayer)
{
	return m_Joypad.bUseJoyPad[nPlayer];
}

//--------------------------------------------------
// ジョイパッドのスティックのトリガー処理(プレイヤー指定なし)
//--------------------------------------------------
bool CInputJoypad::StickTrigger(CInputJoypad::JOYKEY Stick, CInputJoypad::JOYKEY Key)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (IdxStickTrigger(Stick, Key, i))
		{// 誰かが指定のスティックを指定の方向に倒した
			return true;
		}
	}

	return false;
}

//--------------------------------------------------
// ジョイパッドのスティックのトリガー処理(プレイヤー指定あり)
//--------------------------------------------------
bool CInputJoypad::IdxStickTrigger(CInputJoypad::JOYKEY Stick, CInputJoypad::JOYKEY Key, int nPlayer)
{
	D3DXVECTOR3 stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fDeadZone = 0.0f;
	int nStick = 0;

	switch (Stick)
	{// スティックの種類
	case CInputJoypad::JOYKEY_LEFT_STICK:
		stick = D3DXVECTOR3((float)(m_Joypad.JoyKeyState[nPlayer].Gamepad.sThumbLX), (float)(-m_Joypad.JoyKeyState[nPlayer].Gamepad.sThumbLY), 0.0f);
		fDeadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		nStick = 0;
		break;

	case CInputJoypad::JOYKEY_RIGHT_STICK:
		stick = D3DXVECTOR3((float)(m_Joypad.JoyKeyState[nPlayer].Gamepad.sThumbRX), (float)(-m_Joypad.JoyKeyState[nPlayer].Gamepad.sThumbRY), 0.0f);
		fDeadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		nStick = 1;
		break;

	default:
		assert(false);
		break;
	}

	switch (Key)
	{// 傾ける方向
	case CInputJoypad::JOYKEY_UP:		// 上
		if (!m_Joypad.bStick[nPlayer][nStick][0])
		{// 左スティックが傾いてない
			if (stick.y < -fDeadZone)
			{// 左スティックが傾いた
				m_Joypad.bLean[nPlayer][nStick][0] = true;
				return true;
			}
		}
		break;

	case CInputJoypad::JOYKEY_DOWN:	// 下
		if (!m_Joypad.bStick[nPlayer][nStick][1])
		{// 左スティックが傾いてない
			if (stick.y > fDeadZone)
			{// 左スティックが傾いた
				m_Joypad.bLean[nPlayer][nStick][1] = true;
				return true;
			}
		}
		break;

	case CInputJoypad::JOYKEY_LEFT:	// 左
		if (!m_Joypad.bStick[nPlayer][nStick][2])
		{// 左スティックが傾いてない
			if (stick.x < -fDeadZone)
			{// 左スティックが傾いた
				m_Joypad.bLean[nPlayer][nStick][2] = true;
				return true;
			}
		}
		break;

	case CInputJoypad::JOYKEY_RIGHT:	// 右
		if (!m_Joypad.bStick[nPlayer][nStick][3])
		{// 左スティックが傾いてない
			if (stick.x > fDeadZone)
			{// 左スティックが傾いた
				m_Joypad.bLean[nPlayer][nStick][3] = true;
				return true;
			}
		}
		break;

	default:
		assert(false);
		break;
	}

	return false;
}