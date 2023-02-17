//===================================================
//
// メッセージヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _MESSAGE_H_
#define _MESSAGE_H_	//二重インクルード防止

//-------------------------------
// インクルード
//-------------------------------
#include "object2d.h"

//================================
// メッセージクラス
//================================
class CMessage : public CObject2D
{
public:
	enum MESSAGE
	{
		MESSAGE_NONE = 0,
		MESSAGE_CLEAR,		//クリア
		MESSAGE_BREAK,		//ブレイク
		MESSAGE_START,		//スタート
		MESSAGE_1,			//1
		MESSAGE_2,			//2
		MESSAGE_3,			//3
		MESSAGE_MAX
	};

	CMessage();		//コンストラクタ
	~CMessage();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//------------------
	// 静的メンバ変数
	//------------------
	static CMessage* Create(D3DXVECTOR3 pos, float fWidth, float fHeight, MESSAGE message);

private:
	//------------------
	// メンバ変数
	//------------------
	D3DXCOLOR m_col;	//色
	int m_nCntTime;		//時間カウント
	MESSAGE m_message;	//メッセージ
};

#endif