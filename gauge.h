//===================================================
//
// ゲージのヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_	//二重インクルード防止

//--------------------------------
// インクルード
//--------------------------------
#include "object2D.h"

//--------------------------------
// 前方宣言
//--------------------------------
class CFream;	//フレーム処理

//--------------------------------
// ゲージクラス
//--------------------------------
class CGauge : public CObject2D
{
public:
	CGauge();
	explicit CGauge(int nPriority);	//コンストラクタ
	virtual ~CGauge() override;		//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	virtual HRESULT Init(D3DXVECTOR3 pos) override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	//------------------
	// セッター
	//------------------
	void SetLife(float fLife, float fRemLife);		//HPの設定
	bool SubHP();									//HP減少時の処理
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }	//位置の設定

protected:
	//------------------
	// メンバ変数
	//------------------
	D3DXVECTOR3 m_pos;	//位置
	float m_fLife;		//体力受け取り用
	float m_fRemLife;	//残り体力受け取り用
	float m_fWidth;		//幅
	float m_fHeight;	//高さ
	float m_fLength;	//HPバーの長さ
	CFream* m_pFream;	//フレーム
};

#endif