//===================================================
//
// プレイヤーの処理
// Author:Teruto Sato
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "player.h"
#include "object.h"
#include "application.h"
#include "renderer.h"
#include "input.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "camera.h"
#include "debug_proc.h"
#include "model.h"
#include "game.h"
#include "line.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
const float CPlayer::fPlayerSpeed = 7.0f;

//------------------------
// グローバル変数
//------------------------
CPlayer::KEY_SET g_aKeySet[] =	//キーセット情報
{
	//----------------------
	// キー1
	//----------------------
	{ 40,	//フレーム数
		//		Pos				Rot
		{{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//タイヤ
		{ 0.0f,0.0f,0.0f , -1.0f,0.0f,0.0f },	//体
		{ 0.0f,0.0f,0.0f , 0.9f,0.0f,0.0f },	//頭
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,-0.56f },	//右腕
		{ 0.0f,0.0f,0.0f , 0.75f,0.5f,0.0f },	//右手
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.5f },	//左腕
		{ 0.0f,0.0f,0.0f , 0.4f,0.0f,0.0f }},	//左手
	},

	//----------------------
	// キー2
	//----------------------
	{ 40,	//フレーム数
			//		Pos				Rot
	{ { 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//タイヤ
	{ 0.0f,0.0f,0.0f , -1.0f,0.0f,0.0f },	//体
	{ 0.0f,0.0f,0.0f , 0.9f,0.0f,0.0f },	//頭
	{ 0.0f,0.0f,0.0f , 0.0f,0.0f,-0.56f },	//右腕
	{ 0.0f,0.0f,0.0f , 0.75f,0.5f,0.0f },	//右手
	{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.5f },	//左腕
	{ 0.0f,0.0f,0.0f , 0.4f,0.0f,0.0f } },	//左手
	},
};

//========================
// コンストラクタ
//========================
CPlayer::CPlayer() : CObject(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//大きさの最大値
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//大きさの最小値
	m_worldMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//ワールド上の最大値
	m_worldMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//ワールド上の最小値

	//モデル
	for (int i = 0; i < MAX_PARTS; i++)
	{
		m_pModel[i] = nullptr;
	}

	//線
	for (int i = 0; i < nMaxLine; i++)
	{
		m_pLIne[i] = nullptr;
	}

	/* ↓ モーション情報 ↓ */
	m_nCurrentKey = 0;
	m_nCntMotion = 0;
}

//========================
// デストラクタ
//========================
CPlayer::~CPlayer()
{

}

//========================
// 初期化
//========================
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	m_pos = pos;
	m_nCntMotion = 1;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//回転マトリックスの初期化
	D3DXMatrixIdentity(&m_mtxRot);

	//--------------------
	// モデルの生成
	//--------------------
	SetModel();

	//-----------------------
	// モデルの大きさを取得
	//-----------------------
	GetSize();

	//-----------------------
	// 線の表示
	//-----------------------
	//SetLine();

	return S_OK;
}

//========================
// 終了
//========================
void CPlayer::Uninit()
{
	//-------------------------
	// モデルの終了
	//-------------------------
	for (int i = 0; i < MAX_PARTS; i++)
	{//モデルパーツ数分回す
		if (m_pModel[i])
		{//モデルがnullじゃないなら
			m_pModel[i]->Uninit();
			delete m_pModel[i];
			m_pModel[i] = nullptr;
		}
	}

	CObject::Release();
}

