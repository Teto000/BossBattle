//===================================================
//
// アプリケーションヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _APPLICATION_H_
#define _APPLICATION_H_	

//------------------------------
// インクルード
//------------------------------
#include <d3dx9.h>

//------------------------------
// 前方宣言
//------------------------------
class CGame;		//ゲーム
class CTitle;		//タイトル
class CTutorial;	//チュートリアル
class CResult;		//リザルト
class CFade;		//フェード

class CRenderer;	//レンダラー
class CInput;		//インプット
class CTexture;		//テクスチャ
class CSound;		//サウンド
class CLight;		//ライト
class CDebugProc;	//デバッグ用文字

//================================
// アプリケーションクラスの定義
//================================
class CApplication
{
public:
	//--------------------------------
	// ゲームモードの列挙型の定義
	//--------------------------------
	enum MODE
	{
		MODE_TITLE = 0,		//タイトル
		MODE_TUTORIAL,		//チュートリアル
		MODE_GAME,			//ゲーム
		MODE_RESULT,		//リザルト
		MODE_FADE,			//フェード
		MODE_MAX
	};

	enum GAMEMODE
	{
		GAMEMODE_TIME = 0,
		GAMEMODE_SCORE,
		GAMEMODE_MAX
	};

	CApplication();		//コンストラクタ
	~CApplication();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();
	void Draw();

	//------------------
	// 静的メンバ関数
	//------------------
	static void SetMode(MODE mode);				//モードの設定
	static MODE GetMode() { return m_mode; };	//モードの取得
	static CGame*			GetGame()		{ return m_pGame; }			//ゲームの取得
	static CTitle*			GetTitle()		{ return m_pTitle; }		//タイトルの取得
	static CFade*			GetFade()		{ return m_pFade; }			//フェードの取得
	static CRenderer*		GetRenderer()	{ return m_pRenderer; }		//レンダラーの取得
	static CInput*			GetInput()		{ return m_pInput; }		//インプットの取得
	static CTexture*		GetTexture()	{ return m_pTexture; }		//テクスチャの取得
	static CSound*			GetSound()		{ return m_pSound; }		//サウンドの取得
	static CLight*			GetLight()		{ return m_pLight; }		//ライトの取得

private:
	//------------------
	// 静的メンバ変数
	//------------------
	static CGame*		m_pGame;		//ゲームクラス
	static CTitle*		m_pTitle;		//タイトルクラス
	static CTutorial*	m_pTutorial;	//チュートリアル
	static CResult*		m_pResult;		//リザルトクラス
	static CFade*		m_pFade;		//フェードクラス
	static MODE			m_mode;			//モード
	static GAMEMODE		m_gameMode;		//ゲームモード

	static CRenderer*	m_pRenderer;	//レンダラークラス
	static CInput*		m_pInput;		//インプットクラス
	static CTexture*	m_pTexture;		//テクスチャクラス
	static CSound*		m_pSound;		//サウンドクラス
	static CLight*		m_pLight;		//ライトクラス
	static CDebugProc*	m_pDebugproc;	//デバッグ用文字
};

#endif // !_APPLICATION_H_