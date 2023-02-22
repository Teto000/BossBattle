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
#include <vector>
#include <algorithm>
#include "ranking.h"
#include "time.h"
#include "game.h"
#include "renderer.h"
#include "input.h"
#include "input_keyboard.h"

//----------------------
// 静的メンバ変数宣言
//----------------------
int CRanking::m_nTime = 0;	//時間

//=======================
// コンストラクタ
//=======================
CRanking::CRanking() : CObject(1)
{
	m_nRankUpdate = 0;		//更新ランクNo.

	for (int i = 0; i < nMaxRanking; i++)
	{
		m_pTime[i];	//時間
	}
}

//=======================
// デストラクタ
//=======================
CRanking::~CRanking()
{

}

//=======================
// 初期化
//=======================
HRESULT CRanking::Init(D3DXVECTOR3 pos)
{
	for (int i = 0; i < nMaxRanking; i++)
	{
		m_pTime[i] = CTime::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 100 + (i * 100.0f), 0.0f));
	}

	Load();		//読み込み
	Ranking();	//並び替え
	Save();		//書き込み

	return S_OK;
}

//=======================
// 終了
//=======================
void CRanking::Uninit()
{
	for (int i = 0; i < nMaxRanking; i++)
	{
		m_pTime[i]->Uninit();
	}
}

//=======================
// 更新
//=======================
void CRanking::Update()
{
	for (int i = 0; i < nMaxRanking; i++)
	{
		m_pTime[i]->Update();
	}

	//-------------------------------
	// 今回のスコアを赤くする
	//-------------------------------
	if (m_nTime != m_nRankUpdate)
	{//今回のタイムがランキングの数値と同じなら
		m_pTime[m_nRankUpdate]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}

#ifdef _DEBUG
	//ランキングの初期化
	if (CInputKeyboard::Trigger(DIK_1))
	{
		for (int i = 0; i < nMaxRanking; i++)
		{
			m_pTime[i]->SetTime(999);
			Save();		//書き込み
		}
	}
#endif // _DEBUG
}

//=======================
// 描画
//=======================
void CRanking::Draw()
{
	for (int i = 0; i < nMaxRanking; i++)
	{
		m_pTime[i]->Draw();
	}
}

//=======================
// 生成
// 引数：位置、コンボ数
//=======================
CRanking *CRanking::Create()
{
	CRanking *pRanking = nullptr;

	//----------------------------------
	// 生成と初期化
	//----------------------------------
	pRanking = new CRanking;	//生成

	if (pRanking != nullptr)
	{//NULLチェック
		//初期化
		pRanking->Init(D3DXVECTOR3(0.0f,0.0f,0.0f));
	}

	return pRanking;
}

//=========================================
// 書き込み
//=========================================
void CRanking::Save()
{
	FILE*fp = fopen("data\\TEXT\\Ranking.txt", "w");	//ファイル読み込み
	const int lenLine = 1024;							//1単語の最大数

	if (fp != nullptr)
	{
		for (int i = 0; i < nMaxRanking; i++)
		{
			fprintf(fp, "%d\n", m_pTime[i]->GetTime());	//読み込んだ文字ごとに設定する
		}

		fclose(fp);
	}
}

//=========================================
// 読み込み
//=========================================
void CRanking::Load()
{
	FILE*fp = fopen("data\\TEXT\\Ranking.txt", "r");	//ファイル読み込み
	const int lenLine = 1024;							//1単語の最大数
	int nScore = 0;										//読み込み用の文字列

	if (fp != nullptr)
	{
		for (int i = 0; i < nMaxRanking; i++)
		{
			fscanf(fp, "%d", &nScore);	//読み込んだ文字ごとに設定する
			m_pTime[i]->SetTime(nScore);
		}

		fclose(fp);
	}
}

//=========================================
// ランキングの並び替え
//=========================================
void CRanking::Ranking()
{
	m_nRankUpdate = -1;
	if (m_nTime <= m_pTime[nMaxRanking - 1]->GetTime())
	{//比較
		m_pTime[nMaxRanking - 1]->SetTime(m_nTime);

		//昇順に並び変える
		std::vector<int> rank = { m_pTime[0]->GetTime()
								, m_pTime[1]->GetTime()
								, m_pTime[2]->GetTime()
								, m_pTime[3]->GetTime()
								, m_pTime[4]->GetTime() };

		std::sort(rank.begin(), rank.end());

		for (int i = 0; i < nMaxRanking; i++)
		{//並び変えたやつを代入
			m_pTime[i]->SetTime(rank[i]);

			if (m_nTime == rank[i])
			{
				m_nRankUpdate = i;
			}
		}
	}
}