//===================================================
//
// チュートリアルの処理
// Author : Sato Teruto
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "tutorial.h"
#include "renderer.h"
#include "input.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "application.h"
#include "sound.h"
#include "bg.h"
#include "fade.h"
#include "ranking.h"

//===========================
// コンストラクタ
//===========================
CTutorial::CTutorial()
{

}

//===========================
// デストラクタ
//===========================
CTutorial::~CTutorial()
{
	m_nNumber = 0;	//番号

	for (int i = 0; i < nMaxBg; i++)
	{
		m_pBg[i] = nullptr;	//背景
	}
}

//===========================
// 初期化
//===========================
HRESULT CTutorial::Init()
{
	m_nNumber = TYPE_MANUAL;;

	{
		//背景の生成
		D3DXVECTOR3 pos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		m_pBg[m_nNumber] = CBg::Create(pos, CBg::BGTYPE_MANUAL);
	}

	////BGMの再生
	//CSound::PlaySound(CSound::SOUND_LABEL_TITLE);

	return S_OK;
}

//===========================
// 終了
//===========================
void CTutorial::Uninit()
{
	//BGMの停止
	CSound::StopSound();
}

//===========================
// 更新
//===========================
void CTutorial::Update()
{
	// ジョイパッドでの操作
	CInputJoypad* joypad = CApplication::GetInput()->GetJoypad();

	//生成位置
	D3DXVECTOR3 pos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

	//画面遷移
	if (CInputKeyboard::Trigger(DIK_RETURN) || joypad->Trigger(CInputJoypad::JOYKEY_B))
	{
		if (m_nNumber < TYPE_GAME && m_pBg[m_nNumber])
		{
			m_pBg[m_nNumber]->Uninit();
			m_pBg[m_nNumber] = nullptr;

			m_nNumber++;
		}

		//SEの再生
		CSound::PlaySound(CSound::SOUND_LABEL_SE_BUTTOM);
	}
	else if (CInputKeyboard::Trigger(DIK_BACK) || joypad->Trigger(CInputJoypad::JOYKEY_A))
	{
		if (m_nNumber > 0 && m_pBg[m_nNumber])
		{
			m_pBg[m_nNumber]->Uninit();
			m_pBg[m_nNumber] = nullptr;

			m_nNumber--;
		}

		//SEの再生
		CSound::PlaySound(CSound::SOUND_LABEL_SE_BUTTOM);
	}

	//-------------------------------
	// 画面切り替え
	//-------------------------------
	if (m_nNumber == TYPE_MANUAL)
	{//0番なら
		m_pBg[m_nNumber] = CBg::Create(pos, CBg::BGTYPE_MANUAL);
	}
	else if (m_nNumber == TYPE_TUTORIAL1)
	{//1番なら
		m_pBg[m_nNumber] = CBg::Create(pos, CBg::BGTYPE_TUTORIAL1);
	}
	else if (m_nNumber == TYPE_TUTORIAL2)
	{//2番なら
		m_pBg[m_nNumber] = CBg::Create(pos, CBg::BGTYPE_TUTORIAL2);
	}
	else
	{
		//ゲーム画面に移行
		CApplication::GetFade()->SetFade(CApplication::MODE_GAME);
	}
}