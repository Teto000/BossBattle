//===================================================
//
// タイマー処理
// Author : Sato Teruto
//
//===================================================

//----------------------
// インクルード
//----------------------
#include <string>
#include "time.h"
#include "number.h"
#include "game.h"
#include "renderer.h"
#include "ranking.h"
#include "input.h"
#include "input_keyboard.h"

//=======================
// コンストラクタ
//=======================
CTime::CTime() : CObject(1)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_numberPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//数字の位置
	m_nTime = 0;								//時間
	m_nCntMove = 0;								//移動までの時間
	m_nCntFream = 0;							//フレーム数のカウント
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
	if (!CGame::GetStart())
	{//開始フラグが立っていないなら
		return;
	}

	//----------------------
	// ゲーム終了時に拡大
	//----------------------
	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		if (CGame::GetFinish())
		{
			//タイムを保存
			CRanking::SetNewTime(m_nTime);

			m_nCntMove++;

			if (m_nCntMove >= 80)
			{//一定時間が経過したら
				//--------------------------------------
				// 目的の位置まで移動する
				//--------------------------------------
				m_pos.x += ((SCREEN_WIDTH / 2 - 50.0f) - m_pos.x) * 0.08f;	//減衰処理
				m_pos.y += ((SCREEN_HEIGHT / 2) - m_pos.y) * 0.08f;

				for (int i = 0; i < nMaxDigits; i++)
				{
					m_pNumber[i]->SetPosition(D3DXVECTOR3(m_pos.x + i * 50.0f, m_pos.y, m_pos.z));
				}
			}
		}
		else
		{//ゲーム中なら
			//フレーム数を数える
			m_nCntFream++;

			//----------------------
			// 時間の加算
			//----------------------
			if (m_nCntFream >= 60)
			{
				m_nTime++;
				SetNumber();
				m_nCntFream = 0;
			}
		}
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
			//桁数を計算
			//int nDigit = std::to_string(m_nTime).size();
			int nDigit = (int)(log10(m_nTime) + 1);

			//ナンバーの描画を有効・無効にする
			m_pNumber[i]->SetEnable(nMaxDigits - i <= nDigit);

			if (m_nTime == 0)
			{//コンボ数が0なら
			 //ナンバーの描画を有効・無効にする
				m_pNumber[nMaxDigits - 1]->SetEnable(true);
			}

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

//=======================
// 時間の設定
//=======================
void CTime::SetTime(int nTime)
{
	m_nTime = nTime;

	SetNumber();
}

//=======================
// 色の設定
//=======================
void CTime::SetColor(D3DXCOLOR col)
{
	for (int i = 0; i < nMaxDigits; i++)
	{
		m_pNumber[i]->SetColor(col);
	}
}