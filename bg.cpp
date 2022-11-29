//===================================================
//
// 背景の処理
// Author:Teruto Sato
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "bg.h"
#include "renderer.h"
#include "texture.h"

//========================
// コンストラクタ
//========================
CBg::CBg() : CObject(0)
{
	m_pObject2D = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
}

//========================
// デストラクタ
//========================
CBg::~CBg()
{

}

//========================
// 初期化
//========================
HRESULT CBg::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_pos = pos;
	m_fWidth = SCREEN_WIDTH;
	m_fHeight = SCREEN_HEIGHT;

	if (m_pObject2D != nullptr)
	{
		m_pObject2D->Init(m_pos);
		m_pObject2D->SetSize(m_fWidth, m_fHeight);

		//-------------------------
		// 種類ごとの処理
		//-------------------------
		switch (m_type)
		{
		case BGTYPE_TITLE:
			m_pObject2D->SetTexture(CTexture::TEXTURE_TITLE);
			break;

		case BGTYPE_RESULT:
			m_pObject2D->SetTexture(CTexture::TEXTURE_RESULT);
			break;

		default:
			break;
		}
	}

	return S_OK;
}

//========================
// 終了
//========================
void CBg::Uninit()
{
	if (m_pObject2D != nullptr)
	{
		m_pObject2D->Uninit();
	}
}

//========================
// 更新
//========================
void CBg::Update()
{
	if (m_pObject2D != nullptr)
	{
		m_pObject2D->Update();
	}
}

//========================
// 描画
//========================
void CBg::Draw()
{
	if (m_pObject2D != nullptr)
	{
		m_pObject2D->Draw();
	}
}

//========================
// 生成
//========================
CBg* CBg::Create(D3DXVECTOR3 pos, BGTYPE type)
{
	CBg *pBg = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pBg = new CBg;	//生成
	pBg->m_pObject2D = new CObject2D(0);

	if (pBg != nullptr)
	{//NULLチェック
		//変数に代入
		pBg->m_type = type;

		//初期化
		pBg->Init(pos);
	}

	return pBg;
}

//========================
// 位置の取得
//========================
D3DXVECTOR3 CBg::GetPosition()
{
	return m_pos;
}

//========================
//	幅の取得
//========================
float CBg::GetWidth()
{
	return m_fWidth;
}

//========================
// 高さ取得
//========================
float CBg::GetHeight()
{
	return m_fHeight;
}