//========================
// 更新
//========================
void CPlayer::Update()
{
	//-------------------------
	// モデルの更新
	//-------------------------
	for (int i = 0; i < MAX_PARTS; i++)
	{//モデルパーツ数分回す
		if (m_pModel[i])
		{//モデルがnullじゃないなら
			m_pModel[i]->Update();
		}
	}

	//-------------------------
	// 移動
	//-------------------------
	MoveKeyboard(DIK_W, DIK_S, DIK_A, DIK_D);	//キーボード
	MoveJoypad();	//ジョイパッド

	//タイヤの回転
	m_pModel[0]->SetRotX(m_rotWheel);

	if (CInputKeyboard::Press(DIK_Z))
	{
		m_pModel[1]->SetRotX(m_rotWheel / 2);
	}

	//D3DXMatrix

	//-------------------------
	// 回転
	//-------------------------
	//回転軸の設定
	/*m_vecAxis.x = sinf(D3DXToRadian(90));
	m_vecAxis.z = sinf(D3DXToRadian(90));

	//回転角の設定
	if (CInputKeyboard::Press(DIK_A))
	{//Aキーが押された
		m_fValueRot = D3DXToRadian(fPlayerSpeed);
	}
	else if (CInputKeyboard::Press(DIK_D))
	{//Dキーが押された
		m_fValueRot = -D3DXToRadian(fPlayerSpeed);
	}

	if (CInputKeyboard::Press(DIK_W))
	{//Wキーが押された
		m_fValueRot = D3DXToRadian(fPlayerSpeed);
	}
	else if (CInputKeyboard::Press(DIK_S))
	{//Sキーが押された
		m_fValueRot = -D3DXToRadian(fPlayerSpeed);
	}

	if (!CInputKeyboard::Press(DIK_A) && !CInputKeyboard::Press(DIK_D)
		&& !CInputKeyboard::Press(DIK_W) && !CInputKeyboard::Press(DIK_S))
	{//移動していないなら
		m_fValueRot = 0.0f;
	}*/

	//-------------------------
	// モーション
	//-------------------------
	SetMotion(true);

	//-------------------------
	// 線の更新
	//-------------------------
	//UpdateLine();
}

//========================
// 描画
//========================
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//モデルのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//モデルの向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//---------------------------------------
	// クォータニオンを使った回転の反映
	//---------------------------------------
	/*//クォータニオン作成
	//引数：クォータニオン、回転軸、回転角
	D3DXQuaternionRotationAxis(&m_quat, &m_vecAxis, m_fValueRot);

	//クォータニオンから回転マトリックスを作成
	//引数：回転マトリックス、クォータニオン
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quat);

	//回転マトリックス(保存用)に反映
	D3DXMatrixMultiply(&m_mtxRot, &m_mtxRot, &mtxRot);

	//ワールドマトリックスに反映
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);*/

	//モデルの位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//-------------------------
	// モデルの描画
	//-------------------------
	for (int i = 0; i < MAX_PARTS; i++)
	{//モデルパーツ数分回す
		if (!m_pModel[i])
		{//モデルがnullなら
			return;
		}

		m_pModel[i]->Draw(&m_mtxWorld);
	}
}

//========================
// 生成
//========================
CPlayer* CPlayer::Create()
{
	CPlayer *pPlayer = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pPlayer = new CPlayer;	//生成

	if (pPlayer != nullptr)
	{//NULLチェック
		//初期化
		pPlayer->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pPlayer->SetObjType(OBJTYPE_PLAYER);
	}

	return pPlayer;
}

