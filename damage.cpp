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
#include "num_board.h"
#include "application.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"

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
		m_aPosTexU[i] = 0;			//今の桁の数値
		m_pNumBoard[i] = nullptr;	//数字
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
	m_fWidth = 30.0f;	//幅
	m_fHeight = 40.0f;	//高さ

	if (type == DAMAGE_TYPE_CRITICAL)
	{//クリティカルなら
		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	//色
	}
	else
	{
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色
	}

	//------------------------------
	// 数値の設定
	//------------------------------
	float cameraRotY = CGame::GetCamera()->GetRot().y;	//カメラの向きを取得
	cameraRotY += D3DX_PI / 2;	//90°回転
	D3DXVECTOR3 vec = D3DXVECTOR3(sinf(cameraRotY), cosf(cameraRotY), 0.0f);	//ベクトルの計算

	for (int i = 0; i < nMaxDigits; i++)
	{
		//カメラ側に向いて並ぶ位置を計算
		D3DXVECTOR3 numberPos = D3DXVECTOR3((m_pos.x - (m_fWidth / 2)) + ((m_fWidth * vec.x) * i),
											m_pos.y,
											m_pos.z + ((m_fWidth * vec.y) * i));

		m_pNumBoard[i] = CNumBoard::Create(numberPos, m_nAtkValue);
		m_pNumBoard[i]->Set(i);							//値の設定
		m_pNumBoard[i]->SetSize(m_fWidth, m_fHeight);	//大きさの設定
		m_pNumBoard[i]->SetColor(m_col);				//色の設定
	}

	SetNumber();

	return S_OK;
}

//=======================
// 終了
//=======================
void CDamage::Uninit()
{
	for (int i = 0; i < nMaxDigits; i++)
	{
		if (m_pNumBoard[i] != nullptr)
		{//nullじゃないなら
			m_pNumBoard[i]->Uninit();
			m_pNumBoard[i] = nullptr;
		}
	}
}

//=======================
// 更新
//=======================
void CDamage::Update()
{
	for (int i = 0; i < nMaxDigits; i++)
	{
		if (m_pNumBoard[i] == nullptr)
		{//nullなら
			continue;
		}

		//----------------------
		// 上昇させる
		//----------------------
		if (m_nDeleteTime >= 20)
		{
			D3DXVECTOR3 pos = m_pNumBoard[i]->GetPosition();
			pos.y += 1.0f;
			m_pNumBoard[i]->SetPos(pos);
		}

		m_pNumBoard[i]->Update();

		//色の設定
		m_pNumBoard[i]->SetColor(m_col);
	}

	//消えるまでの時間を数える
	m_nDeleteTime++;

	if (m_nDeleteTime >= 20)
	{//一定時間が経過したら
		//透明度の減少
		m_col.a -= 0.05f;
	}

	if (m_col.a <= 0.0f)
	{//完全に透明になったら
		Uninit();
		return;
	}
}

//=======================
// 描画
//=======================
void CDamage::Draw()
{
	for (int i = 0; i < nMaxDigits; i++)
	{
		if (m_pNumBoard[i] != nullptr)
		{//nullじゃないなら
			m_pNumBoard[i]->Draw();
		}
	}
}

//=======================
// 生成
// 引数：位置、コンボ数
//=======================
CDamage *CDamage::Create(D3DXVECTOR3 pos, int nNum, DAMAGE_TYPE type)
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
		pDamage->type = type;

		//初期化
		pDamage->Init(pos);
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
		//桁数を計算
		int nDigit = (int)(log10(m_nAtkValue) + 1);

		//ナンバーの描画を有効・無効にする
		m_pNumBoard[i]->SetEnable(nMaxDigits - i <= nDigit);

		//powで桁数を出す。
		int nCntNumber = nMaxDigits - i - 1;
		int nNum0 = (int)pow(10, nCntNumber + 1);
		int nNum1 = (int)pow(10, nCntNumber);

		//桁ごとの値を求める
		m_aPosTexU[i] = (m_nAtkValue % nNum0) / nNum1;
		m_pNumBoard[i]->Set(m_aPosTexU[i]);
	}
}