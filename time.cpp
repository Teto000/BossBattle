//===================================================
//
// タイマー処理
// Author : Sato Teruto
//
//===================================================

//----------------------
// インクルード
//----------------------
#include "time.h"
#include "number.h"

//=======================
// コンストラクタ
//=======================
CTime::CTime() : CObject(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_numberPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//数字の位置
	m_nTime = 0;								//時間
	m_nCnt = 0;									//カウント
	fInterval = 0.0f;							//数値の間隔

	for (int i = 0; i < nMaxDigits; i++)
	{
		m_aPosTexU[i] = 0;		//今の桁の数値
		m_pNumber[i] = nullptr;	//数値
	}
}

//=======================
// デストラクタ
//=======================
CTime::~CTime()
{

}

//=======================
// 初期化
//=======================
HRESULT CTime::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_pos = pos;		//位置
	fInterval = 50.0f;	//数値の間隔

	//------------------------------
	// 数値の設定
	//------------------------------
	for (int i = 0; i < nMaxDigits; i++)
	{
		D3DXVECTOR3 numberPos = D3DXVECTOR3(m_pos.x + (fInterval * i), m_pos.y, m_pos.z);
		m_pNumber[i] = CNumber::Create(numberPos, m_nTime);
		m_pNumber[i]->Set(i);
	}

	SetNumber();

	return S_OK;
}

//=======================
// 終了
//=======================
void CTime::Uninit()
{
	for (int i = 0; i < nMaxDigits; i++)
	{
		m_pNumber[i]->Uninit();
	}
}

//=======================
// 更新
//=======================
void CTime::Update()
{
	m_nCnt++;

	if (m_nCnt >= 60)
	{
		m_nTime++;
		SetNumber();
		m_nCnt = 0;
	}
}

//=======================
// 描画
//=======================
void CTime::Draw()
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
CTime *CTime::Create(D3DXVECTOR3 pos)
{
	CTime *pCombo = nullptr;

	//----------------------------------
	// 生成と初期化
	//----------------------------------
	pCombo = new CTime;	//生成

	if (pCombo != nullptr)
	{//NULLチェック
		//初期化
		pCombo->Init(D3DXVECTOR3(pos));
	}

	return pCombo;
}

//=======================
// 数値の設定
//=======================
void CTime::SetNumber()
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
			m_aPosTexU[i] = (m_nTime % nNum0) / nNum1;
			m_pNumber[i]->Set(m_aPosTexU[i]);
		}
	}
}