//========================
// モデルの生成
//========================
void CPlayer::SetModel()
{
	//モデル0：タイヤ
	m_pModel[0] = CModel::Create("data\\MODEL\\wheel.x", nullptr,
		D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//モデル1：体
	m_pModel[1] = CModel::Create("data\\MODEL\\body.x", nullptr,
		D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//モデル2：頭
	m_pModel[2] = CModel::Create("data\\MODEL\\head.x", m_pModel[1],
		D3DXVECTOR3(0.0f, 105.0f, 12.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//モデル3：右腕
	m_pModel[3] = CModel::Create("data\\MODEL\\armR.x", m_pModel[1],
		D3DXVECTOR3(-25.0f, 85.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//モデル4：右手
	m_pModel[4] = CModel::Create("data\\MODEL\\handR.x", m_pModel[3],
		D3DXVECTOR3(-4.0f, -40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//モデル5：左腕
	m_pModel[5] = CModel::Create("data\\MODEL\\armL.x", m_pModel[1],
		D3DXVECTOR3(25.0f, 85.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//モデル6：左手
	m_pModel[6] = CModel::Create("data\\MODEL\\handL.x", m_pModel[5],
		D3DXVECTOR3(4.0f, -40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//========================
// モーションの設定
//========================
void CPlayer::SetMotion(bool bLoop)
{
	if (m_nCurrentKey + 1 >= MAX_KEY)
	{//キーが最大数に達したら
		if (bLoop)
		{
			m_nCurrentKey = 0;	//キー番号の初期化
		}
		else
		{
			return;
		}
	}

	for (int i = 1; i < MAX_PARTS; i++)
	{//モデルパーツ数分回す
		//-----------------------------------------
		// NULLチェック
		//-----------------------------------------
		if (!m_pModel[i])
		{//モデルパーツがnullなら
			return;
		}

		//キー情報を持った変数
		KEY key = g_aKeySet[m_nCurrentKey].aKey[i];
		KEY keyNext = g_aKeySet[m_nCurrentKey + 1].aKey[i];

		//-----------------------------------------
		// 現在値を取得
		//-----------------------------------------
		//位置を取得
		float fPosX = m_pModel[i]->GetPos().x;
		float fPosY = m_pModel[i]->GetPos().y;
		float fPosZ = m_pModel[i]->GetPos().z;

		//向きを取得
		float fRotX = m_pModel[i]->GetRot().x;
		float fRotY = m_pModel[i]->GetRot().y;
		float fRotZ = m_pModel[i]->GetRot().z;

		//-----------------------------------------
		// 差分の計算
		// (終了値 - 開始値)
		//-----------------------------------------
		//位置
		float fDifPosX = keyNext.fPosX - key.fPosX;
		float fDifPosY = keyNext.fPosY - key.fPosY;
		float fDifPosZ = keyNext.fPosZ - key.fPosZ;

		//向き
		float fDifRotX = keyNext.fRotX - key.fRotX;
		float fDifRotY = keyNext.fRotY - key.fRotY;
		float fDifRotZ = keyNext.fRotZ - key.fRotZ;

		//-----------------------------------------
		// 相対値の計算
		// (モーションカウンター / フレーム数)
		//-----------------------------------------
		float fNumRelative = m_nCntMotion / (float)g_aKeySet[m_nCurrentKey].nFrame;

		//-----------------------------------------
		// 現在値の計算
		// (開始値 + (差分 * 相対値))
		//-----------------------------------------
		//位置
		fPosX += key.fPosX + (fDifPosX * fNumRelative);
		fPosY += key.fPosY + (fDifPosY * fNumRelative);
		fPosZ += key.fPosZ + (fDifPosZ * fNumRelative);

		//向き
		fRotX = key.fRotX + (fDifRotX * fNumRelative);
		fRotY = key.fRotY + (fDifRotY * fNumRelative);
		fRotZ = key.fRotZ + (fDifRotZ * fNumRelative);

		//-----------------------------------------
		// モデル情報の設定
		//-----------------------------------------
		//位置の設定
		m_pModel[i]->SetPos(D3DXVECTOR3(fPosX, fPosY, fPosZ));

		//向きの設定
		m_pModel[i]->SetRot(D3DXVECTOR3(fRotX, fRotY, fRotZ));
	}

	//モーションカウンターを進める
	m_nCntMotion++;

	//-------------------------
	// 初期化
	//-------------------------
	if (m_nCntMotion >= g_aKeySet[m_nCurrentKey].nFrame)
	{//モーションカウンターが再生フレームに達したら
		m_nCurrentKey++;	//キー番号を加算
		m_nCntMotion = 0;	//モーションカウンターを初期化
	}
}

//=====================================
// 移動
// 引数：上キー,下キー,左キー,右キー
//=====================================
void CPlayer::MoveKeyboard(int nUpKey, int nDownKey, int nLeftKey, int nRightKey)
{
	//カメラの情報取得
	D3DXVECTOR3 cameraRot = CGame::GetCamera()->GetRot();

	//-------------------------------
	// プレイヤーの操作
	//-------------------------------
	if (CInputKeyboard::Press(nLeftKey))
	{//Aキーが押された
		if (CInputKeyboard::Press(nUpKey))
		{//Wキーが押された
			m_pos.x += sinf(cameraRot.y - D3DX_PI * 0.25f) * fPlayerSpeed;	//左奥移動
			m_pos.z += cosf(cameraRot.y - D3DX_PI * 0.25f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.75f;	//向きの切り替え
		}
		else if (CInputKeyboard::Press(nDownKey))
		{//Sキーが押された
			m_pos.x += sinf(cameraRot.y - D3DX_PI * 0.75f) * fPlayerSpeed;	//左前移動
			m_pos.z += cosf(cameraRot.y - D3DX_PI * 0.75f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.25f;
		}
		else
		{
			m_pos.x -= sinf(cameraRot.y + D3DX_PI * 0.5f) * fPlayerSpeed;	//左移動
			m_pos.z -= cosf(cameraRot.y + D3DX_PI * 0.5f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.5f;
		}
	}
	else if (CInputKeyboard::Press(nRightKey))
	{//Dキーが押された
		if (CInputKeyboard::Press(nUpKey))
		{//Wキーが押された
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.25f) * fPlayerSpeed;	//右奥移動
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.25f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.75f;
		}
		else if (CInputKeyboard::Press(nDownKey))
		{//Sキーが押された
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.75f) * fPlayerSpeed;	//右前移動
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.75f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.25f;
		}
		else
		{
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.5f) * fPlayerSpeed;	//右移動
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.5f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.5f;
		}
	}
	else if (CInputKeyboard::Press(nUpKey))
	{//Wキーが押された
		m_pos.x -= sinf(cameraRot.y + D3DX_PI * 1.0f) * fPlayerSpeed;	//奥移動
		m_pos.z -= cosf(cameraRot.y + D3DX_PI * 1.0f) * fPlayerSpeed;
		m_rotDest.y = cameraRot.y + D3DX_PI * 1.0f;
	}
	else if (CInputKeyboard::Press(nDownKey))
	{//Sキーが押された
		m_pos.x -= sinf(cameraRot.y + D3DX_PI * 0.0f) * fPlayerSpeed;	//前移動
		m_pos.z -= cosf(cameraRot.y + D3DX_PI * 0.0f) * fPlayerSpeed;
		m_rotDest.y = cameraRot.y + D3DX_PI * 0.0f;
	}

	//向きを目的の角度に合わせる
	SetRot();

	//タイヤの回転量の加算
	m_rotWheel += D3DXToRadian(-nWheelRotValue);

	if (!CInputKeyboard::Press(nUpKey) && !CInputKeyboard::Press(nDownKey)
		&& !CInputKeyboard::Press(nRightKey) && !CInputKeyboard::Press(nLeftKey))
	{//移動キーが押されていないなら
		//タイヤの回転量を0にする
		m_rotWheel = 0;
	}
}

//===========================
// ジョイパッドを使った移動
//===========================
void CPlayer::MoveJoypad()
{
	// ジョイパッドでの操作
	CInputJoypad* joypad = CApplication::GetJoypad();
	D3DXVECTOR3 stick = joypad->Stick(CInputJoypad::JOYKEY_LEFT_STICK, 0);

	if (joypad->IsJoyPadUse(0) == false)
	{//ジョイパッドが使われていないなら
		return;
	}

	//スティックを動かす値の設定
	float fMoveValue = 0.5f;

	//カメラの情報取得
	D3DXVECTOR3 cameraRot = CGame::GetCamera()->GetRot();

	//-----------------------------------
	// 右移動
	//-----------------------------------
	if (stick.x >= fMoveValue)
	{
		// スティックを倒した方向へ移動する
		if (stick.y <= -fMoveValue)
		{//右奥移動
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.25f) * fPlayerSpeed;
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.25f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.75f;	//向きの切り替え
		}
		else if (stick.y >= fMoveValue)
		{//右前移動
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.75f) * fPlayerSpeed;
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.75f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.25f;
		}
		else
		{
			m_pos.x += sinf(cameraRot.y + D3DX_PI * fMoveValue) * fPlayerSpeed;
			m_pos.z += cosf(cameraRot.y + D3DX_PI * fMoveValue) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * fMoveValue;
		}
	}
	//-----------------------------------
	// 左移動
	//-----------------------------------
	else if (stick.x <= -fMoveValue)
	{
		if (stick.y <= -fMoveValue)
		{//左奥移動
			m_pos.x += sinf(cameraRot.y - D3DX_PI * 0.25f) * fPlayerSpeed;
			m_pos.z += cosf(cameraRot.y - D3DX_PI * 0.25f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.75f;
		}
		else if (stick.y >= fMoveValue)
		{//左前移動
			m_pos.x += sinf(cameraRot.y - D3DX_PI * 0.75f) * fPlayerSpeed;
			m_pos.z += cosf(cameraRot.y - D3DX_PI * 0.75f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.25f;
		}
		else
		{
			m_pos.x -= sinf(cameraRot.y + D3DX_PI * fMoveValue) * fPlayerSpeed;
			m_pos.z -= cosf(cameraRot.y + D3DX_PI * fMoveValue) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * fMoveValue;
		}
	}
	//-----------------------------------
	// 奥移動
	//-----------------------------------
	else if (stick.y <= -fMoveValue)
	{
		m_pos.x -= sinf(cameraRot.y + D3DX_PI * 1.0f) * fPlayerSpeed;
		m_pos.z -= cosf(cameraRot.y + D3DX_PI * 1.0f) * fPlayerSpeed;
		m_rotDest.y = cameraRot.y + D3DX_PI * 1.0f;
	}
	//-----------------------------------
	// 前移動
	//-----------------------------------
	else if (stick.y >= fMoveValue)
	{
		m_pos.x -= sinf(cameraRot.y + D3DX_PI * 0.0f) * fPlayerSpeed;
		m_pos.z -= cosf(cameraRot.y + D3DX_PI * 0.0f) * fPlayerSpeed;
		m_rotDest.y = cameraRot.y + D3DX_PI * 0.0f;
	}

	//向きを目的の角度に合わせる
	SetRot();

	//タイヤの回転量の加算
	m_rotWheel += D3DXToRadian(-nWheelRotValue);

	if (stick.x < fMoveValue && stick.x > -fMoveValue 
		&& stick.y < fMoveValue && stick.y > -fMoveValue)
	{//スティックが動かされていないなら
		//タイヤの回転量を0にする
		m_rotWheel = 0;
	}
}

//===========================
// 角度の設定
//===========================
void CPlayer::SetRot()
{
	//-------------------------------
	// 目的の角度の正規化
	//-------------------------------
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{//回転したい角度が180以上なら
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{//回転したい角度が-180以下なら
		m_rotDest.y += D3DX_PI * 2;
	}

	//-------------------------------
	// 目的の角度まで回転する
	//-------------------------------
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.04f;	//減衰処理

	//-------------------------------
	// 角度の正規化
	//-------------------------------
	if (m_rot.y > D3DX_PI)
	{//角度が180以上なら
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{//角度が-180以下なら
		m_rot.y += D3DX_PI * 2;
	}
}

//========================
// 線の設置
//========================
void CPlayer::SetLine()
{
	//線の色
	D3DXCOLOR lineCol(1.0f, 0.0f, 0.0f, 1.0f);

	//ワールド変換行列を使ってMin,Maxを求める
	D3DXVec3TransformCoord(&m_worldMin, &m_vtxMin, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldMax, &m_vtxMax, &m_mtxWorld);

	//代入する値をまとめる
	D3DXVECTOR3 min = m_worldMin;
	D3DXVECTOR3 max = m_worldMax;

	//-----------------------------------
	// 下辺
	//-----------------------------------
	D3DXVECTOR3 start = D3DXVECTOR3(min.x, min.y, min.z);
	D3DXVECTOR3 end = D3DXVECTOR3(max.x, min.y, min.z);
	m_pLIne[0] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, min.y, min.z);
	end = D3DXVECTOR3(min.x, min.y, max.z);
	m_pLIne[1] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(max.x, min.y, min.z);
	end = D3DXVECTOR3(max.x, min.y, max.z);
	m_pLIne[2] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, min.y, max.z);
	end = D3DXVECTOR3(max.x, min.y, max.z);
	m_pLIne[3] = CLine::Create(m_pos, start, end, lineCol);

	//-----------------------------------
	// 上辺
	//-----------------------------------
	start = D3DXVECTOR3(min.x, max.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, min.z);
	m_pLIne[4] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, max.y, min.z);
	end = D3DXVECTOR3(min.x, max.y, max.z);
	m_pLIne[5] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(max.x, max.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLIne[6] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, max.y, max.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLIne[7] = CLine::Create(m_pos, start, end, lineCol);

	//-----------------------------------
	// 縦辺
	//-----------------------------------
	start = D3DXVECTOR3(min.x, min.y, min.z);
	end = D3DXVECTOR3(min.x, max.y, min.z);
	m_pLIne[8] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, min.y, max.z);
	end = D3DXVECTOR3(min.x, max.y, max.z);
	m_pLIne[9] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(max.x, min.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, min.z);
	m_pLIne[10] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(max.x, min.y, max.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLIne[11] = CLine::Create(m_pos, start, end, lineCol);
}

//========================
// 線の情報の更新
//========================
void CPlayer::UpdateLine()
{
	//ワールド変換行列を使ってMin,Maxを求める
	D3DXVec3TransformCoord(&m_worldMin, &m_vtxMin, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldMax, &m_vtxMax, &m_mtxWorld);

	//最大最小を求めなおす

	//剣を含まないでけいさんする

	//代入する値をまとめる
	D3DXVECTOR3 min = m_worldMin;
	D3DXVECTOR3 max = m_worldMax;

	//-----------------------------------
	// 下辺
	//-----------------------------------
	D3DXVECTOR3 start = D3DXVECTOR3(min.x, min.y, min.z);
	D3DXVECTOR3 end = D3DXVECTOR3(max.x, min.y, min.z);
	m_pLIne[0]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, min.y, min.z);
	end = D3DXVECTOR3(min.x, min.y, max.z);
	m_pLIne[1]->SetLine(start, end);

	start = D3DXVECTOR3(max.x, min.y, min.z);
	end = D3DXVECTOR3(max.x, min.y, max.z);
	m_pLIne[2]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, min.y, max.z);
	end = D3DXVECTOR3(max.x, min.y, max.z);
	m_pLIne[3]->SetLine(start, end);

	//-----------------------------------
	// 上辺
	//-----------------------------------
	start = D3DXVECTOR3(min.x, max.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, min.z);
	m_pLIne[4]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, max.y, min.z);
	end = D3DXVECTOR3(min.x, max.y, max.z);
	m_pLIne[5]->SetLine(start, end);

	start = D3DXVECTOR3(max.x, max.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLIne[6]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, max.y, max.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLIne[7]->SetLine(start, end);

	//-----------------------------------
	// 縦辺
	//-----------------------------------
	start = D3DXVECTOR3(min.x, min.y, min.z);
	end = D3DXVECTOR3(min.x, max.y, min.z);
	m_pLIne[8]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, min.y, max.z);
	end = D3DXVECTOR3(min.x, max.y, max.z);
	m_pLIne[9]->SetLine(start, end);

	start = D3DXVECTOR3(max.x, min.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, min.z);
	m_pLIne[10]->SetLine(start, end);

	start = D3DXVECTOR3(max.x, min.y, max.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLIne[11]->SetLine(start, end);
}

//===========================
// 大きさの取得
//===========================
void CPlayer::GetSize()
{
	for (int i = 0; i < MAX_PARTS; i++)
	{
		//-------------------------------
		// 最小値の比較
		//-------------------------------
		D3DXVECTOR3 vtxMin = m_pModel[i]->GetVtxMin();

		if (vtxMin.x <= m_vtxMin.x)
		{
			m_vtxMin.x = floorf(m_vtxMin.x);
		}
		if (vtxMin.y <= m_vtxMin.y)
		{
			m_vtxMin.y = floorf(m_vtxMin.y);
		}
		if (vtxMin.z <= m_vtxMin.z)
		{
			m_vtxMin.z = floorf(m_vtxMin.z);
		}

		//-------------------------------
		// 最大値の比較
		//-------------------------------
		D3DXVECTOR3 vtxMax = m_pModel[i]->GetVtxMax();

		if (vtxMax.x >= m_vtxMax.x)
		{
			m_vtxMax.x = floorf(vtxMax.x);
		}
		if (vtxMax.y >= m_vtxMax.y)
		{
			m_vtxMax.y = floorf(vtxMax.y);
		}
		if (vtxMax.z >= m_vtxMax.z)
		{
			m_vtxMax.z = floorf(vtxMax.z);
		}
	}
}

//===========================
// 位置の設定
//===========================
void CPlayer::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===========================
// 位置の取得
//===========================
D3DXVECTOR3 CPlayer::GetPosition()
{
	return m_pos;
}

//========================
// 向きの取得
//========================
D3DXVECTOR3 CPlayer::GetRot()
{
	return m_rot;
}

//===========================
// 幅の取得
//===========================
float CPlayer::GetWidth()
{
	return 0.0f;
}

//===========================
// 高さの取得
//===========================
float CPlayer::GetHeight()
{
	return 0.0f;
}

//===========================
// ワールドマトリックスの取得
//===========================
D3DXMATRIX CPlayer::GetmtxWorld()
{
	return m_mtxWorld;
}