//===================================================
//
// 数値をビルボードで表示する処理
// Author : Sato Teruto
//
//===================================================

//----------------------
// インクルード
//----------------------
#include "num_board.h"
#include "application.h"
#include "renderer.h"

//=======================
// コンストラクタ
//=======================
CNumBoard::CNumBoard() : CBillBoard(0)
{
	m_nValue = 0;	//数値
	m_nDigit = 0;	//桁数
}

//=======================
// デストラクタ
//=======================
CNumBoard::~CNumBoard()
{

}

//=======================
// 初期化
//=======================
HRESULT CNumBoard::Init(D3DXVECTOR3 pos)
{
	//オブジェクトの初期化
	CBillBoard::Init(pos);

	//テクスチャの設定
	CBillBoard::SetTexture(CTexture::TEXTURE_NUMBER);

	return S_OK;
}

//=======================
// 終了
//=======================
void CNumBoard::Uninit()
{
	//オブジェクトの終了
	CBillBoard::Uninit();
}

//=======================
// 更新
//=======================
void CNumBoard::Update()
{
	//オブジェクトの更新
	CBillBoard::Update();
}

//=======================
// 描画
//=======================
void CNumBoard::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//アルファテストの有効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 80);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Zバッファのクリア
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	//オブジェクトの描画
	CBillBoard::Draw();

	// Zバッファの有効化設定
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	//アルファテストの無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=======================
// 生成
// 引数：位置、値、桁数
//=======================
CNumBoard *CNumBoard::Create(D3DXVECTOR3 pos, int nValue)
{
	CNumBoard *pNumber = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pNumber = new CNumBoard;	//生成

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
void CNumBoard::Set(int nDigit)
{
	CBillBoard::SetAnimation((float)nDigit, 10);
}