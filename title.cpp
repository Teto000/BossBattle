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
#include "title_camera.h"
#include "stage.h"
#include "meshfield.h"
#include "sky.h"
#include "enemy.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
CTitleCamera* CTitle::m_pCamera = nullptr;
CMeshField*	CTitle::m_pMeshField = nullptr;

//===========================
// コンストラクタ
//===========================
CTitle::CTitle()
{
	m_pBg = nullptr;	//背景
	m_pStage = nullptr;	//ステージ
	m_pSky = nullptr;	//空
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
	m_pCamera = new CTitleCamera;
	m_pCamera->Init();

	m_pStage = CStage::Create("data\\MODEL\\Stage.x", D3DXVECTOR3(0.0f, -100.0f, 0.0f));

	m_pMeshField = CMeshField::Create();

	m_pSky = CSky::Create(CTexture::TEXTURE_SKY);

	//背景の生成
	{
		D3DXVECTOR3 pos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100.0f, 0.0f);
		m_pBg = CBg::Create(pos, CBg::BGTYPE_TITLE);

		D3DXVECTOR3 pos2(SCREEN_WIDTH / 2,  550.0f, 0.0f);
		m_pBg = CBg::Create(pos2, CBg::BGTYPE_PRESS);
	}

	//BGMの再生
	CSound::PlaySound(CSound::SOUND_LABEL_TITLE);

	return S_OK;
}

//===========================
// 終了
//===========================
void CTitle::Uninit()
{
	////BGMの停止
	//CSound::StopSound();

	//カメラの終了
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}
}

//===========================
// 更新
//===========================
void CTitle::Update()
{
	//----------------------------
	// カメラの更新
	//----------------------------
	if (m_pCamera != nullptr)
	{
		m_pCamera->Update();
	}

	// ジョイパッドでの操作
	CInputJoypad* joypad = CApplication::GetInput()->GetJoypad();

	//画面遷移
	if (CInputKeyboard::Trigger(DIK_RETURN) || joypad->AllTrigger())
	{
		//ゲーム画面に移行
		CMode::GetFade()->SetFade(CMode::MODE_TUTORIAL);

		//SEの再生
		CSound::PlaySound(CSound::SOUND_LABEL_SE_BUTTOM);
	}
}