//===================================================
//
// ゲージの処理
// Author : Sato Teruto
//
//===================================================

//-----------------------
// インクルード
//-----------------------
#include "gauge.h"

//==========================
// コンストラクタ
//==========================
CGauge::CGauge() : CBillBoard(0)
{
}
CGauge::CGauge(int nPriority) : CBillBoard(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
	m_fLife = 0;		//体力受け取り用
	m_fRemLife = 0;		//残り体力受け取り用
	m_fWidth = 0.0f;	//幅
	m_fHeight = 0.0f;	//高さ
	m_fLength = 0.0f;	//HPバーの長さ
}

//==========================
// デストラクタ
//==========================
CGauge::~CGauge()
{

}

//==========================
// 初期化
//==========================
HRESULT CGauge::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_pos = pos;	//位置
	m_fLength = (m_fWidth / 100);	//幅

	CBillBoard::Init(pos);
	CBillBoard::SetSize(m_fWidth, m_fHeight);

	//頂点座標の設定
	CBillBoard::SetVtxCIE_Gauge(m_pos, m_fWidth / 2,
		-m_fWidth / 2 + (m_fLength * 100), m_fHeight / 2, m_fHeight / 2);

	CBillBoard::SetTexture(CTexture::TEXTURE_NONE);	//テクスチャの設定

	return S_OK;
}

//==========================
// 終了
//==========================
void CGauge::Uninit()
{
	CBillBoard::Uninit();
}

//==========================
// 更新
//==========================
void CGauge::Update()
{
	CBillBoard::Update();

	//HP減少時の処理
	SubHP();
}

//==========================
// 描画
//==========================
void CGauge::Draw()
{
	CBillBoard::Draw();
}

//===========================
// HPの設定
//===========================
void CGauge::SetLife(float fLife, float fRemLife)
{
	m_fLife = fLife;
	m_fRemLife = fRemLife;
}

//===========================
// HP減少時の処理
//===========================
void CGauge::SubHP()
{
	//-------------------------
	// 頂点座標の設定
	//-------------------------
	if (m_fRemLife >= 1)
	{//残り体力(%)があるなら
		//その体力分の座標を設定
		CBillBoard::SetVtxCIE_Gauge(m_pos, -m_fWidth / 2,
			-m_fWidth / 2 + (m_fLength * m_fRemLife), m_fHeight / 2, -m_fHeight / 2);
	}
	else if (m_fRemLife == 0 && m_fLife > 0)
	{//残り体力が0% かつ 体力が0じゃないなら
		//1%分のゲージを維持
		CBillBoard::SetVtxCIE_Gauge(m_pos, -m_fWidth / 2,
			-m_fWidth / 2 + m_fLength, -m_fHeight / 2, -m_fHeight / 2);
	}

	//-------------------------
	// HPごとの処理
	//-------------------------
	if (m_fRemLife <= 0 && m_fLife <= 0)
	{//HPが0になった かつ 体力がなかったら
		//HPバーの消去
		Uninit();
		return;
	}
	else if (m_fRemLife <= 20)
	{//HPが20%以下になったら
		//赤色にする
		CBillBoard::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else if (m_fRemLife <= 50)
	{//HPが50%以下になったら
		//黄色にする
		CBillBoard::SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
		//緑色にする
		CBillBoard::SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}
}