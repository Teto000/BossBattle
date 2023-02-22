//===================================================
//
// 数値を表示する処理
// Author : Sato Teruto
//
//===================================================

//----------------------
// インクルード
//----------------------
#include "number.h"
#include "application.h"
#include "renderer.h"

//=======================
// コンストラクタ
//=======================
CNumber::CNumber() : CObject2D(1)
{
	m_nValue = 0;		//数値
	m_nDigit = 0;		//桁数
	m_bDraw = false;	//描画フラグ
}

//=======================
// デストラクタ
//=======================
CNumber::~CNumber()
{

}

//=======================
// 初期化
//=======================
HRESULT CNumber::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_fWidth = 60.0f;
	m_fHeight = 80.0f;

	//オブジェクトの初期化
	CObject2D::Init(pos);

	//大きさの設定
	CObject2D::SetSize(m_fWidth, m_fHeight);

	//テクスチャの設定
	CObject2D::SetTexture(CTexture::TEXTURE_NUMBER);

	return S_OK;
}

//=======================
// 終了
//=======================
void CNumber::Uninit()
{
	//オブジェクトの終了
	CObject2D::Uninit();
}

//=======================
// 更新
//=======================
void CNumber::Update()
{
	//オブジェクトの更新
	CObject2D::Update();
}

//=======================
// 描画
//=======================
void CNumber::Draw()
{
	if (!m_bDraw)
	{//描画しない状態なら
		return;
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//アルファテストの有効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//オブジェクトの描画
	CObject2D::Draw();

	//アルファテストの無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=======================
// 生成
// 引数：位置、値、桁数
//=======================
CNumber *CNumber::Create(D3DXVECTOR3 pos, int nValue)
{
	CNumber *pNumber = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pNumber = new CNumber;	//生成

	if (pNumber != nullptr)
	{//NULLチェック
		//メンバ変数に代入
		pNumber->m_nValue = nValue;

		//初期化
		pNumber->Init(D3DXVECTOR3(pos));
		pNumber->SetObjType(OBJTYPE_NUMBER);
	}

	return pNumber;
}

//=======================
// 数値の設定
// 引数：桁数
//=======================
void CNumber::Set(int nDigit)
{
	CObject2D::SetAnimation((float)nDigit, 10);
}

//=======================
// 拡縮
//=======================
void CNumber::SetScaling(float fWidth, float fHeight)
{
	//幅と高さの設定
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	CObject2D::SetSize(m_fWidth, m_fHeight);
}