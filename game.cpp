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
#include "bg.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
bool CGame::m_bDeathEnemny = false;			//敵が死んでいるか
bool CGame::m_bFinish = false;				//終了フラグ
bool CGame::m_bStart = false;				//開始フラグ

CCamera*	CGame::m_pCamera = nullptr;		//カメラ
CPlayer*	CGame::m_pPlayer = nullptr;		//プレイヤー
CEnemy*		CGame::m_pEnemy = nullptr;		//エネミー
CMeshField*	CGame::m_pMeshField = nullptr;	//メッシュフィールド
CPolygon*	CGame::m_pPolygon = nullptr;	//2Dポリゴン
CSky*		CGame::m_pSky = nullptr;		//スカイ
CStage*		CGame::m_pStage = nullptr;		//ステージ
CTime*		CGame::m_pTime = nullptr;		//タイマー

//===========================
// コンストラクタ
//===========================
CGame::CGame()
{
	m_nCntFinish = 0;
	m_nCntItem = 0;
	m_nCntMessage = 0;		//メッセージを表示するまでの時間
	m_nNumMessage = 0;		//表示するメッセージ番号
	m_pItem = nullptr;		//アイテム
	m_pMessage = nullptr;	//メッセージ
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
	m_nNumMessage = 3;
	m_nCntMessage = 60;

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

	//タイマーの生成
	m_pTime = CTime::Create(D3DXVECTOR3(1088.0f, 592.0f, 0.0f));

	//背景の生成
	m_pBg = CBg::Create(D3DXVECTOR3(1140.0f, 350.0f, 0.0f), CBg::BGTYPE_COMBO);		//コンボ背景
	m_pBg = CBg::Create(D3DXVECTOR3(1140.0f, 580.0f, 0.0f), CBg::BGTYPE_TIMER);		//タイマー背景
	m_pBg = CBg::Create(D3DXVECTOR3(220.0f, 590.0f, 0.0f), CBg::BGTYPE_PLAYER_HP);	//プレイヤーHP背景

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
		m_nCntFinish++;	//カウントを加算

		if (m_nCntFinish >= 300)
		{//カウントが5秒以上なら
			//リザルト画面に移行
			CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
		}
		else if (m_nCntFinish >= 120)
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
	// 開始しているかどうか
	//----------------------------
	if (m_bStart == false)
	{//開始フラグが立っていないなら
		//----------------------------
		// メッセージの表示
		//----------------------------
		D3DXVECTOR3 pos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

		//時間を数える
		m_nCntMessage++;

		if (m_nCntMessage >= 60)
		{
			if (m_nNumMessage == 3)
			{//カウント3
				m_pMessage = CMessage::Create(pos, 600.0f, 300.0f, CMessage::MESSAGE_3);
			}
			else if (m_nNumMessage == 2)
			{//カウント2
				m_pMessage = CMessage::Create(pos, 600.0f, 300.0f, CMessage::MESSAGE_2);
			}
			else if (m_nNumMessage == 1)
			{//カウント1
				m_pMessage = CMessage::Create(pos, 600.0f, 300.0f, CMessage::MESSAGE_1);
			}
			else if (m_nNumMessage == 0)
			{//スタート
				m_pMessage = CMessage::Create(pos, 800.0f, 400.0f, CMessage::MESSAGE_START);
			}
			else
			{
				//開始フラグを立てる
				m_bStart = true;
			}

			//表示する文字を変更
			m_nNumMessage--;

			m_nCntMessage = 0;
		}
	}
	else
	{//開始しているなら
		//----------------------------
		// アイテムの生成
		//----------------------------
		m_nCntItem++;	//カウントの加算

		if (m_nCntItem >= 480)
		{
			int nWidth = 1500;

			int X = rand() % nWidth - (nWidth / 2);
			int Z = rand() % nWidth - (nWidth / 2);

			D3DXVECTOR3 pos((float)X, 100.0f, (float)Z);
			m_pItem = CItem::Create(pos, CItem::ITEMTYPE_HEAL);
			m_nCntItem = 0;	//リセット
		}
	}
}