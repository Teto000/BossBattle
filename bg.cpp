//===================================================
//
// 背景の処理
// Author:Teruto Sato
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "bg.h"
#include "renderer.h"
#include "texture.h"

//========================
// コンストラクタ
//========================
CBg::CBg() : CObject(1)
{
	m_pObject2D = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色
	m_nFlashTime = 0;	//点滅時間
	m_fWidth = 0.0f;	//幅
	m_fHeight = 0.0f;	//高さ
}

//========================
// デストラクタ
//========================
CBg::~CBg()
{

}

//========================
// 初期化
//========================
HRESULT CBg::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_pos = pos;
	m_nFlashTime = 0;
	m_fWidth = SCREEN_WIDTH;
	m_fHeight = SCREEN_HEIGHT;

	if (m_pObject2D != nullptr)
	{
		m_pObject2D->Init(m_pos);

		//-------------------------
		// 種類ごとの処理
		//-------------------------
		switch (m_type)
		{
		//タイトル画面の背景
		case BGTYPE_TITLE:
			m_pObject2D->SetTexture(CTexture::TEXTURE_TITLE);
			m_fWidth = 1000.0f;
			m_fHeight = 300.f;
			break;

		//リザルト画面の背景
		case BGTYPE_RESULT:
			m_pObject2D->SetTexture(CTexture::TEXTURE_RESULT);
			break;

		//キー押下の指示
		case BGTYPE_PRESS:
			m_fWidth = 800.0f;
			m_fHeight = 150.0f;
			m_pObject2D->SetTexture(CTexture::TEXTURE_BG_PRESS);
			break;

		//コンボUIの背景
		case BGTYPE_COMBO:
			m_fWidth = 200.0f;
			m_fHeight = 180.0f;
			m_pObject2D->SetTexture(CTexture::TEXTURE_BG_COMBO);
			break;

		//タイマーUIの背景
		case BGTYPE_TIMER:
			m_fWidth = 200.0f;
			m_fHeight = 180.0f;
			m_pObject2D->SetTexture(CTexture::TEXTURE_BG_TIMER);
			break;

		//プレイヤーのHPの文字
		case BGTYPE_PLAYER_HP:
			m_fWidth = 330.0f;
			m_fHeight = 50.0f;
			m_pObject2D->SetTexture(CTexture::TEXTURE_BG_PLAYER_HP);
			break;

		//敵のHPの文字
		case BGTYPE_ENEMY_HP:
			m_fWidth = 330.0f;
			m_fHeight = 50.0f;
			m_pObject2D->SetTexture(CTexture::TEXTURE_BG_ENEMY_HP);
			break;

		default:
			break;
		}

		m_pObject2D->SetSize(m_fWidth, m_fHeight);
	}

	return S_OK;
}

//========================
// 終了
//========================
void CBg::Uninit()
{

}

//========================
// 更新
//========================
void CBg::Update()
{
	if (m_type == BGTYPE_PRESS)
	{//キー押下指示なら
		//---------------------------
		// 点滅処理
		//---------------------------
		//点滅時間を数える
		m_nFlashTime++;

		//透明度の変更
		if (m_nFlashTime <= 60)
		{
			//不透明にする
			m_col.a -= 0.012f;
		}
		else
		{
			//半透明にする
			m_col.a += 0.012f;

			if (m_nFlashTime > 120)
			{
				m_nFlashTime = 0;	//時間のリセット
			}
		}

		//色の設定
		m_pObject2D->SetColor(m_col);
	}
}

//========================
// 描画
//========================
void CBg::Draw()
{

}

//========================
// 生成
//========================
CBg* CBg::Create(D3DXVECTOR3 pos, BGTYPE type)
{
	CBg *pBg = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pBg = new CBg;	//生成
	pBg->m_pObject2D = new CObject2D(1);

	if (pBg != nullptr)
	{//NULLチェック
		//変数に代入
		pBg->m_type = type;

		//初期化
		pBg->Init(pos);
	}

	return pBg;
}