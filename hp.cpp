//===================================================
//
// HPの処理
// Author : Sato Teruto
//
//===================================================

//-----------------------
// インクルード
//-----------------------
#include "hp.h"
#include "fream.h"

//==========================
// コンストラクタ
//==========================
CHP::CHP() : CGauge(1)
{

}

//==========================
// デストラクタ
//==========================
CHP::~CHP()
{

}

//==========================
// 初期化
//==========================
HRESULT CHP::Init(D3DXVECTOR3 pos)
{
	CGauge::Init(pos);

	CGauge::SetType(m_type);

	return S_OK;
}

//==========================
// 終了
//==========================
void CHP::Uninit()
{
	CGauge::Uninit();
}

//==========================
// 更新
//==========================
void CHP::Update()
{
	CGauge::Update();
}

//==========================
// 描画
//==========================
void CHP::Draw()
{
	CGauge::Draw();
}

//==========================
// 生成
//==========================
CHP* CHP::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, HPTYPE type)
{
	CHP *pHP = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pHP = new CHP;	//生成

	if (pHP != nullptr)
	{//NULLチェック
		//変数に代入
		pHP->m_pos = pos;			//位置
		pHP->m_fWidth = fWidth;		//幅
		pHP->m_fHeight = fHeight;	//高さ
		pHP->m_type = type;			//種類

		//初期化
		pHP->Init(D3DXVECTOR3(pos));
		pHP->SetObjType(OBJTYPE_GAUGE);
	}

	return pHP;
}