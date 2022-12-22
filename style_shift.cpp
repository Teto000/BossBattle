//===================================================
//
// 戦闘スタイル表示処理
// Author:Teruto Sato
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "style_shift.h"
#include "texture.h"

//========================
// コンストラクタ
//========================
CStyleShift::CStyleShift() : CBillBoard(0)
{
	
}

//========================
// デストラクタ
//========================
CStyleShift::~CStyleShift()
{

}

//========================
// 初期化
//========================
HRESULT CStyleShift::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_fWidth = 100.0f;	//幅
	m_fHeight = 100.0f;	//高さ
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	//色

	CBillBoard::Init(pos);

	//大きさの設定
	CBillBoard::SetSize(m_fWidth, m_fHeight);

	//色の設定
	CBillBoard::SetColor(m_col);

	//テクスチャの設定
	CBillBoard::SetTexture(CTexture::TEXTURE_NONE);

	return S_OK;
}

//========================
// 終了
//========================
void CStyleShift::Uninit()
{
	CBillBoard::Uninit();
}

//========================
// 更新
//========================
void CStyleShift::Update()
{
	CBillBoard::Update();

	//-----------------------
	// 徐々に鮮明にする処理
	//-----------------------
	if (m_bFlash && m_col.a <= 1.0f)
	{//明るくする状態 & 不透明じゃないなら
		m_col.a += 0.05f;
		CBillBoard::SetColor(m_col);
	}

}

//========================
// 描画
//========================
void CStyleShift::Draw()
{
	CBillBoard::Draw();
}

//========================
// 生成
//========================
CStyleShift* CStyleShift::Create(D3DXVECTOR3 pos)
{
	CStyleShift *pStyleShift = nullptr;

	//----------------------------------
	// 生成と初期化
	//----------------------------------
	pStyleShift = new CStyleShift;	//生成

	if (pStyleShift != nullptr)
	{//NULLチェック
		//初期化
		pStyleShift->Init(pos);
		pStyleShift->SetObjType(OBJTYPE_STYLESHIFT);
	}

	return pStyleShift;
}

//========================
// スタイルの状態の設定
//========================
void CStyleShift::SetStyle(bool flash)
{
	//状態を設定
	m_bFlash = flash;

	//-------------------------------
	// 状態に応じた透明度に設定
	//-------------------------------
	switch (m_bFlash)
	{
	case true:
		CBillBoard::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		break;

	case false:
		CBillBoard::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		break;
	}
}