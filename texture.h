//**************************************************
// 
// texture.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _TEXTURE_H_	//このマクロ定義がされてなかったら
#define _TEXTURE_H_	//２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include <d3dx9.h>

//==================================================
// 定義
//==================================================
class CTexture
{
public: /* 定義 */
	enum TEXTURE
	{
		TEXTURE_TITLE = 0,		//タイトル
		TEXTURE_RESULT,			//リザルト
		TEXTURE_GROUND,			//地面
		TEXTURE_AIM,			//照準
		TEXTURE_NUMBER,			//数値
		TEXTURE_SKY,			//空
		TEXTURE_BULLET,			//弾

		/* ↓ メッセージ ↓ */
		TEXTURE_CLEAR,			//クリア
		TEXTURE_GAMEOVER,		//ゲームオーバー
		TEXTURE_BREAK,			//ブレイク
		TEXTURE_START,			//スタート
		TEXTURE_1,				//1
		TEXTURE_2,				//2
		TEXTURE_3,				//3

		/* ↓ アイテム ↓ */
		TEXTURE_ITEM_HEAL,		//回復

		/* ↓ 背景 ↓ */
		TEXTURE_BG_PRESS,		//Press Any Key
		TEXTURE_BG_COMBO,		//コンボ背景
		TEXTURE_BG_TIMER,		//タイマー背景
		TEXTURE_BG_PLAYER_HP,	//プレイヤー背景
		TEXTURE_BG_ENEMY_HP,	//エネミーHP背景

		TEXTURE_MAX,
		TEXTURE_NONE,			//使用しない
	};

	static const char* s_FileName[];	// ファイルパス

public:
	CTexture();		// デフォルトコンストラクタ
	~CTexture();	// デストラクタ

public: /* メンバ関数 */
	void LoadAll();										// 全ての読み込み
	void Load(TEXTURE inTexture);						// 指定の読み込み
	void ReleaseAll();									// 全ての破棄
	void Release(TEXTURE inTexture);					// 指定の破棄
	LPDIRECT3DTEXTURE9 GetTexture(TEXTURE inTexture);	// 情報の取得

private: /* メンバ変数 */
	LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];	// テクスチャの情報
};

#endif // !_TEXTURE_H_
