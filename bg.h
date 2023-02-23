//===================================================
//
// 背景のヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _BG_H_
#define _BG_H_	

//--------------------------------
// インクルード
//--------------------------------
#include "object2D.h"

//--------------------------------
// 背景クラス
//--------------------------------
class CBg : public CObject
{
public:
	enum BGTYPE
	{
		BGTYPE_TITLE,		//タイトル
		BGTYPE_RESULT,		//リザルト
		BGTYPE_MANUAL,		//操作説明
		BGTYPE_TUTORIAL1,	//チュートリアル1
		BGTYPE_TUTORIAL2,	//チュートリアル2
		BGTYPE_PRESS,		//キー押下の指示
		BGTYPE_COMBO,		//コンボ
		BGTYPE_TIMER,		//タイマー
		BGTYPE_PLAYER_HP,	//プレイヤーHP
		BGTYPE_ENEMY_HP,	//エネミーHP
		BGTYPE_BLACK,		//黒い板
		BGTYPE_MAX
	};

	CBg();				//コンストラクタ
	~CBg() override;	//デストラクタ

	//-------------------
	// メンバ関数
	//-------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw()	  override;

	//-------------------
	// ゲッター
	//-------------------
	D3DXVECTOR3 GetPosition() override  { return m_pos; }		//位置の取得
	float GetWidth() override			{ return 0.0f; }		//幅の取得
	float GetHeight() override			{ return 0.0f; }		//高さの取得

	//-------------------
	// 静的メンバ関数
	//-------------------
	static CBg *Create(D3DXVECTOR3 pos, BGTYPE type);	//生成

private:
	//-------------------
	// メンバ変数
	//-------------------
	CObject2D *m_pObject2D;		//オブジェクト2D
	D3DXVECTOR3 m_pos;			//位置
	D3DXCOLOR m_col;			//色
	int m_nFlashTime;			//点滅時間
	float m_fWidth;				//幅
	float m_fHeight;			//高さ
	BGTYPE m_type;				//種類
};

#endif // !_BG_H_