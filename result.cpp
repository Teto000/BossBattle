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
#include "input_keyboard.h"
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
		//背景の生成
		D3DXVECTOR3 pos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		m_pBg = CBg::Create(pos, CBg::BGTYPE_RESULT);
	}

	//BGMの再生
	CSound::PlaySound(CSound::SOUND_LABEL_RESULT);

	return S_OK;
}

//===========================
// 終了
//===========================
void CResult::Uninit()
{
	//BGMの停止
	CSound::StopSound();
}

//===========================
// 更新
//===========================
void CResult::Update()
{
	// ジョイパッドでの操作
	CInputJoypad* joypad = CApplication::GetInput()->GetJoypad();

	//画面遷移
	if (CInputKeyboard::AllTrigger() || joypad->AllTrigger())
	{
		//ゲーム画面に移行
		CApplication::GetFade()->SetFade(CApplication::MODE_TITLE);

		//SEの再生
		CSound::PlaySound(CSound::SOUND_LABEL_SE_BUTTOM);
	}
}