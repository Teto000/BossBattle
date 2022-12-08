//===================================================
//
// 2Dポリゴンの処理
// Author:Teruto Sato
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "polygon.h"
#include "renderer.h"
#include "texture.h"
#include "game.h"
#include "camera.h"

//========================
// コンストラクタ
//========================
CPolygon::CPolygon() : CObject2D(0)
{
	m_fRadius = 0.0f;
	m_type = POLYGONTYPE_NONE;
}

//========================
// デストラクタ
//========================
CPolygon::~CPolygon()
{

}

//========================
// 初期化
//========================
HRESULT CPolygon::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_fRadius = 100.0f;

	CObject2D::Init(pos);

	CObject2D::SetSize(m_fRadius, m_fRadius);
	CObject2D::SetTexture(CTexture::TEXTURE_AIM);

	return S_OK;
}

//========================
// 終了
//========================
void CPolygon::Uninit()
{
	CObject2D::Uninit();
}

//========================
// 更新
//========================
void CPolygon::Update()
{
	CObject2D::Update();
}

//========================
// 描画
//========================
void CPolygon::Draw()
{
	if (CGame::GetCamera()->GetLockOn())
	{//ロックオン状態なら
		CObject2D::Draw();
	}
}

//========================
// 生成
//========================
CPolygon* CPolygon::Create(CPolygon::POLYGONTYPE type)
{
	CPolygon *pPolygon = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pPolygon = new CPolygon;	//生成;

	if (pPolygon != nullptr)
	{//NULLチェック
		//変数に代入
		pPolygon->m_type = type;

		//初期化
		pPolygon->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50.0f, 0.0f));
	}

	return pPolygon;
}