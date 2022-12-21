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
// ゲージクラス
//--------------------------------
class CGauge : public CObject2D
{
public:
	enum GAUGETYPE
	{
		GAUGETYPE_NONE = 0,
		GAUGETYPE_PLAYER,
		GAUGETYPE_ENEMY,
		GAUGETYPE_MAX
	};

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
	void SubHP();									//HP減少時の処理
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }	//位置の設定
	void SetType(int nNum);		//種類の設定

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
	GAUGETYPE m_type;	//種類
};

#endif