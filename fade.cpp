//===================================================
//
// フェードの処理
// Author : Sato Teruto
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "fade.h"
#include "input.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "application.h"
#include "renderer.h"
#include "object2d.h"

//===========================
// コンストラクタ
//===========================
CFade::CFade()
{

}

//===========================
// デストラクタ
//===========================
CFade::~CFade()
{

}

//===========================
// 初期化
//===========================
HRESULT CFade::Init(CApplication::MODE modeNext)
{
	//列挙型の初期化
	m_fade = FADE_IN;		//フェードイン状態
	m_modeNext = modeNext;	//次の画面(モード)を設定
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//黒いポリゴン(不透明)にしておく

	m_pObject = new CObject2D(3);

	m_pObject->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	m_pObject->SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

	m_pObject->SetColor(m_col);

	m_pObject->SetTexture(CTexture::TEXTURE_NONE);	//テクスチャの設定

	return S_OK;
}

//===========================
// 終了
//===========================
void CFade::Uninit()
{

}

//===========================
// 更新
//===========================
void CFade::Update()
{
	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{//フェードイン状態
			m_col.a -= 0.02f;	//ポリゴンを透明にしていく

			if (m_col.a <= 0.0f)
			{
				m_col.a = 0.0f;
				m_fade = FADE_NONE;	//何もしていない状態に
			}
		}
		else if (m_fade == FADE_OUT)
		{//フェードアウト状態
			m_col.a += 0.02f;	//ポリゴンを不透明にしていく

			if (m_col.a >= 1.0f)
			{
				m_col.a = 1.0f;
				m_fade = FADE_IN;	//フェードイン状態に

				CApplication::SetMode(m_modeNext);
				m_pObject = new CObject2D(3);

				m_pObject->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

				m_pObject->SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

				m_pObject->SetColor(m_col);

				m_pObject->SetTexture(CTexture::TEXTURE_NONE);	//テクスチャの設定
			}
		}
	}
	m_pObject->SetColor(m_col);
	m_pObject->Update();		//後ろに置かないと生成された一瞬だけ透明のままになってしまう
}

//===========================
// 描画
//===========================
void CFade::Draw()
{
	m_pObject->Draw();
}

//===========================
// 設定
//===========================
void CFade::SetFade(CApplication::MODE modeNext)
{
	if (m_fade != FADE_OUT)
	{
		m_fade = FADE_OUT;		//フェードアウト状態に
		m_modeNext = modeNext;	//次の画面を設定
		m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
}