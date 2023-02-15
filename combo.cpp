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
CCombo::CCombo() : CObject(1)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_initPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//最初の位置
	m_numberPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//数字の位置
	m_nNumCombo = 0;							//コンボ数
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
	fInterval = 50.0f;	//数値の間隔
	m_newWidth = 60.0f;
	m_newHeight = 80.0f;

	//------------------------------
	// 数値の設定
	//------------------------------
	for (int i = 0; i < nMaxDigits; i++)
	{
		D3DXVECTOR3 numberPos = D3DXVECTOR3(m_pos.x + (fInterval * i), m_pos.y, m_pos.z);
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
	float nPoint = 1.5;

	for (int i = 0; i < nMaxDigits; i++)
	{
		m_pNumber[i]->Update();

		//-------------------
		// 位置の設定
		//-------------------
		if (m_pos.x <= m_initPos.x)
		{//元の位置より小さいなら
			m_pos.x += nPoint;	//位置の加算
		}
		//-------------------
		// 数字間隔の縮小
		//-------------------
		if (fInterval >= 50.0f)
		{//元の幅より大きいなら
			fInterval -= nPoint;
		}
		//-------------------
		// 幅の縮小
		//-------------------
		if (m_newWidth >= 60.0f)
		{//元の幅より大きいなら
			m_newWidth -= nPoint;
		}
		//-------------------
		// 高さの縮小
		//-------------------
		if (m_newHeight >= 80.0f)
		{//元の幅より大きいなら
			m_newHeight -= nPoint;
		}

		//大きさの再設定
		m_pNumber[i]->SetScaling(m_newWidth, m_newHeight);

		//座標の再設定
		m_numberPos = D3DXVECTOR3(m_pos.x + (fInterval * i), m_pos.y, m_pos.z);
		m_pNumber[i]->SetPosition(m_numberPos);
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
		pCombo->m_initPos = pos;

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
	if (m_nNumCombo + nNum < 100)
	{//100にならないなら
		m_nNumCombo += nNum;
	}

	//数値の設定
	SetNumber();

	fInterval = 110.0f;
	m_pos.x = 1050.0f;
	m_newWidth = 130.0f;
	m_newHeight = 150.0f;

	for (int i = 0; i < nMaxDigits; i++)
	{//桁数分回す
		if (m_pNumber[i] != nullptr)
		{//nullじゃないなら
			m_pNumber[i]->SetScaling(m_newWidth, m_newHeight);
			
			m_numberPos = D3DXVECTOR3(m_pos.x + (fInterval * i), m_pos.y, m_pos.z);
			m_pNumber[i]->SetPosition(m_numberPos);
		}
	}

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