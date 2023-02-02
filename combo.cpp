//===================================================
//
// コンボ表示処理
// Author : Sato Teruto
//
//===================================================

//----------------------
// インクルード
//----------------------
#include "combo.h"
#include "number.h"

//=======================
// コンストラクタ
//=======================
CCombo::CCombo() : CObject(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
	m_nNumCombo = 0;						//コンボ数

	for (int i = 0; i < nMaxDigits; i++)
	{
		m_aPosTexU[i] = 0;		//今の桁の数値
		m_pNumber[i] = nullptr;	//数値
	}
}

//=======================
// デストラクタ
//=======================
CCombo::~CCombo()
{

}

//=======================
// 初期化
//=======================
HRESULT CCombo::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_pos = pos;		//位置

	//------------------------------
	// 数値の設定
	//------------------------------
	for (int i = 0; i < nMaxDigits; i++)
	{
		D3DXVECTOR3 numberPos = D3DXVECTOR3(m_pos.x + (50.0f * i), m_pos.y, m_pos.z);
		m_pNumber[i] = CNumber::Create(numberPos, m_nNumCombo);
		m_pNumber[i]->Set(i);
	}

	SetNumber();

	return S_OK;
}

//=======================
// 終了
//=======================
void CCombo::Uninit()
{
	for (int i = 0; i < nMaxDigits; i++)
	{
		m_pNumber[i]->Uninit();
	}
}

//=======================
// 更新
//=======================
void CCombo::Update()
{
	for (int i = 0; i < nMaxDigits; i++)
	{
		m_pNumber[i]->Update();
	}
}

//=======================
// 描画
//=======================
void CCombo::Draw()
{
	for (int i = 0; i < nMaxDigits; i++)
	{
		m_pNumber[i]->Draw();
	}
}

//=======================
// 生成
// 引数：位置、コンボ数
//=======================
CCombo *CCombo::Create(D3DXVECTOR3 pos, int nNum)
{
	CCombo *pCombo = nullptr;

	//----------------------------------
	// 生成と初期化
	//----------------------------------
	pCombo = new CCombo;	//生成

	if (pCombo != nullptr)
	{//NULLチェック
		//メンバ変数に代入
		pCombo->m_nNumCombo = nNum;

		//初期化
		pCombo->Init(D3DXVECTOR3(pos));
	}

	return pCombo;
}

//=======================
// 数値の設定
//=======================
void CCombo::SetNumber()
{
	for (int i = 0; i < nMaxDigits; i++)
	{//桁数分回す
		if (m_pNumber[i] != nullptr)
		{//nullじゃないなら
		 //powで桁数を出す。
			int nCntNumber = nMaxDigits - i - 1;
			int nNum0 = (int)pow(10, nCntNumber + 1);
			int nNum1 = (int)pow(10, nCntNumber);

			//桁ごとの値を求める
			m_aPosTexU[i] = (m_nNumCombo % nNum0) / nNum1;
			m_pNumber[i]->Set(m_aPosTexU[i]);
		}
	}
}

//=======================
// 数値の加算
//=======================
int CCombo::AddNumber(int nNum)
{
	m_nNumCombo += nNum;

	//数値の設定
	SetNumber();

	return m_nNumCombo;
}

//=======================
// コンボ数リセット
//=======================
void CCombo::ResetCombo()
{
	m_nNumCombo = 0;

	//数値の設定
	SetNumber();
}