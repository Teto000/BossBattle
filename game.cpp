//===================================================
//
// ゲームの処理
// Author : Sato Teruto
//
//===================================================

//------------------------
// インクルード
//------------------------
#include <time.h>
#include "game.h"
#include "application.h"
#include "renderer.h"
#include "input.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "debug_proc.h"
#include "sound.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "meshfield.h"
#include "fade.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
CCamera*	CGame::m_pCamera = nullptr;		//カメラ
CPlayer*	CGame::m_pPlayer = nullptr;		//プレイヤー
CEnemy*		CGame::m_pEnemy = nullptr;		//エネミー
CMeshField*	CGame::m_pMeshField = nullptr;	//メッシュフィールド

//===========================
// コンストラクタ
//===========================
CGame::CGame()
{
	
}

//===========================
// デストラクタ
//===========================
CGame::~CGame()
{

}

//===========================
// 初期化
//===========================
HRESULT CGame::Init()
{
	//カメラの生成
	m_pCamera = new CCamera;
	m_pCamera->Init();

	//プレイヤーの生成
	m_pPlayer = CPlayer::Create();

	//エネミーの生成
	{
		D3DXVECTOR3 pos(0.0f, 0.0f, 300.0f);
		D3DXVECTOR3 rot(0.0f, 0.0f, 0.0f);
		m_pEnemy = CEnemy::Create(pos, rot);
	}

	//メッシュフィールドの生成
	m_pMeshField = CMeshField::Create();

	return S_OK;
}

//===========================
// 終了
//===========================
void CGame::Uninit()
{
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
void CGame::Update()
{
	//カメラの更新
	if (m_pCamera != nullptr)
	{
		m_pCamera->Update();
	}

	if (CInputKeyboard::Trigger(DIK_RETURN))
	{
		CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
	}
}