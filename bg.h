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
// バレットクラス
//--------------------------------
class CBg : public CObject
{
public:
	enum BGTYPE
	{
		BGTYPE_TITLE,	//タイトル
		BGTYPE_RESULT,	//リザルト
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
	D3DXVECTOR3 GetPosition() override;		//位置の取得
	float GetWidth() override;				//幅の取得
	float GetHeight() override;				//高さの取得

	//-------------------
	// 静的メンバ関数
	//-------------------
	static CBg *Create(D3DXVECTOR3 pos, BGTYPE type);

private:
	//-------------------
	// メンバ変数
	//-------------------
	CObject2D *m_pObject2D;		//オブジェクト2D
	D3DXVECTOR3 m_pos;			//位置
	float m_fWidth;				//幅
	float m_fHeight;			//高さ
	BGTYPE m_type;				//種類
};

#endif // !_BG_H_