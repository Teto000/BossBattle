//===================================================
//
// ゲージのフレーム処理
// Author : Sato Teruto
//
//===================================================

//-----------------------
// インクルード
//-----------------------
#include "fream.h"

//==========================
// コンストラクタ
//==========================
CFream::CFream() : CObject2D(0)
{
}

//==========================
// デストラクタ
//==========================
CFream::~CFream()
{

}

//==========================
// 初期化
//==========================
HRESULT CFream::Init(D3DXVECTOR3 pos)
{
	CObject2D::Init(pos);

	//大きさの設定
	CObject2D::SetSize(m_fWidth, m_fHeight);

	//色の設定
	CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	//テクスチャの設定
	CObject2D::SetTexture(CTexture::TEXTURE_NONE);

	return S_OK;
}

//==========================
// 終了
//==========================
void CFream::Uninit()
{
	CObject2D::Uninit();
}

//==========================
// 更新
//==========================
void CFream::Update()
{
	CObject2D::Update();
}

//==========================
// 描画
//==========================
void CFream::Draw()
{
	CObject2D::Draw();
}

//==========================
// 生成
//==========================
CFream* CFream::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CFream *pFream = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pFream = new CFream;	//生成

	if (pFream != nullptr)
	{//NULLチェック
		//変数に代入
		pFream->m_pos = pos;			//位置
		pFream->m_fWidth = fWidth;		//幅
		pFream->m_fHeight = fHeight;	//高さ

		//初期化
		pFream->Init(D3DXVECTOR3(pos));
	}

	return pFream;
}