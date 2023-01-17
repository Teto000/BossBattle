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

	if (SubHP())
	{//ゲージが消えていたら
		return;
	}

	//-------------------------
	// 種類ごとの処理
	//-------------------------
	switch (m_type)
	{
	//----------------------------
	// プレイヤーのHP
	//----------------------------
	case GAUGETYPE_HP_PLAYER:
		if (m_fRemLife <= 20)
		{//HPが20%以下になったら
			//赤色にする
			CObject2D::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else if (m_fRemLife <= 50)
		{//HPが50%以下になったら
			//黄色にする
			CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		}
		else
		{
			//緑色にする
			CObject2D::SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		}
		break;

	//----------------------------
	// 敵のHP
	//----------------------------
	case GAUGETYPE_HP_ENEMY:
		//オレンジ色にする
		CObject2D::SetColor(D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));
		break;
	}
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
CHP* CHP::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, GAUGETYPE type)
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