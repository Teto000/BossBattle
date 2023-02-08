//===================================================
//
// ダメージ数処理
// Author : Sato Teruto
//
//===================================================

//----------------------
// インクルード
//----------------------
#include "damage.h"
#include "number.h"
#include "application.h"
#include "renderer.h"

//=======================
// コンストラクタ
//=======================
CDamage::CDamage() : CBillBoard(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
	m_nAtkValue = 0;						//コンボ数
	m_nDeleteTime = 0;						//消えるまでの時間

	for (int i = 0; i < nMaxDigits; i++)
	{
		m_aPosTexU[i] = 0;		//今の桁の数値
	}
}

//=======================
// デストラクタ
//=======================
CDamage::~CDamage()
{

}

//=======================
// 初期化
//=======================
HRESULT CDamage::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_pos = pos;		//位置
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色
	m_fWidth = 60.0f;	//幅
	m_fHeight = 80.0f;	//高さ

	//------------------------------
	// 数値の設定
	//------------------------------
	//オブジェクトの初期化
	CBillBoard::Init(pos);

	//大きさの設定
	CBillBoard::SetSize(m_fWidth, m_fHeight);

	//テクスチャの設定
	CBillBoard::SetTexture(CTexture::TEXTURE_NUMBER);

	//数値の設定
	SetNumber();

	return S_OK;
}

//=======================
// 終了
//=======================
void CDamage::Uninit()
{
	CBillBoard::Uninit();
}

//=======================
// 更新
//=======================
void CDamage::Update()
{
	CBillBoard::Update();

	//消えるまでの時間を数える
	m_nDeleteTime++;

	if (m_nDeleteTime >= 60)
	{//一定時間が経過したら
		//透明度の減少
		m_col.a -= 0.1f;
	}

	if (m_col.a <= 0.0f)
	{//完全に透明になったら
		Uninit();
	}
}

//=======================
// 描画
//=======================
void CDamage::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//アルファテストの有効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 80);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Zバッファのクリア
	pDevice->SetRenderState(D3DRS_ZENABLE, false);

	//オブジェクトの描画
	CBillBoard::Draw();

	// Zバッファの有効化設定
	pDevice->SetRenderState(D3DRS_ZENABLE, true);

	//アルファテストの無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=======================
// 生成
// 引数：位置、コンボ数
//=======================
CDamage *CDamage::Create(D3DXVECTOR3 pos, int nNum)
{
	CDamage *pDamage = nullptr;

	//----------------------------------
	// 生成と初期化
	//----------------------------------
	pDamage = new CDamage;	//生成

	if (pDamage != nullptr)
	{//NULLチェック
		//メンバ変数に代入
		pDamage->m_nAtkValue = nNum;

		//初期化
		pDamage->Init(D3DXVECTOR3(pos));
	}

	return pDamage;
}

//=======================
// 数値の設定
//=======================
void CDamage::SetNumber()
{
	for (int i = 0; i < nMaxDigits; i++)
	{//桁数分回す
		//powで桁数を出す。
		int nCntNumber = nMaxDigits - i - 1;
		int nNum0 = (int)pow(10, nCntNumber + 1);
		int nNum1 = (int)pow(10, nCntNumber);

		//桁ごとの値を求める
		m_aPosTexU[i] = (m_nAtkValue % nNum0) / nNum1;

		CBillBoard::SetAnimation((float)m_aPosTexU[i], 10);
	}
}