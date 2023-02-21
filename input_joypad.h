//===================================================
//
// ジョイパッドヘッダー
// Author: Koduna Hirohito
// Author : Sato Teruto
//
//===================================================
#ifndef _INPUT_JOYPAD_H_
#define _INPUT_JOYPAD_H_	

//--------------------------------
// ライブラリのリンク 
//--------------------------------
#pragma comment(lib,"dxguid.lib")		//DirectXコンポーネント(部品)に使用
#pragma comment(lib,"xinput.lib")		//入力処理に必要（ジョイパット）

//--------------------------------
// インクルード
//--------------------------------
#define DIRECTINPUT_VERSION (0x0800)		//ビルド時の警告対処用マクロ
#include <Xinput.h>		//ジョイパッド処理に必要

//--------------------------------
// マクロ定義
//--------------------------------
#define NUM_KEY_MAX		(256)	// キーの最大数（キーボード）
#define PLAYER_MAX		(4)		// プレイヤーの最大人数
#define DIRECTION_MAX	(4)		// 向きの最大数
#define STICK_MAX		(2)		// スティックの最大数

//================================
// ジョイパッドクラスの定義
//================================
class CInputJoypad
{
public:
	//--------------------------------------------------
	// ジョイパッドの構造体
	//--------------------------------------------------
	typedef struct
	{
		XINPUT_STATE		JoyKeyState[PLAYER_MAX];						// ジョイパッドのプレス情報
		XINPUT_STATE		JoyKeyStateTrigger[PLAYER_MAX];					// ジョイパッドのトリガー情報
		XINPUT_STATE		JoyKeyStateRelease[PLAYER_MAX];					// ジョイパッドのリリース情報
		D3DXVECTOR3			JoyStickPos[PLAYER_MAX];						// ジョイスティックの傾き
		XINPUT_VIBRATION	JoyMoter[PLAYER_MAX];							// ジョイパッドのモーター
		int					nTime[PLAYER_MAX];								// 振動持続時間
		WORD				nStrength[PLAYER_MAX];							// 振動の強さ (0 - 65535)
		bool				bUseJoyPad[PLAYER_MAX];							// ジョイパッドを使用してるか
		bool				bStick[PLAYER_MAX][STICK_MAX][DIRECTION_MAX];	// スティックのトリガー情報
		bool				bLean[PLAYER_MAX][STICK_MAX][DIRECTION_MAX];	// スティックが傾いたかどうか
	}Joypad;

	//--------------------------------------------------
	// 列挙型を定義
	//--------------------------------------------------
	typedef enum
	{
		JOYKEY_UP = 0,				// 十字ボタン上
		JOYKEY_DOWN,				// 十字ボタン下
		JOYKEY_LEFT,				// 十字ボタン左
		JOYKEY_RIGHT,				// 十字ボタン右
		JOYKEY_START,				// スタートボタン
		JOYKEY_BACK,				// バックボタン
		JOYKEY_LEFT_THUMB,			// 左スティック押込み
		JOYKEY_RIGHT_THUMB,			// 右スティック押込み
		JOYKEY_LEFT_SHOULDER,		// L１ボタン
		JOYKEY_RIGHT_SHOULDER,		// R１ボタン
		JOYKEY_LEFT_TRIGGER,		// L２トリガー
		JOYKEY_RIGHT_TRIGGER,		// R２トリガー
		JOYKEY_A,					// Aボタン
		JOYKEY_B,					// Bボタン
		JOYKEY_X,					// Xボタン
		JOYKEY_Y,					// Yボタン
		JOYKEY_LEFT_STICK,			// 左スティック
		JOYKEY_RIGHT_STICK,			// 右スティック
		JOYKEY_MAX,
	}JOYKEY;

	CInputJoypad();		//コンストラクタ
	~CInputJoypad();	//デストラクタ

	//-----------------------------
	// メンバ関数
	//-----------------------------
	HRESULT Init();	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void UpdateStickTrigger(JOYKEY Stick, JOYKEY Key, int nPlayer);	// スティックのトリガーの更新

	//-----------------------------
	// 静的メンバ関数
	//-----------------------------
	bool AllPress(void);											// ジョイパッドプレス処理(キー指定なし)
	bool Press(JOYKEY Key);											// ジョイパッドプレス処理(プレイヤー指定なし)
	bool IdxPress(JOYKEY Key, int nPlayer);							// ジョイパッドのプレス処理(プレイヤー指定あり)
	bool AllTrigger(void);											// ジョイパッドトリガー処理(キー指定なし)
	bool Trigger(JOYKEY Key);										// ジョイパッドトリガー処理(プレイヤー指定なし)
	bool IdxTrigger(JOYKEY Key, int nPlayer);						// ジョイパッドのトリガー処理(プレイヤー指定あり)
	bool Release(JOYKEY Key);										// ジョイパッドリリース処理(プレイヤー指定なし)
	bool IdxRelease(JOYKEY Key, int nPlayer);						// ジョイパッドのリリース処理(プレイヤー指定あり)
	D3DXVECTOR3 Stick(JOYKEY Key, int nPlayer);						// ジョイパッドのスティック処理
	int TriggerPedal(JOYKEY Key, int nPlayer);						// ジョイパッドのトリガーペダル処理
	void Vibration(int nTime, WORD nStrength, int nPlayer);			// ジョイパッドの振動制御
	bool IsJoyPadUse(int nPlayer);									// ジョイパッドの使用状況
	bool StickTrigger(JOYKEY Stick, JOYKEY Key);					// ジョイパッドのスティックのトリガー処理(プレイヤー指定なし)
	bool IdxStickTrigger(JOYKEY Stick, JOYKEY Key, int nPlayer);	// ジョイパッドのスティックのトリガー処理(プレイヤー指定あり)

private:
	//-----------------------------
	// 静的メンバ変数
	//-----------------------------
	static Joypad m_Joypad;	//構造体
};

#endif // !_INPUT_JOYPAD_H_