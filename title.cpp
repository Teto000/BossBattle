//===================================================
//
// タイトルの処理
// Author : Sato Teruto
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "title.h"
#include "renderer.h"
#include "input.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "application.h"
#include "fade.h"
#include "sound.h"
#include "game.h"
#include "bg.h"

//===========================
// コンストラクタ
//===========================
CTitle::CTitle()
{
	m_pBg = nullptr;	//背景
}

//===========================
// デストラクタ
//===========================
CTitle::~CTitle()
{

}

//===========================
// 初期化
//===========================
HRESULT CTitle::Init()
{
	{
		D3DXVECTOR3 pos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		m_pBg = CBg::Create(pos, CBg::BGTYPE_TITLE);
	}

	return S_OK;
}

//===========================
// 終了
//===========================
void CTitle::Uninit()
{

}

//===========================
// 更新
//===========================
void CTitle::Update()
{
	if (CInputKeyboard::Trigger(DIK_RETURN))
	{
		CApplication::GetFade()->SetFade(CApplication::MODE_GAME);
	}
}