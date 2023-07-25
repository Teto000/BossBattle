//===================================================
//
// モードヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _MODE_H_
#define _MODE_H_	

//-------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>

//--------------------------------
// 前方宣言
//--------------------------------
class CGame;		//ゲーム
class CTitle;		//タイトル
class CTutorial;	//チュートリアル
class CResult;		//リザルト
class CFade;		//フェード

//================================
// モードクラスの定義
//================================
class CMode
{
public:
	//----------------------------
	// モード列挙型の定義
	//----------------------------
	enum MODE
	{
		MODE_TITLE = 0,		//タイトル
		MODE_TUTORIAL,		//チュートリアル
		MODE_GAME,			//ゲーム
		MODE_RESULT,		//リザルト
		MODE_FADE,			//フェード
		MODE_MAX
	};

	CMode();	//コンストラクタ
	~CMode();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	static HRESULT Init();
	static void Uninit();
	static void Update();

	//------------------
	// 静的メンバ関数
	//------------------
	static void SetMode(MODE mode);		//モードの設定
	static MODE GetMode()		{ return m_mode; };		//モードの取得
	static CGame*	GetGame()	{ return m_pGame; }		//ゲームの取得
	static CTitle*	GetTitle()	{ return m_pTitle; }	//タイトルの取得
	static CFade*	GetFade()	{ return m_pFade; }		//フェードの取得

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
};

#endif