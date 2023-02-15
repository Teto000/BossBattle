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
#include "model.h"
#include "meshfield.h"
#include "fade.h"
#include "polygon.h"
#include "message.h"
#include "sky.h"
#include "hp.h"
#include "stage.h"
#include "item.h"
#include "time.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
bool CGame::m_bDeathEnemny = false;			//敵が死んでいるか
bool CGame::m_bFinish = false;				//終了フラグ

CCamera*	CGame::m_pCamera = nullptr;		//カメラ
CPlayer*	CGame::m_pPlayer = nullptr;		//プレイヤー
CEnemy*		CGame::m_pEnemy = nullptr;		//エネミー
CMeshField*	CGame::m_pMeshField = nullptr;	//メッシュフィールド
CPolygon*	CGame::m_pPolygon = nullptr;	//2Dポリゴン
CMessage*	CGame::m_pMessage = nullptr;	//メッセージ
CSky*		CGame::m_pSky = nullptr;		//スカイ
CStage*		CGame::m_pStage = nullptr;		//ステージ
CTime*		CGame::m_pTime = nullptr;		//タイマー

//===========================
// コンストラクタ
//===========================
CGame::CGame()
{
	nCntFinish = 0;
	nCntItem = 0;
	m_pItem = nullptr;
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
	//初期値の設定
	m_bDeathEnemny = false;
	m_bFinish = false;

	//カメラの生成
	m_pCamera = new CCamera;
	m_pCamera->Init();

	//プレイヤーの生成
	m_pPlayer = CPlayer::Create();

	//----------------------------
	// エネミーの生成
	//----------------------------
	{
		D3DXVECTOR3 pos(0.0f, 0.0f, 300.0f);
		D3DXVECTOR3 rot(0.0f, 0.0f, 0.0f);
		m_pEnemy = CEnemy::Create(pos, rot);
	}

	//メッシュフィールドの生成
	m_pMeshField = CMeshField::Create();

	//ステージの生成
	m_pStage = CStage::Create("data\\MODEL\\Stage.x", D3DXVECTOR3(0.0f, -100.0f, 0.0f));

	//空の生成
	m_pSky = CSky::Create(CTexture::TEXTURE_SKY);

	//2Dポリゴンの生成
	m_pPolygon = CPolygon::Create(CPolygon::POLYGONTYPE_AIM);

	//メッセージの生成
	{
		D3DXVECTOR3 pos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		m_pMessage = CMessage::Create(pos, 800.0f, 400.0f, CMessage::MESSAGE_CLEAR);
		m_pMessage = CMessage::Create(pos, 600.0f, 200.0f, CMessage::MESSAGE_BREAK);
	}

	//タイマーの生成
	m_pTime = CTime::Create(D3DXVECTOR3(1100.0f, 650.0f, 0.0f));

	//BGMの再生
	CSound::PlaySound(CSound::SOUND_LABEL_GAME);

	return S_OK;
}

//===========================
// 終了
//===========================
void CGame::Uninit()
{
	//BGMの停止
	CSound::StopSound();

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
	if (m_bDeathEnemny)
	{//敵が死んでいるなら
		m_pEnemy = nullptr;
		m_bFinish = true;	//終了フラグを立てる
	}

	//----------------------------
	// 終了処理
	//----------------------------
	if (m_bFinish)
	{//終了フラグが立っているなら
		nCntFinish++;	//カウントを加算

		if (nCntFinish >= 300)
		{//カウントが5秒以上なら
			//リザルト画面に移行
			CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
		}
		else if (nCntFinish >= 120)
		{//カウントが2秒以上(クリアの文字が消えたら)
			if (CInputKeyboard::Trigger(DIK_RETURN))
			{//ENTERキーが押されたら
				//リザルト画面に移行
				CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
			}
		}
	}

	//----------------------------
	// カメラの更新
	//----------------------------
	if (m_pCamera != nullptr)
	{
		m_pCamera->Update();
	}

	//----------------------------
	// アイテムの生成
	//----------------------------
	nCntItem++;	//カウントの加算

	if(nCntItem >= 60)
	{
		int nWidth = 300;

		int X = rand() % nWidth - (nWidth / 2);
		int Z = rand() % nWidth - (nWidth / 2);

		D3DXVECTOR3 pos((float)X, 100.0f, (float)Z);
		m_pItem = CItem::Create(pos, CItem::ITEMTYPE_HEAL);
		nCntItem = 0;	//リセット
	}
}