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
#include "game.h"
#include "player.h"

//========================
// コンストラクタ
//========================
CStyleShift::CStyleShift() : CBillBoard(0)
{
	m_stylePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_offSet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	m_fWidth = 70.0f;	//幅
	m_fHeight = 70.0f;	//高さ
	m_stylePos = pos;
	m_offSet = m_pos;

	CBillBoard::Init(m_stylePos);

	//大きさの設定
	CBillBoard::SetSize(m_fWidth, m_fHeight);

	//テクスチャの設定
	CBillBoard::SetTexture(CTexture::TEXTURE_NONE);

	switch (m_type)
	{
	case STYLE_TYPE_ATTACK:
		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);	//色
		break;

	case STYLE_TYPE_SPEED:
		m_col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.0f);
		break;

	case STYLE_TYPE_COMBO:
		m_col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f);
		break;

	default:
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		break;
	}

	//色の設定
	CBillBoard::SetColor(m_col);

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

	//-----------------------------
	// プレイヤーの位置を参照
	//-----------------------------
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetWorldPos();
	D3DXVECTOR3 pos = playerPos;
	CBillBoard::SetPos(pos);
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
CStyleShift* CStyleShift::Create(D3DXVECTOR3 pos, STYLE_TYPE type)
{
	CStyleShift *pStyleShift = nullptr;

	//----------------------------------
	// 生成と初期化
	//----------------------------------
	pStyleShift = new CStyleShift;	//生成

	if (pStyleShift != nullptr)
	{//NULLチェック
		//メンバ変数に代入
		pStyleShift->m_type = type;	//種類

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
		m_col.a = 0.5f;
		break;

	case false:
		m_col.a = 0.0f;
		break;
	}

	CBillBoard::SetColor(m_col);
}