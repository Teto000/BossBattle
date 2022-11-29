//===================================================
//
// リザルトの処理
// Author : Sato Teruto
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "result.h"
#include "renderer.h"
#include "input.h"
#include "input_keybord.h"
#include "input_joypad.h"
#include "application.h"
#include "sound.h"
#include "bg.h"
#include "fade.h"

//===========================
// コンストラクタ
//===========================
CResult::CResult()
{

}

//===========================
// デストラクタ
//===========================
CResult::~CResult()
{
	m_pBg = nullptr;	//背景
}

//===========================
// 初期化
//===========================
HRESULT CResult::Init()
{
	{
		D3DXVECTOR3 pos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		m_pBg = CBg::Create(pos, CBg::BGTYPE_RESULT);
	}

	return S_OK;
}

//===========================
// 終了
//===========================
void CResult::Uninit()
{

}

//===========================
// 更新
//===========================
void CResult::Update()
{
	if (CInputKeyboard::Trigger(DIK_RETURN))
	{
		CApplication::GetFade()->SetFade(CApplication::MODE_TITLE);
	}
}